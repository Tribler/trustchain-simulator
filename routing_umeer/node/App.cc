//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <vector>
#include <omnetpp.h>
#include "Packet_m.h"

using namespace omnetpp;

/**
 * Generates traffic for the network.
 */

const int INITIAL_MONEY = 10;

class TrustChainElement {
public:
    int partnerId, partnerSeqNum, transactionValue; // values limited to +- 2147483647

    TrustChainElement(int partnerId, int partnerSeqNum, int transactionValue) {
        this->partnerId = partnerId;
        this->partnerSeqNum = partnerSeqNum;
        this->transactionValue = transactionValue;
    }
};

class LogDatabaseElement {
public:
    int UserAId, UserASeqNum, UserBId, UserBSeqNum, transactionValue; // values limited to +- 2147483647

    LogDatabaseElement(int UserAId, int UserASeqNum, int UserBId,
            int UserBSeqNum, int transactionValue) {
        this->UserAId = UserAId;
        this->UserASeqNum = UserASeqNum;
        this->UserBId = UserBId;
        this->UserBSeqNum = UserBSeqNum;
        this->transactionValue = transactionValue;
    }
};

class App: public cSimpleModule {
private:
    // configuration
    int myAddress;
    std::vector<int> destAddresses;
    cPar *sendIATime;
    cPar *packetLengthBytes;

    // state
    //cMessage *generatePacket;
    cMessage *timerThread;
    long pkCounter;
    long chainTotalValue;

    int waitingForAck; //0 - IDLE 1 - Transaction Sent (waiting for ack)
    int waitingForChainLog; //2 - Chain Request (waiting for chain)

    int tempBlockID;
    int tempPartnerSeqNum;
    int tempBlockTransaction;

    // signals
    simsignal_t endToEndDelaySignal;
    simsignal_t hopCountSignal;
    simsignal_t sourceAddressSignal;

    //TrustChain Storage
    std::vector<TrustChainElement> trustChain;
    std::vector<LogDatabaseElement> logDatabase;

public:
    App();
    virtual ~App();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    //Utility functions
    virtual void threadFunction();
    virtual void receiveMessage(cMessage *msg);
    virtual int randomNodeAddressPicker();
    virtual void registerNewChainNode(int id, int seqNum, int value);
    virtual void calculateChainValue();

    //Messaging
    virtual void createTransactionMessage();
    virtual void createChainRequestMessage();
    virtual void createChainLogMessage();
    virtual void createAckMessage();
    virtual void createDisseminationMessage();

    virtual void verificationTransactionChain(Packet *pk);
    virtual void logTransactionChain(Packet *pk);
    virtual bool isAlreadyPresentInDb(LogDatabaseElement *element);
};

Define_Module(App);

App::App() {
    //generatePacket = nullptr;
    timerThread = nullptr;
}

App::~App() {
    //cancelAndDelete(generatePacket);
    cancelAndDelete(timerThread);
}

void App::initialize() {
    myAddress = par("address");
    packetLengthBytes = &par("packetLength");
    sendIATime = &par("sendIaTime");  // volatile parameter
    pkCounter = 0;

    tempBlockID = 0;
    tempPartnerSeqNum = 0;
    tempBlockTransaction = 0;
    waitingForAck = 0;
    waitingForChainLog = 0;

    //Information To Log
    WATCH(pkCounter);
    WATCH(myAddress);
    WATCH(chainTotalValue);

    const char *destAddressesPar = par("destAddresses");
    cStringTokenizer tokenizer(destAddressesPar);
    const char *token;
    while ((token = tokenizer.nextToken()) != nullptr)
        destAddresses.push_back(atoi(token));

    if (destAddresses.size() == 0)
        throw cRuntimeError(
                "At least one address must be specified in the destAddresses parameter!");

    // TrustChain initialization
    registerNewChainNode(-1, -1, INITIAL_MONEY);

    //Start the recursive thread
    timerThread = new cMessage("TimerThead");
    scheduleAt(sendIATime->doubleValue(), timerThread);

    endToEndDelaySignal = registerSignal("endToEndDelay");
    hopCountSignal = registerSignal("hopCount");
    sourceAddressSignal = registerSignal("sourceAddress");
}

void App::handleMessage(cMessage *msg) {
    if (msg == timerThread) {
        threadFunction();
    } else {
        receiveMessage(msg);
    }
}

