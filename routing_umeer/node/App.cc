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
    int partnerId, transactionValue; // values limited to +- 2147483647

    TrustChainElement(int partnerId, int transactionValue) {
        this->partnerId = partnerId;
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
    cMessage *generatePacket;
    cMessage *timerThread;
    long pkCounter;
    long chainTotalValue;

    int waitingForAck; //0 - IDLE 1 - Transaction Sent (waiting for ack)
    int waitingForChainLog; //2 - Chain Request (waiting for chain)

    int tempBlockID;
    int tempBlockTransaction;

    // signals
    simsignal_t endToEndDelaySignal;
    simsignal_t hopCountSignal;
    simsignal_t sourceAddressSignal;

    //TrustChain Storage
    std::vector<TrustChainElement> trustChain;

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
    virtual void calculateChainValue();

    virtual void createTransactionMessage();
    virtual void createChainRequestMessage();
    virtual void createChainLogMessage();
    virtual void createAckMessage();
    virtual void createDisseminationMessage();

    virtual void verificationTransactionChain(Packet *pk);
    virtual void logTransactionChain(Packet *pk);
};

Define_Module(App);

App::App() {
    generatePacket = nullptr;
    timerThread = nullptr;
}

App::~App() {
    cancelAndDelete(generatePacket);
    cancelAndDelete(timerThread);
}

void App::initialize() {
    myAddress = par("address");
    packetLengthBytes = &par("packetLength");
    sendIATime = &par("sendIaTime");  // volatile parameter
    pkCounter = 0;

    tempBlockID = 0;
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
    TrustChainElement *chainElement = new TrustChainElement(-1, INITIAL_MONEY);
    trustChain.push_back(*chainElement);

    //Start the recursive thread
    timerThread = new cMessage("nextPacket");
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

    if (pk->getPacketType() == 0) { // Transaction Request Received
        tempBlockID = pk->getSrcAddr();
        tempBlockTransaction = pk->getTransactionValue();
        createChainRequestMessage();
        waitingForChainLog = 1;
    } else if (pk->getPacketType() == 1) { // Chain Request Received
        createChainLogMessage();
    } else if (pk->getPacketType() == 2) { // Chain Received
        // log / verification procedure here
        //TODO

        verificationTransactionChain(pk);
        logTransactionChain(pk);

        waitingForChainLog = 0;
        TrustChainElement *chainElement = new TrustChainElement(tempBlockID,
                tempBlockTransaction);
        trustChain.push_back(*chainElement);
        calculateChainValue();

        createAckMessage();
        createDisseminationMessage();
        tempBlockID = 0;
        tempBlockTransaction = 0;
    } else if (pk->getPacketType() == 3) { // Ack Received
        if (pk->getSrcAddr() == tempBlockID) {
            TrustChainElement *chainElement = new TrustChainElement(tempBlockID,
                    -tempBlockTransaction);
            trustChain.push_back(*chainElement);
            calculateChainValue();

            createDisseminationMessage();
            tempBlockID = 0;
            tempBlockTransaction = 0;
            waitingForAck = 0;
        }
    }

//    TrustChainElement *chainElement = new TrustChainElement(pk->getSrcAddr(), pk->getTransactionValue());
//    trustChain.push_back(*chainElement);
//    calculateChainValue();

//    if (trustChain.size() != 0){
//            EV << "I'm "<< myAddress <<" I have in my trustChain mex form ";
//
//            int i, totalValue=0;
//            for(i=0; i<trustChain.size(); i++){
//                EV << " "<<trustChain[i].partnerId << " with transaction value: "<<trustChain[i].transactionValue;
//                totalValue = totalValue + trustChain[i].transactionValue;
//            }
//
//            EV << endl<< " and a total value of: "<< totalValue;
//            EV << " "<< endl;
//        }

    delete pk;

    if (hasGUI())
        getParentModule()->bubble("Arrived!");
}
void App::verificationTransactionChain(Packet *pk) {
    if (pk->getUserBArraySize() != 0) {
               int i;
               for (i = 0; i < pk->getUserBArraySize(); i++) {
                   EV << " User B array size: " << pk->getUserBArraySize()
                             << " get user: " << pk->getUserB(i)
                             << " get transaction: " << pk->getTransaction(i)
                             << endl;
               }
           }
    //TODO
}
void App::logTransactionChain(Packet *pk) {

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
        pk->setUserBArraySize(trustChain.size());
        pk->setTransactionArraySize(trustChain.size());
        int i;
        for (i = 0; i < trustChain.size(); i++) {
            pk->setUserB(i, trustChain[i].partnerId);
            pk->setTransaction(i, trustChain[i].transactionValue);
        }
    } else {
        pk->setUserBArraySize(0);
        pk->setTransactionArraySize(0);
    }

    send(pk, "out");
}

void App::createAckMessage() {

    int destAddress = tempBlockID;

    char pkname[40];
    sprintf(pkname, "#%ld from-%d-to-%d transaction ack", pkCounter++,
            myAddress, destAddress);
    //EV << "generating packet " << pkname << endl;

    if (hasGUI())
        getParentModule()->bubble("Generating packet...");

    //Packet Creation
    Packet *pk = new Packet(pkname);
    pk->setByteLength(packetLengthBytes->intValue());
    pk->setSrcAddr(myAddress);
    pk->setDestAddr(destAddress);

    pk->setPacketType(3);

    send(pk, "out");
}

void App::createDisseminationMessage() {
    //TODO

}

int App::randomNodeAddressPicker() {

    int destAddress = -1;

    while (destAddress == -1 || destAddress == myAddress) {
        destAddress = destAddresses[intuniform(0, destAddresses.size() - 1)];
    }

    return destAddress;
}

void App::calculateChainValue() {

    int localTotalChainValue = 0, i = 0;

    for (i = 0; i < trustChain.size(); i++) {
        localTotalChainValue = localTotalChainValue
                + trustChain[i].transactionValue;
    }

    chainTotalValue = localTotalChainValue;
}
