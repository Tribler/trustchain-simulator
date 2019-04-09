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

const int INITIAL_MONEY=10;


class TrustChainElement {
  public:
    int partnerId, transactionValue; // values limited to +- 2147483647

    TrustChainElement (int partnerId, int transactionValue ){
        this->partnerId = partnerId;
        this->transactionValue=transactionValue;
    }
};


class App : public cSimpleModule
{
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

    // signals
    simsignal_t endToEndDelaySignal;
    simsignal_t hopCountSignal;
    simsignal_t sourceAddressSignal;

    //TrustChain Storage
    std::vector<int> extraInfoArray;
    std::vector<TrustChainElement> trustChain;



  public:
    App();
    virtual ~App();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    //Utility functions
    virtual void threadFunction();
    virtual void createMessage();
    virtual void receiveMessage(cMessage *msg);
    virtual int randomNodeAddressPicker();
    virtual void calculateChainValue();

};

Define_Module(App);

App::App()
{
    generatePacket = nullptr;
    timerThread = nullptr;
}

App::~App()
{
    cancelAndDelete(generatePacket);
    cancelAndDelete(timerThread);
}

void App::initialize()
{
    myAddress = par("address");
    packetLengthBytes = &par("packetLength");
    sendIATime = &par("sendIaTime");  // volatile parameter
    pkCounter = 0;

    WATCH(pkCounter);
    WATCH(myAddress);
    WATCH(chainTotalValue);

    const char *destAddressesPar = par("destAddresses");
    cStringTokenizer tokenizer(destAddressesPar);
    const char *token;
    while ((token = tokenizer.nextToken()) != nullptr)
        destAddresses.push_back(atoi(token));

    if (destAddresses.size() == 0)
        throw cRuntimeError("At least one address must be specified in the destAddresses parameter!");

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

void App::handleMessage(cMessage *msg)
{
    if (msg == timerThread)
    {
        threadFunction();
    }
    else
    {
        receiveMessage (msg);
    }
}

void App::threadFunction(){

    createMessage();

    scheduleAt(simTime() + sendIATime->doubleValue(), timerThread);
}

void App::createMessage()
{
    //Just for Production Test
    if(myAddress != 0){
        return;
    }


    //Check if i have the money for the transaction
    calculateChainValue();
    if(chainTotalValue<=0){
        return;
    }

    //Calculate transaction value
    int transactionValue = intuniform(1, chainTotalValue);

    // Sending packet
    int destAddress = randomNodeAddressPicker();

    TrustChainElement *chainElement = new TrustChainElement(destAddress, -transactionValue);
    trustChain.push_back(*chainElement);
    calculateChainValue();

    char pkname[40];
    sprintf(pkname, "#%ld from-%d-to-%d $%d",  pkCounter++, myAddress, destAddress, transactionValue);
    //EV << "generating packet " << pkname << endl;

    if (hasGUI())
            getParentModule()->bubble("Generating packet...");

    //Packet Creation
    Packet *pk = new Packet(pkname);
    pk->setByteLength(packetLengthBytes->intValue());
    pk->setKind(intuniform(0, 7));
    pk->setSrcAddr(myAddress);
    pk->setDestAddr(destAddress);
    pk->setTransactionValue(transactionValue);

    send(pk, "out");
}

void App::receiveMessage(cMessage *msg){
    // Handle incoming packet
    Packet *pk = check_and_cast<Packet *>(msg);


    EV << "received packet " << pk->getName() << " after " << pk->getHopCount() << "hops" << endl;
    emit(endToEndDelaySignal, simTime() - pk->getCreationTime());
    emit(hopCountSignal, pk->getHopCount());
    emit(sourceAddressSignal, pk->getSrcAddr());

    TrustChainElement *chainElement = new TrustChainElement(pk->getSrcAddr(), pk->getTransactionValue());
    trustChain.push_back(*chainElement);
    calculateChainValue();


    if (trustChain.size() != 0){
            EV << "I'm "<< myAddress <<" I have in my trustChain mex form ";

            int i, totalValue=0;
            for(i=0; i<trustChain.size(); i++){
                EV << " "<<trustChain[i].partnerId << " with transaction value: "<<trustChain[i].transactionValue;
                totalValue = totalValue + trustChain[i].transactionValue;
            }

            EV << endl<< " and a total value of: "<< totalValue;
            EV << " "<< endl;
        }



    delete pk;

    if (hasGUI())
        getParentModule()->bubble("Arrived!");
}

int App::randomNodeAddressPicker(){

    int destAddress = -1;

    while(destAddress == -1 || destAddress == myAddress ){
        destAddress = destAddresses[intuniform(0, destAddresses.size()-1)];
    }

    return destAddress;
}

void App::calculateChainValue(){

    int localTotalChainValue = 0, i = 0;

    for(i=0; i<trustChain.size(); i++){
        localTotalChainValue = localTotalChainValue + trustChain[i].transactionValue;
    }

    chainTotalValue = localTotalChainValue;
}