void App::threadFunction() {
    if (waitingForAck == 0) { //Not transaction are pending

        //Just for Production Test (ONLY 0 CAN SEND MONEY)
        if (myAddress != 0) {
            return;
        }

        //Check if i have the money for the transaction
        calculateChainValue();
        if (chainTotalValue <= 0) {
            return;
        }

        createTransactionMessage();
        waitingForAck = 1;
    }

    scheduleAt(simTime() + sendIATime->doubleValue(), timerThread);
}

void App::receiveMessage(cMessage *msg) {
    // Handle incoming packet
    Packet *pk = check_and_cast<Packet *>(msg);

    EV << "received packet " << pk->getName() << " after " << pk->getHopCount()
              << "hops" << endl;
    emit(endToEndDelaySignal, simTime() - pk->getCreationTime());
    emit(hopCountSignal, pk->getHopCount());
    emit(sourceAddressSignal, pk->getSrcAddr());

    switch (pk->getPacketType()) {
    case 0: { // Transaction Request Received
        tempBlockID = pk->getSrcAddr();
        tempPartnerSeqNum = pk->getMyChainSeqNum();
        tempBlockTransaction = pk->getTransactionValue();
        createChainRequestMessage();
        waitingForChainLog = 1;
        break;
    }
    case 1: { // Chain Request Received
        createChainLogMessage();
        break;
    }
    case 2: { // Partner Chain Received
        verificationTransactionChain(pk);
        logTransactionChain(pk);

        waitingForChainLog = 0;
        registerNewChainNode(tempBlockID, tempPartnerSeqNum,
                tempBlockTransaction);
        createAckMessage();
        createDisseminationMessage();
        tempBlockID = 0;
        tempBlockTransaction = 0;
        break;
    }
    case 3: { // Ack Received
        if (pk->getSrcAddr() == tempBlockID) {
            registerNewChainNode(tempBlockID, pk->getMyChainSeqNum(),
                    -tempBlockTransaction);
            createDisseminationMessage();
            tempBlockID = 0;
            tempBlockTransaction = 0;
            waitingForAck = 0;
        }
        break;
    }
    default: {
        break;
    }
    }
    delete pk;

    if (hasGUI())
        getParentModule()->bubble("Arrived!");
}

void App::createTransactionMessage() {
    //Calculate transaction value
    int transactionValue = intuniform(1, chainTotalValue);

    // Sending packet
    int destAddress = randomNodeAddressPicker();

    tempBlockID = destAddress;
    tempBlockTransaction = transactionValue;

    char pkname[40];
    sprintf(pkname, "#%ld from-%d-to-%d $%d", pkCounter++, myAddress,
            destAddress, transactionValue);
    //EV << "generating packet " << pkname << endl;

    if (hasGUI())
        getParentModule()->bubble("Generating packet...");

    //Packet Creation
    Packet *pk = new Packet(pkname);
    pk->setByteLength(packetLengthBytes->intValue());
    pk->setSrcAddr(myAddress);
    pk->setDestAddr(destAddress);

    pk->setPacketType(0);
    pk->setTransactionValue(transactionValue);
    pk->setMyChainSeqNum(trustChain.size() + 1);

    send(pk, "out");
}

void App::createChainRequestMessage() {
    int destAddress = tempBlockID;

    char pkname[40];
    sprintf(pkname, "#%ld from-%d-to-%d chain request", pkCounter++, myAddress,
            destAddress);
    //EV << "generating packet " << pkname << endl;

    if (hasGUI())
        getParentModule()->bubble("Generating packet...");

    //Packet Creation
    Packet *pk = new Packet(pkname);
    pk->setByteLength(packetLengthBytes->intValue());
    pk->setSrcAddr(myAddress);
    pk->setDestAddr(destAddress);

    pk->setPacketType(1);

    send(pk, "out");
}

void App::createChainLogMessage() {
    int destAddress = tempBlockID;

    char pkname[40];
    sprintf(pkname, "#%ld from-%d-to-%d chain log", pkCounter++, myAddress,
            destAddress);
    //EV << "generating packet " << pkname << endl;

    if (hasGUI())
        getParentModule()->bubble("Generating packet...");

    //Packet Creation
    Packet *pk = new Packet(pkname);
    pk->setByteLength(packetLengthBytes->intValue());
    pk->setSrcAddr(myAddress);
    pk->setDestAddr(destAddress);

    pk->setPacketType(2);

    //Add chain to the block

    if (trustChain.size() != 0) {
        pk->setUserBIDArraySize(trustChain.size());
        pk->setUserBSeqNumArraySize(trustChain.size());
        pk->setTransactionArraySize(trustChain.size());
        int i;
        for (i = 0; i < trustChain.size(); i++) {
            pk->setUserBID(i, trustChain[i].partnerId);
            pk->setUserBSeqNum(i, trustChain[i].partnerSeqNum);
            pk->setTransaction(i, trustChain[i].transactionValue);
        }
    } else {
        pk->setUserBIDArraySize(0);
        pk->setUserBSeqNumArraySize(0);
        pk->setTransactionArraySize(0);
    }

    send(pk, "out");
}

