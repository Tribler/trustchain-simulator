#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <bits/stdc++.h>
#include <vector>
#include <cstdlib>
#include <omnetpp.h>
#include "Packet_m.h"

using namespace omnetpp;

class AnonymizerTrackingElement
{
public:
    int anonymizerId, status; //-1- available 0- request sent 1- contract accepted 2- positive reply

    AnonymizerTrackingElement(int anonymizerId, int status)
    {
        this->anonymizerId = anonymizerId;
        this->status = status;
    }
};

class TrustChainElement
{
public:
    int partnerId, partnerSeqNum, transactionValue; // values limited to +- 2147483647

    TrustChainElement(int partnerId, int partnerSeqNum, int transactionValue)
    {
        this->partnerId = partnerId;
        this->partnerSeqNum = partnerSeqNum;
        this->transactionValue = transactionValue;
    }
};

class LogDatabaseElement
{
public:
    int UserAId, UserASeqNum, UserBId, UserBSeqNum, transactionValue; // values limited to +- 2147483647

    LogDatabaseElement(int UserAId, int UserASeqNum, int UserBId, int UserBSeqNum, int transactionValue)
    {
        this->UserAId = UserAId;
        this->UserASeqNum = UserASeqNum;
        this->UserBId = UserBId;
        this->UserBSeqNum = UserBSeqNum;
        this->transactionValue = transactionValue;
    }
};

class SimulationTiming
{
public:
    int nodeId;
    simtime_t transactionTime, detectionTime;

    SimulationTiming(int nodeId, simtime_t transactionTime, simtime_t detectionTime)
    {
        this->nodeId = nodeId;
        this->transactionTime = transactionTime;
        this->detectionTime = detectionTime;
    }
};

class RandomDistinctPicker
{
public:
    int seedIncremental;
    int seedCore;
    int min, max;
    std::unordered_set<int> intSet;
    int resultProided;

    RandomDistinctPicker(int min, int max, int seedCore)
    {
        this->min = min;
        this->max = max;
        this->seedCore = seedCore;
        seedIncremental = 0;
        resultProided = 0;
    }

    int getRandomNumber()
    {
        if (resultProided >= (max - min + 1))
            return min;

        int posibleRandomNum = rand();
        while (intSet.find(posibleRandomNum) != intSet.end()) {
            posibleRandomNum = rand();
        }
        intSet.insert(posibleRandomNum);
        resultProided++;
        return posibleRandomNum;
    }
private:
    int rand()
    {
        seedIncremental++;
        std::mt19937 gen(seedIncremental + seedCore);
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen);
    }

};

static std::vector<SimulationTiming> simulationTiming; // This variable contains the evil nodes last transaction and detection timing

class App : public omnetpp::cSimpleModule
{
private:
    // configuration
    int myAddress;
    std::vector<int> destAddresses;
    cPar *sendIATime;
    cPar *sendIaTimeEvil;
    cPar *packetLengthBytes;

    // state
    //cMessage *generatePacket;
    cMessage *timerThread;
    long pkCounter;
    long chainTotalValue;
    bool amIEvil;
    std::vector<int> victimDestAddresses; //used to keep track of whom the evil node has attacked
    int totalEvilTransactions;
    std::vector<int> evilNodeIds;

    //Transactions related par
    int tempBlockID;
    int tempPartnerSeqNum;
    int tempBlockTransaction;

    //Signals
    simsignal_t endToEndDelaySignal;
    simsignal_t hopCountSignal;
    simsignal_t sourceAddressSignal;

    //TrustChain Storage
    std::vector<TrustChainElement> trustChain; // This variable contains the personal chain
    std::vector<LogDatabaseElement> logDatabase; // This variable contains the chain knowledge received by the network

    //Anonymizer Tracking
    std::vector<AnonymizerTrackingElement> anonymizersTracking;

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
    virtual void printInformation(int nodeId, int evilNodeId, int location);

    //Messaging
    virtual void createTransactionMessage();
    virtual void createChainRequestMessage(int destination, int target);
    virtual void createChainLogMessage();
    virtual void createAckMessage();
    virtual void createDisseminationMessage(int userXID, int userXSeqNum, int userYID, int userYSeqNum, int transactionValue);
    virtual void reDisseminateMessage(Packet *pk);
    virtual void createBusyMessage(int destAddress);
    virtual void contactAnonymizers();
    virtual void sendAnonymizerConfirmation(int destAddress);

    virtual void createDirectChannel(int nodeId);
    virtual void closeDirectChannel();

    //Ledger management
    virtual bool verificationTransactionChain(Packet *pk);
    virtual int verificationDissemination(Packet *pk); //return -1 if all good otherwise return the id of the evil node
    virtual void logTransactionChain(Packet *pk);
    virtual bool isAlreadyPresentInDb(LogDatabaseElement *element);
    virtual bool isNodeEvil();
    virtual bool itIsAlreadyBeenAttacked(int nodeId);

    //Data logging
    virtual void simulationRegisterTransactionTime(int idNode);
    virtual void simulationRegisterDetectionTime(int idNode);
    virtual void stopSimulation();

};