void App::createAckMessage() {
    int destAddress = tempBlockID;

    char pkname[40];
    sprintf(pkname, "#%ld from-%d-to-%d transaction ack", pkCounter++,
            myAddress, destAddress);

    if (hasGUI())
        getParentModule()->bubble("Generating packet...");

    //Packet Creation
    Packet *pk = new Packet(pkname);
    pk->setByteLength(packetLengthBytes->intValue());
    pk->setSrcAddr(myAddress);
    pk->setDestAddr(destAddress);

    pk->setPacketType(3);
    pk->setMyChainSeqNum(trustChain.size());

    send(pk, "out");
}

void App::verificationTransactionChain(Packet *pk) {
    if (pk->getUserBIDArraySize() != 0) {
        int i;
        for (i = 0; i < pk->getUserBIDArraySize(); i++) {
            EV << " User B array size: " << pk->getUserBIDArraySize()
                      << " get user: " << pk->getUserBID(i)
                      << " get user chain seq numb: " << pk->getUserBSeqNum(i)
                      << " get transaction: " << pk->getTransaction(i) << endl;
        }
    }
}

void App::logTransactionChain(Packet *pk) {
    if (pk->getUserBIDArraySize() != 0) {
        int i;
        for (i = 1; i < pk->getUserBIDArraySize(); i++) {
            LogDatabaseElement *element = new LogDatabaseElement(
                    pk->getSrcAddr(), i + 1, pk->getUserBID(i),
                    pk->getUserBSeqNum(i), pk->getTransaction(i));
            if (!isAlreadyPresentInDb(element)) {
                logDatabase.push_back(*element);
                EV << " **************new element" << endl;
            } else {
                EV << " ***************already present " << endl;
            }
        }
    }
}

bool App::isAlreadyPresentInDb(LogDatabaseElement *element) {
    int i;
    for (i = 0; i < logDatabase.size(); i++) {
        if (logDatabase[i].UserAId == element->UserAId
                && logDatabase[i].UserASeqNum == element->UserASeqNum
                && logDatabase[i].UserBId == element->UserBId
                && logDatabase[i].UserBSeqNum == element->UserBSeqNum) {
            return true;
        }
        if (logDatabase[i].UserAId == element->UserBId
                && logDatabase[i].UserASeqNum == element->UserBSeqNum
                && logDatabase[i].UserBId == element->UserAId
                && logDatabase[i].UserBSeqNum == element->UserASeqNum) {
            return true;
        }
    }
    return false;
}

void App::createDisseminationMessage() {
    //TODO
    int i;
    for (i = 0; i < logDatabase.size(); i++) {
        EV << " UserA " << logDatabase[i].UserAId << " UserADeqNum: "
                  << logDatabase[i].UserASeqNum << " UserB "
                  << logDatabase[i].UserBId << " UserBDeqNum: "
                  << logDatabase[i].UserBSeqNum << " transaction:"
                  << logDatabase[i].transactionValue << endl;
    }
}

int App::randomNodeAddressPicker() {
    int destAddress = -1;

    while (destAddress == -1 || destAddress == myAddress) {
        destAddress = destAddresses[intuniform(0, destAddresses.size() - 1)];
    }

    return destAddress;
}

void App::registerNewChainNode(int id, int seqNum, int value) {
    TrustChainElement *chainElement = new TrustChainElement(id, seqNum, value);
    trustChain.push_back(*chainElement);
    calculateChainValue();

    if (id != -1) {
        LogDatabaseElement *element = new LogDatabaseElement(myAddress,
                trustChain.size(), id, seqNum, value);
        logDatabase.push_back(*element);
    }
}

void App::calculateChainValue() {
    int localTotalChainValue = 0, i = 0;

    for (i = 0; i < trustChain.size(); i++) {
        localTotalChainValue = localTotalChainValue
                + trustChain[i].transactionValue;
    }
    chainTotalValue = localTotalChainValue;
}
