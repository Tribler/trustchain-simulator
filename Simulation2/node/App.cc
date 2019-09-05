//TODO LIST
// - optimize evil node so that do transactions with node far
// - refactoring: implement enum to mark type of packets

#include "App.h"
#include "Routing.h"

using namespace omnetpp;

Define_Module(App);

App::App()
{
    timerInitTransaction = nullptr;
    timerTransactionTimeout = nullptr;
    timerAnonymusAuditingTimeout = nullptr;
    timerAnonymiserDissemination = nullptr;
}
App::~App()
{
    cancelAndDelete(timerInitTransaction);
    cancelAndDelete(timerTransactionTimeout);
    cancelAndDelete(timerAnonymusAuditingTimeout);
    cancelAndDelete(timerAnonymiserDissemination);
}

void App::initialize()
{
    myAddress = par("address");
    packetLengthBytes = &par("packetLength");
    sendIATime = &par("sendIaTime");
    anonymusAuditingTimeoutTime = &par("anonymusAuditingTimeoutTime");
    sendIaTimeEvil = &par("sendIaTimeEvil");
    pkCounter = 0;

    tempBlockID = -1;
    tempPartnerSeqNum = 0;
    tempBlockTransaction = 0;
    transactionStage = 0;

    timerInitTransaction = new cMessage("InitNewTransaction");
    timerTransactionTimeout = new cMessage("TransactionTimeout");
    timerAnonymusAuditingTimeout = new cMessage("AnonymusAuditingTimeout");
    timerAnonymiserDissemination = new cMessage("TimerAnonymiserDissemination");

    //Information To Log
    WATCH(pkCounter);
    WATCH(myAddress);
    WATCH(chainTotalValue);

    //Neighbors definition
    for (int i = 0; i < (int) par("totalNodes"); i++) {
        destAddresses.push_back(i);
    }

    if (destAddresses.size() == 0)
        throw cRuntimeError("At least one address must be specified in the destAddresses parameter!");

    //TrustChain initialization
    registerNewChainNode(-1, -1, (int) par("initialMoney"));

    //Evil node marking
    const char *evilNodeIdsPar = par("evilNodeIds");
    cStringTokenizer tokenizer(evilNodeIdsPar);
    const char *token;
    while ((token = tokenizer.nextToken()) != nullptr)
        evilNodeIds.push_back(atoi(token));
    if (evilNodeIds.size() == 0) {
        evilNodeIds.push_back((int) getParentModule()->getParentModule()->par("evilNodeIdSingle"));
    }

    amIEvil = false;
    int i = 0;
    for (i = 0; i < evilNodeIds.size(); i++) {
        if (myAddress == evilNodeIds[i]) {
            amIEvil = true;
        }
    }
    totalEvilTransactions = 0;

    //Start the recursive thread
    scheduleAt(simTime() + par("sendIaTimeInit").doubleValue(), timerInitTransaction);
    scheduleAt(simTime() + par("anonymizerDisseminationTimeInit").doubleValue(), timerAnonymiserDissemination);

    endToEndDelaySignal = registerSignal("endToEndDelay");
    hopCountSignal = registerSignal("hopCount");
    sourceAddressSignal = registerSignal("sourceAddress");
}

void App::handleMessage(cMessage *msg)
{
    if (msg == timerInitTransaction) {
        initiateNewTransaction();
    }
    else if (msg == timerTransactionTimeout) {
        transactionTimeout();
    }
    else if (msg == timerAnonymusAuditingTimeout) {
        anonymusAuditingTimeout();
    }
    else if (msg == timerAnonymiserDissemination) {
        disseminateMeAsAnonymiser();
    }
    else {
        receiveMessage(msg);
    }
}
void App::receiveMessage(cMessage *msg)
{
    if (hasGUI())
        getParentModule()->bubble("Arrived!");
    Packet *pk = check_and_cast<Packet *>(msg);

    emit(endToEndDelaySignal, simTime() - pk->getCreationTime());
    emit(hopCountSignal, pk->getHopCount());
    emit(sourceAddressSignal, pk->getSrcAddr());

    switch (pk->getPacketType()) {
        case 0: { // Transaction Request Received

            if (tempBlockID != -1 || transactionStage != 0) { //currently in another transaction
                createBusyMessage(pk->getSrcAddr());
                break;
            }

            calculateChainValue();
            if (isNodeEvil() && chainTotalValue != 0) { //ignore all the incoming transactions
                createBusyMessage(pk->getSrcAddr());
                break;
            }

            tempBlockID = pk->getSrcAddr();
            tempPartnerSeqNum = pk->getMyChainSeqNum();
            tempBlockTransaction = pk->getTransactionValue();

            transactionStage = 1;
            contactAnonymizers();

            break;
        }
        case 1: { // Chain Request Received
            if (pk->getUserXID() == myAddress) {
                createChainLogMessage(pk->getSrcAddr());
            }
            else /*TEST if (myAddress != 1)*/{ // this is an anonymization request
                sendAnonymizerConfirmation(pk->getSrcAddr());
                anonymizerWaitList.push_back(*new AnonymizerWaitListElement(pk->getSrcAddr(), pk->getUserXID()));
                createChainRequestMessage(pk->getUserXID(), pk->getUserXID());
            }

            break;
        }
        case 11: { // Anonymizer Confirmation Received
            markAnonymizerNodeAsActive(pk->getSrcAddr());
            break;
        }
        case 2: { // Partner Chain Received

            if (pk->getSrcAddr() == pk->getUserXID()) {
                switch (verificationTransactionChain(pk)) {
                    case -1: {
                        printInformation(myAddress, pk->getSrcAddr(), 0);
                        simulationRegisterDetectionTime(pk->getSrcAddr());
                        stopSimulation();
                        break;
                    }
                    case 0: {
                        //ask again one time
                        if (isFirstTimeMissingTransaction(pk->getSrcAddr())) {
                            registerNodeMissingTransaction(pk->getSrcAddr());
                            createChainRequestMessage(pk->getSrcAddr(), pk->getSrcAddr());
                        }
                        else {
                            removeNodeMissingTransaction(pk->getSrcAddr());
                            EV << "second tentative failed" << endl;
                            printInformation(myAddress, pk->getSrcAddr(), 0);
                            simulationRegisterDetectionTime(pk->getSrcAddr());
                            stopSimulation();
                        }
                        break;
                    }
                    case 1: {
                        removeNodeMissingTransaction(pk->getSrcAddr());
                        logTransactionChain(pk);
                        int positionIndex = getIndexFromAnonymizerWaitList(pk->getSrcAddr());
                        if (positionIndex != -1) { //I'm anonymizer receiving a reply from a target
                            forwardReceivedChainToRequester(anonymizerWaitList[positionIndex].requesterId, pk);
                            anonymizerWaitList.erase(anonymizerWaitList.begin() + positionIndex);
                        }
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }
            else if (pk->getUserXID() == tempBlockID && transactionStage == 1 && isAnAuditedAnonymizer(pk->getSrcAddr())) {
                //I'm a node receiving reply from anonymizer (pk->getSrcAddr() != pk->getUserXID())
                if (verificationTransactionChain(pk) == 1 && pk->getTransactionArraySize() < tempPartnerSeqNum) {
                    logTransactionChain(pk);
                    updateDisseminationNodeAddresses(pk);
                    logAnonymiserReply(pk->getSrcAddr());

                    //have all anonymizer replied?
                    bool allDone = true;
                    for (int i = 0; i < anonymizersTracking.size(); i++) {
                        if (anonymizersTracking[i].status != 2)
                            allDone = false;
                    }
                    if (allDone == true) {
                        cancelEvent(timerAnonymusAuditingTimeout);
                        transactionStage = 2;
                        registerNewChainNode(tempBlockID, tempPartnerSeqNum, tempBlockTransaction);
                        createAckMessage();
                        disseminationAuditing();
                        tempBlockID = -1;
                        tempPartnerSeqNum = 0;
                        tempBlockTransaction = 0;
                    }
                }
                else {
                    EV << "anonymizer are replying wrong" << endl;
                    printInformation(myAddress, tempBlockID, 0);
                    simulationRegisterDetectionTime(tempBlockID);
                    stopSimulation();
                }
            }

            break;
        }
        case 3: { // Ack Received
            if (pk->getSrcAddr() == tempBlockID) {
                EV << "im node: " << myAddress << " just received ack" << endl;
                cancelEvent(timerTransactionTimeout);

                if (!isNodeEvil()) {
                    registerNewChainNode(tempBlockID, pk->getMyChainSeqNum(), -tempBlockTransaction);
                }
                else {
                    char text[128];
                    sprintf(text, "Evil node: #%d - completed transaction with #%d of value $%d Time: %s s", myAddress, tempBlockID, tempBlockTransaction, SIMTIME_STR(simTime()));
                    EV << text << endl;
                    getSimulation()->getActiveEnvir()->alert(text);
                    totalEvilTransactions = totalEvilTransactions + 1;
                    victimDestAddresses.push_back(tempBlockID);
                }

                tempBlockID = -1;
                tempBlockTransaction = 0;
            }
            break;
        }
        case 4: { // Dissemination Received
            int result = verificationDissemination(pk);
            if (result == -1) {
                LogDatabaseElement *element = new LogDatabaseElement(pk->getUserXID(), pk->getUserXSeqNum(), pk->getUserYID(), pk->getUserYSeqNum(), pk->getTransactionValue());
                if (!isAlreadyPresentInDb(element)) {
                    logDatabase.push_back(*element);
                    //reDisseminateMessage(pk);
                }
            }
            else {
                printInformation(myAddress, result, 2);
                simulationRegisterDetectionTime(result);
                stopSimulation();
            }
            break;
        }
        case 5: { // Busy Received
            if (pk->getSrcAddr() == tempBlockID) {
                cancelEvent(timerTransactionTimeout);
                tempBlockID = -1;
                tempBlockTransaction = 0;
            }
            break;
        }
        case 6: { // Anonymization Received
            if (pk->getUserXID() != myAddress) {
                char key[128];
                sprintf(key, "%d %f", pk->getUserXID(), pk->getTime());
                if (!haveISeenThisAnonymizerDisseminationBefore(key)) {
                    updateAnonymizerNodeList(pk->getUserXID());
                    anonyDisseminationMessageSet.insert(key);
                    reDisseminateMessage(pk);
                }
            }
            break;
        }
        default: {
            break;
        }
    }
    delete pk;

}

//TIMEOUT
void App::transactionTimeout()
{
    char text[128];
    sprintf(text, "im node: #%d life is good and i hate %d for not concluding the transaction Time: %s s", myAddress, tempBlockID, SIMTIME_STR(simTime()));
    getSimulation()->getActiveEnvir()->alert(text);
    tempBlockID = -1;
    tempBlockTransaction = 0;
}
void App::anonymusAuditingTimeout()
{
    if (transactionStage == 1) { //still waiting for anonymizer
        //verify status of collected anonymus chain
        int nodesThatAcceptedToAnonymise = 0;
        int positiveReply = 0;
        for (int i = 0; i < anonymizersTracking.size(); i++) {
            if (anonymizersTracking[i].status == 1 || anonymizersTracking[i].status == 2)
                nodesThatAcceptedToAnonymise++;
            if (anonymizersTracking[i].status == 2)
                positiveReply++;
        }

        //TODO: positiveReply is better to be >1
        if (positiveReply > 0 && positiveReply >= nodesThatAcceptedToAnonymise / 2) {
            transactionStage = 2;
            registerNewChainNode(tempBlockID, tempPartnerSeqNum, tempBlockTransaction);
            createAckMessage();
            disseminationAuditing();
            tempBlockID = -1;
            tempBlockTransaction = 0;
            tempPartnerSeqNum = 0;
        }
        else {
            //transaction not good (NOT POSITVE OR ENOUGH REPLY)
            transactionStage = 0;
            tempBlockID = -1;
            tempBlockTransaction = 0;
            tempPartnerSeqNum = 0;
        }
    }
}

//TRANSACTION INIT
void App::initiateNewTransaction()
{
    if (tempBlockID == -1 && transactionStage == 0) { //No transaction are pending
        calculateChainValue();
        if (chainTotalValue > 0) {
            if (isNodeEvil() && totalEvilTransactions >= (int) par("evilNumberOfTransaction")) {
                return;
            }
            else {
                createTransactionMessage();
                scheduleAt(simTime() + par("transactionTimeoutTime").doubleValue(), timerTransactionTimeout);
            }
        }
    }

    if (isNodeEvil()) {
        sendIATime = sendIaTimeEvil;
    }
    scheduleAt(simTime() + sendIATime->doubleValue(), timerInitTransaction);
}
void App::createBusyMessage(int destAddress)
{
    char pkname[40];
    sprintf(pkname, "#%ld from-%d-to-%d busy", pkCounter++, myAddress, destAddress);

    if (hasGUI())
        getParentModule()->bubble("Generating packet...");

//Packet Creation
    Packet *pk = new Packet(pkname);
    pk->setByteLength(packetLengthBytes->intValue());
    pk->setSrcAddr(myAddress);
    pk->setDestAddr(destAddress);

    pk->setPacketType(5);

    send(pk, "out");
}
void App::createTransactionMessage()
{
//Calculate transaction value
    int transactionValue = intuniform(1, chainTotalValue);

//Decide destination
    int destAddress = randomNodeAddressPicker();
    if (isNodeEvil()) {
        while (itIsAlreadyBeenAttacked(destAddress)) {
            destAddress = randomNodeAddressPicker();
        }
        simulationRegisterTransactionTime(myAddress);
    }

    tempBlockID = destAddress;
    tempBlockTransaction = transactionValue;

    if (isNodeEvil()) {
        char text[128];
        sprintf(text, "Evil node: #%d - starting transaction with #%d of value $%d Time: %s s", myAddress, tempBlockID, tempBlockTransaction, SIMTIME_STR(simTime()));
        EV << text << endl;
        getSimulation()->getActiveEnvir()->alert(text);
    }

    char pkname[40];
    sprintf(pkname, "#%ld from-%d-to-%d $%d", pkCounter++, myAddress, destAddress, transactionValue);

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

//AUDITING SYSTEM
void App::contactAnonymizers()
{
    //Purge nodes that where offline last time
    for (int i = 0; i < anonymizersTracking.size(); i++) {
        if (anonymizersTracking[i].status == 0) { // it did not replied
            for (int j = 0; j < anonymizerList.size(); j++) {
                if (anonymizerList[j].nodeId == anonymizersTracking[i].anonymizerId) {
                    anonymizerList.erase(anonymizerList.begin() + j);
                }
            }

        }
    }

    //Purge nodes that are not disseminating since a while
    for (int i = 0; i < anonymizerList.size(); i++) {
        if ((SIMTIME_DBL(simTime()) - anonymizerList[i].time) > (double) par("anonymizerLifeTime")) {
            anonymizerList.erase(anonymizerList.begin() + i);
            if (i > 0)
                i--;
        }
    }

    int numberOfAnonymizer = (int) par("numberOfAnonymizer");
    int numberOfNodes = (int) par("totalNodes");
    if (numberOfAnonymizer > numberOfNodes - 2) {
        numberOfAnonymizer = numberOfNodes - 2;
    }

    disseminationNodeAddresses.clear();
    anonymizersTracking.clear();
    RandomDistinctPicker *rand = new RandomDistinctPicker(0, numberOfNodes - 1, par("randomSeed"));

    for (int i = 0; i < numberOfAnonymizer; i++) {
        int destAddresses = rand->getRandomNumber();
        if (destAddresses == myAddress || destAddresses == tempBlockID) {
            destAddresses = rand->getRandomNumber();
            if (destAddresses == myAddress || destAddresses == tempBlockID) {

                destAddresses = rand->getRandomNumber(); // worse case can mistake two time in row
            }
        }
        anonymizersTracking.push_back(*new AnonymizerTrackingElement(destAddresses, 0));
        createChainRequestMessage(destAddresses, tempBlockID);
        //TODO: wait a random time
    }

    delete rand;

    scheduleAt(simTime() + anonymusAuditingTimeoutTime->doubleValue(), timerAnonymusAuditingTimeout);
}
void App::sendAnonymizerConfirmation(int destAddress)
{
    char pkname[40];
    sprintf(pkname, "#%ld from-%d-to-%d anonymiser confirmation", pkCounter++, myAddress, destAddress);

    if (hasGUI())
        getParentModule()->bubble("Generating packet...");

//Packet Creation
    Packet *pk = new Packet(pkname);
    pk->setByteLength(packetLengthBytes->intValue());
    pk->setSrcAddr(myAddress);
    pk->setDestAddr(destAddress);

    pk->setPacketType(11);

    send(pk, "out");
}
void App::markAnonymizerNodeAsActive(int nodeAddress)
{
    for (int i = 0; i < anonymizersTracking.size(); i++) {
        if (anonymizersTracking[i].anonymizerId == nodeAddress)
            anonymizersTracking[i].status = 1;
    }
}
void App::createChainRequestMessage(int destination, int target)
{
    int destAddress = destination;

    char pkname[40];
    if (destination != target) {
        sprintf(pkname, "#%ld from-%d-to-%d chain request to anonymizer", pkCounter++, myAddress, destAddress);
    }
    else {
        sprintf(pkname, "#%ld from-%d-to-%d chain request", pkCounter++, myAddress, destAddress);
    }

    if (hasGUI())
        getParentModule()->bubble("Generating packet...");

//Packet Creation
    Packet *pk = new Packet(pkname);
    pk->setByteLength(packetLengthBytes->intValue());
    pk->setSrcAddr(myAddress);
    pk->setDestAddr(destAddress);

    pk->setPacketType(1);
    pk->setUserXID(target);

    send(pk, "out");
}
void App::createChainLogMessage(int destAddress)
{
    char pkname[40];
    sprintf(pkname, "#%ld from-%d-to-%d chain log", pkCounter++, myAddress, destAddress);

    if (hasGUI())
        getParentModule()->bubble("Generating packet...");

//Packet Creation
    Packet *pk = new Packet(pkname);
    pk->setByteLength(packetLengthBytes->intValue() + ((trustChain.size() - 1) * 10));
    pk->setSrcAddr(myAddress);
    pk->setDestAddr(destAddress);

    pk->setPacketType(2);
    pk->setUserXID(myAddress);

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
    }
    else {
        pk->setUserBIDArraySize(0);
        pk->setUserBSeqNumArraySize(0);
        pk->setTransactionArraySize(0);
    }

    send(pk, "out");
}
int App::getIndexFromAnonymizerWaitList(int targetNodeAddress)
{
    for (int i = 0; i < anonymizerWaitList.size(); i++) {
        if (anonymizerWaitList[i].targetId == targetNodeAddress) {
            return i;
        }
    }
    return -1;
}
void App::forwardReceivedChainToRequester(int requesterAddress, Packet *pk)
{
    Packet *copy = (Packet *) pk->dup();

    char pkname[40];
    sprintf(pkname, "#%ld from-%d-to-%d chain log from anonmizer", pkCounter++, myAddress, requesterAddress);
    if (hasGUI())
        getParentModule()->bubble("Generating packet...");
    copy->setName(pkname);
    copy->setSrcAddr(myAddress);
    copy->setDestAddr(requesterAddress);

    send(copy, "out");
}
bool App::isAnAuditedAnonymizer(int anonymizerNodeAddress)
{
    for (int i = 0; i < anonymizersTracking.size(); i++) {
        {
            if (anonymizersTracking[i].anonymizerId == anonymizerNodeAddress && anonymizersTracking[i].status == 1)
                return true;
        }
    }
    return false;
}
void App::logAnonymiserReply(int anonymizerNodeAddress)
{
    for (int i = 0; i < anonymizersTracking.size(); i++) {
        {
            if (anonymizersTracking[i].anonymizerId == anonymizerNodeAddress)
                anonymizersTracking[i].status = 2;
        }
    }
}
void App::updateDisseminationNodeAddresses(Packet *pk)
{
    for (int i = 0; i < pk->getUserBIDArraySize(); i++) {
        if (pk->getUserBID(i) != -1) { //genesis transaction
            bool alreadyPresent = false;
            for (int j = 0; j < disseminationNodeAddresses.size(); j++) {
                if (disseminationNodeAddresses[j] == pk->getUserBID(i))
                    alreadyPresent = true;
            }
            if (!alreadyPresent)
                disseminationNodeAddresses.push_back(pk->getUserBID(i));
        }
    }
}

int App::verificationTransactionChain(Packet *pk)
{
    bool result = 1;

//check if the your local information can be compared to the received informations
// 1 - same sequence number and user ids number have the same transaction value (chain block replaced with another one)
// 2 - All the local info regarding a user can be found in the chain (not hiding something)
    int i;
    for (i = 0; i < logDatabase.size(); i++) {

        if (logDatabase[i].UserAId == pk->getUserXID()) {
            int index = logDatabase[i].UserASeqNum - 1;
            if (pk->getUserBIDArraySize() > index) {
                if (pk->getUserBID(index) == logDatabase[i].UserBId && pk->getUserBSeqNum(index) == logDatabase[i].UserBSeqNum && pk->getTransaction(index) == logDatabase[i].transactionValue) {
                    //all good!
                }
                else {
                    EV << "****The local transaction is not matching with the data provided" << endl;
                    return -1;
                }
            }
            else {
                EV << "****There aren't enough transactions in the received chain" << endl;
                return 0;
            }
        }
        else if (logDatabase[i].UserBId == pk->getUserXID()) {
            int index = logDatabase[i].UserBSeqNum - 1;
            if (pk->getUserBIDArraySize() > index) {
                if (pk->getUserBID(index) == logDatabase[i].UserAId && pk->getUserBSeqNum(index) == logDatabase[i].UserASeqNum && pk->getTransaction(index) == -logDatabase[i].transactionValue) {
                    //all good!
                }
                else {
                    EV << logDatabase[i].UserBSeqNum << " " << pk->getUserBIDArraySize() << endl;
                    EV << "****The local transaction is not matching with the data provided B" << endl;
                    EV << pk->getUserBID(index) << " " << logDatabase[i].UserAId << endl;
                    EV << pk->getUserBSeqNum(index) << " " << logDatabase[i].UserASeqNum << endl;
                    EV << pk->getTransaction(index) << " " << -logDatabase[i].transactionValue << endl;
                    return -1;
                }
            }
            else {
                EV << "****There aren't enough transactions in the received chain B" << endl;
                return 0;
            }
        }
    }

// 3 - Check people with which the other user had transactions with and verify those too
    for (int j = 0; j < pk->getTransactionArraySize(); j++) {
        for (i = 0; i < logDatabase.size(); i++) {
            if (logDatabase[i].UserAId == pk->getUserBID(j) && logDatabase[i].UserASeqNum == pk->getUserBSeqNum(j)) {
                if (!(logDatabase[i].UserBId == pk->getUserXID() && logDatabase[i].UserBSeqNum == (j + 1) && (logDatabase[i].transactionValue == pk->getTransaction(j) || -logDatabase[i].transactionValue == pk->getTransaction(j)))) {
                    simulationRegisterDetectionTime(pk->getUserBID(j));
                    printInformation(myAddress, pk->getUserBID(j), 0);
                    result = -1;
                }
            }
            else if (logDatabase[i].UserBId == pk->getUserBID(j) && logDatabase[i].UserBSeqNum == pk->getUserBSeqNum(j)) {
                if (!(logDatabase[i].UserAId == pk->getUserXID() && logDatabase[i].UserASeqNum == (j + 1) && (logDatabase[i].transactionValue == pk->getTransaction(j) || -logDatabase[i].transactionValue == pk->getTransaction(j)))) {
                    simulationRegisterDetectionTime(pk->getUserBID(j));
                    printInformation(myAddress, pk->getUserBID(j), 0);
                    result = -1;
                }
            }
        }
    }

    return result;
}
void App::printInformation(int nodeId, int evilNodeId, int location)
{
    char text[128];
    if (location == 0) {
        sprintf(text, "Node: #%d - detected double spending during chain verification done by: #%d Time: %s s", nodeId, evilNodeId, SIMTIME_STR(simTime()));
    }
    else {
        sprintf(text, "Node: #%d - detected double spending during dissemination verification done by: #%d Time: %s s", nodeId, evilNodeId, SIMTIME_STR(simTime()));
    }
    EV << text << endl;
    getSimulation()->getActiveEnvir()->alert(text);
}
int App::verificationDissemination(Packet *pk)
{
    int i;
    for (i = 0; i < logDatabase.size(); i++) {

        if (logDatabase[i].UserAId == pk->getUserXID() && logDatabase[i].UserASeqNum == pk->getUserXSeqNum()) {
            if (logDatabase[i].UserBId == pk->getUserYID() && logDatabase[i].UserBSeqNum == pk->getUserYSeqNum() && abs(logDatabase[i].transactionValue) == abs(pk->getTransactionValue())) {
                return -1;
            }
            else {
                return logDatabase[i].UserAId;
            }
        }

        if (logDatabase[i].UserAId == pk->getUserYID() && logDatabase[i].UserASeqNum == pk->getUserYSeqNum()) {
            if (logDatabase[i].UserBId == pk->getUserXID() && logDatabase[i].UserBSeqNum == pk->getUserXSeqNum() && abs(logDatabase[i].transactionValue) == abs(pk->getTransactionValue())) {
                return -1;
            }
            else {
                return logDatabase[i].UserAId;
            }
        }

        if (logDatabase[i].UserBId == pk->getUserXID() && logDatabase[i].UserBSeqNum == pk->getUserXSeqNum()) {
            if (logDatabase[i].UserAId == pk->getUserYID() && logDatabase[i].UserASeqNum == pk->getUserYSeqNum() && abs(logDatabase[i].transactionValue) == abs(pk->getTransactionValue())) {
                return -1;
            }
            else {
                return logDatabase[i].UserBId;
            }
        }

        if (logDatabase[i].UserBId == pk->getUserYID() && logDatabase[i].UserBSeqNum == pk->getUserYSeqNum()) {
            if (logDatabase[i].UserAId == pk->getUserXID() && logDatabase[i].UserASeqNum == pk->getUserXSeqNum() && abs(logDatabase[i].transactionValue) == abs(pk->getTransactionValue())) {
                return -1;
            }
            else {
                return logDatabase[i].UserBId;
            }
        }
    }

    return -1;
}
bool App::isFirstTimeMissingTransaction(int nodeId)
{
    for (int i = 0; i < nodeWithMissingTransaction.size(); i++) {
        if (nodeWithMissingTransaction[i] == nodeId)
            return false;
    }
    return true;
}
void App::registerNodeMissingTransaction(int nodeId)
{
    nodeWithMissingTransaction.push_back(nodeId);
}
void App::removeNodeMissingTransaction(int nodeId)
{
    for (int i = 0; i < nodeWithMissingTransaction.size(); i++) {
        if (nodeWithMissingTransaction[i] == nodeId)
            nodeWithMissingTransaction.erase(nodeWithMissingTransaction.begin() + i);
    }
}

void App::logTransactionChain(Packet *pk)
{
    if (pk->getUserBIDArraySize() != 0) {
        int i;
        for (i = 1; i < pk->getUserBIDArraySize(); i++) {
            LogDatabaseElement *element = new LogDatabaseElement(pk->getUserXID(), i + 1, pk->getUserBID(i), pk->getUserBSeqNum(i), pk->getTransaction(i));
            if (!isAlreadyPresentInDb(element)) {
                logDatabase.push_back(*element);
                //EV << " **************new element" << endl;
            }
            else {
                //EV << " ***************already present " << endl;
            }
        }
    }
}
bool App::isAlreadyPresentInDb(LogDatabaseElement *element)
{
    int i;
    for (i = 0; i < logDatabase.size(); i++) {
        if (logDatabase[i].UserAId == element->UserAId && logDatabase[i].UserASeqNum == element->UserASeqNum && logDatabase[i].UserBId == element->UserBId && logDatabase[i].UserBSeqNum == element->UserBSeqNum) {
            return true;
        }
        if (logDatabase[i].UserAId == element->UserBId && logDatabase[i].UserASeqNum == element->UserBSeqNum && logDatabase[i].UserBId == element->UserAId && logDatabase[i].UserBSeqNum == element->UserASeqNum) {
            return true;
        }
    }
    return false;
}

//CREATE TRANSACTION
void App::createAckMessage()
{
    int destAddress = tempBlockID;

    char pkname[40];
    sprintf(pkname, "#%ld from-%d-to-%d transaction ack", pkCounter++, myAddress, destAddress);

    if (hasGUI())
        getParentModule()->bubble("Generating packet...");

//Packet Creation
    Packet *pk = new Packet(pkname);
    pk->setByteLength(packetLengthBytes->intValue());
    pk->setSrcAddr(myAddress);
    pk->setDestAddr(destAddress);

    pk->setPacketType(3);
    pk->setMyChainSeqNum(trustChain.size());

    createDirectChannel(tempBlockID);
    send(pk, "direct");
    closeDirectChannel();
}
void App::registerNewChainNode(int id, int seqNum, int value)
{
    TrustChainElement *chainElement = new TrustChainElement(id, seqNum, value);
    trustChain.push_back(*chainElement);
    calculateChainValue();

    if (id != -1) {
        LogDatabaseElement *element = new LogDatabaseElement(myAddress, trustChain.size(), id, seqNum, value);
        logDatabase.push_back(*element);
    }
}
void App::calculateChainValue()
{
    int localTotalChainValue = 0, i = 0;

    for (i = 0; i < trustChain.size(); i++) {
        localTotalChainValue = localTotalChainValue + trustChain[i].transactionValue;
    }
    chainTotalValue = localTotalChainValue;
}

//DISSEMINATION
void App::disseminationAuditing()
{
    RandomDistinctPicker *rand = new RandomDistinctPicker(0, disseminationNodeAddresses.size() - 1, par("randomSeed"));

    for (int i = 0; i < disseminationNodeAddresses.size(); i++) {
        int pickedNodeId = disseminationNodeAddresses[rand->getRandomNumber()];
        createChainRequestMessage(pickedNodeId, pickedNodeId);
        sendMyLastTransactionTo(pickedNodeId);
    }

    delete rand;

    transactionStage = 0;
}
void App::sendMyLastTransactionTo(int destinationAddress)
{
    char pkname[40];
    sprintf(pkname, "#%ld from-%d-to-%d dissemination", pkCounter++, myAddress, destinationAddress);

    if (hasGUI())
        getParentModule()->bubble("Generating packet...");

    //Packet Creation
    Packet *pk = new Packet(pkname);
    pk->setByteLength(packetLengthBytes->intValue());
    pk->setSrcAddr(myAddress);
    pk->setDestAddr(destinationAddress);

    pk->setPacketType(4);

    pk->setUserXID(myAddress);
    pk->setUserXSeqNum(trustChain.size());
    pk->setUserYID(tempBlockID);
    pk->setUserYSeqNum(tempPartnerSeqNum);
    pk->setTransactionValue(tempBlockTransaction);

    send(pk, "out");
}
void App::createDisseminationMessage(int userXID, int userXSeqNum, int userYID, int userYSeqNum, int transactionValue)
{
    cModule *mod = getParentModule()->getSubmodule("routing");
    Routing *myRouting = check_and_cast<Routing*>(mod);
    std::vector<int> neighbourNodeAddresses = myRouting->neighbourNodeAddresses;
    RandomDistinctPicker *rand = new RandomDistinctPicker(0, neighbourNodeAddresses.size() - 1, par("randomSeed"));

    for (int i = 0; i < neighbourNodeAddresses.size(); i++) {
// User selection
        int destAddress = neighbourNodeAddresses[rand->getRandomNumber()];

        char pkname[40];
        sprintf(pkname, "#%ld from-%d-to-%d dissemination", pkCounter++, myAddress, destAddress);

        if (hasGUI())
            getParentModule()->bubble("Generating packet...");

//Packet Creation
        Packet *pk = new Packet(pkname);
        pk->setByteLength(packetLengthBytes->intValue());
        pk->setSrcAddr(myAddress);
        pk->setDestAddr(destAddress);

        pk->setPacketType(4);

        pk->setUserXID(userXID);
        pk->setUserXSeqNum(userXSeqNum);
        pk->setUserYID(userYID);
        pk->setUserYSeqNum(userYSeqNum);
        pk->setTransactionValue(transactionValue);

        send(pk, "out");
    }
    delete rand;
}

//ANONYMIZER DISSEMINATION
void App::disseminateMeAsAnonymiser()
{
    cModule *mod = getParentModule()->getSubmodule("routing");
    Routing *myRouting = check_and_cast<Routing*>(mod);
    std::vector<int> neighbourNodeAddresses = myRouting->neighbourNodeAddresses;
    RandomDistinctPicker *rand = new RandomDistinctPicker(0, neighbourNodeAddresses.size() - 1, par("randomSeed"));

    for (int i = 0; i < neighbourNodeAddresses.size(); i++) {
        // User selection
        int destAddress = neighbourNodeAddresses[rand->getRandomNumber()];

        char pkname[40];
        sprintf(pkname, "#%ld from-%d-to-%d anonymizer dissemination", pkCounter++, myAddress, destAddress);

        if (hasGUI())
            getParentModule()->bubble("Generating packet...");

        //Packet Creation
        Packet *pk = new Packet(pkname);
        pk->setByteLength(packetLengthBytes->intValue());
        pk->setSrcAddr(myAddress);
        pk->setDestAddr(destAddress);
        pk->setPacketType(6);
        pk->setTime(SIMTIME_DBL(simTime()));
        pk->setUserXID(myAddress);

        send(pk, "out");
    }
    delete rand;

    scheduleAt(simTime() + par("anonymizerDisseminationTime").doubleValue(), timerAnonymiserDissemination);
}
bool App::haveISeenThisAnonymizerDisseminationBefore(std::string key)
{
    if (anonyDisseminationMessageSet.find(key) == anonyDisseminationMessageSet.end())
        return false;
    else
        return true;
}
void App::updateAnonymizerNodeList(int nodeId)
{
    bool alreadyPresent = false;
    for (int i = 0; i < anonymizerList.size(); i++) {
        if (anonymizerList[i].nodeId == nodeId) {
            anonymizerList[i].time = SIMTIME_DBL(simTime());
            alreadyPresent = true;
        }
    }

    if (alreadyPresent == false) {
        anonymizerList.push_back(*new AnonymizerNodeElement(nodeId, SIMTIME_DBL(simTime())));
    }
}
void App::reDisseminateMessage(Packet *pk)
{
    //TODO: this might need anti-loop control system
    cModule *mod = getParentModule()->getSubmodule("routing");
    Routing *myRouting = check_and_cast<Routing*>(mod);
    std::vector<int> neighbourNodeAddresses = myRouting->neighbourNodeAddresses;

    for (int i = 0; i < neighbourNodeAddresses.size(); i++) {
        if (pk->getSrcAddr() == neighbourNodeAddresses[i])
            continue;

        Packet *copy = (Packet *) pk->dup();
        char pkname[40];
        sprintf(pkname, "#%ld from-%d-to-%d anonymizer dissemination", pkCounter++, myAddress, neighbourNodeAddresses[i]);

        if (hasGUI())
            getParentModule()->bubble("Generating packet...");

        copy->setName(pkname);
        copy->setSrcAddr(myAddress);
        copy->setDestAddr(neighbourNodeAddresses[i]);
        send(copy, "out");
    }
}

//EVIL NODE
bool App::isNodeEvil()
{
    if (amIEvil && trustChain.size() >= (int) par("evilNumberOfSleepingTransaction")) {
        return true;
    }
    else {
        return false;
    }
}
bool App::itIsAlreadyBeenAttacked(int nodeId)    // The evil node can check here if a node as been already addressed or not
{
    int i;
    for (i = 0; i < victimDestAddresses.size(); i++) {
        if (victimDestAddresses[i] == nodeId) {
            return true;
        }
    }
    return false;
}

//SIMULATION
void App::simulationRegisterTransactionTime(int nodeId)    // Here are recorded the transaction starting times for all evil nodes
{
    int i = 0, alreadyRegistered = 0;

    for (i = 0; i < simulationTiming.size(); i++) {
        if (simulationTiming[i].nodeId == nodeId) {
            alreadyRegistered = 1;
            simulationTiming[i].transactionTime = simTime();
        }
    }

    if (alreadyRegistered == 0) {
        SimulationTiming *timeLog = new SimulationTiming(nodeId, simTime(), 0);
        simulationTiming.push_back(*timeLog);
    }
}
void App::simulationRegisterDetectionTime(int nodeId) // Here are logged the evil nodes detection times
{
    int i = 0;

    for (i = 0; i < simulationTiming.size(); i++) {
        if (simulationTiming[i].nodeId == nodeId) {
            if (simulationTiming[i].detectionTime == 0) { // In case of multiple detection caused by dissemination_detection we just need the quickest time
                simulationTiming[i].detectionTime = simTime();
            }
        }
    }
}
void App::stopSimulation() // This function is being called when ever an evil node is being detected
{
    int i, nodesDetected = 0;
    for (i = 0; i < simulationTiming.size(); i++) {
        if (simulationTiming[i].detectionTime != 0) {
            nodesDetected++;
        }
    }

// If there has been detected enough nodes equal to the evil node numbers stop simulation
    if (nodesDetected == evilNodeIds.size()) {
        char text[128];
        for (i = 0; i < simulationTiming.size(); i++) {
            sprintf(text, "Simulation: evil node #%d delta detection time:  %s", simulationTiming[i].nodeId, SIMTIME_STR(simulationTiming[i].detectionTime - simulationTiming[i].transactionTime));
            getSimulation()->getActiveEnvir()->alert(text);
        }

        simulationTiming.clear();
        endSimulation();
    }
}

//GENERAL USE
void App::createDirectChannel(int nodeId)
{
    cModule *target = getParentModule()->getParentModule()->getSubmodule("rte", nodeId)->getSubmodule("queue", 0);
    cDatarateChannel *channel = cDatarateChannel::create("channel");
    channel->setDelay((double) getParentModule()->getParentModule()->par("delay"));
    channel->setDatarate((double) getParentModule()->getParentModule()->par("datarate") * 1000);
    this->gate("direct")->connectTo(target->gate("direct"), channel);
}
void App::closeDirectChannel()
{
    this->gate("direct")->disconnect();
}

int App::randomNodeAddressPicker()
{
    int destAddress = -1;

    while (destAddress == -1 || destAddress == myAddress) {
        destAddress = destAddresses[intuniform(0, destAddresses.size() - 1)];
    }

    return destAddress;
}

//DEBUG CODE
//            if (myAddress == 0) {
//
//                int i;
//                EV << "logDatabase:" << endl;
//                for (i = 0; i < logDatabase.size(); i++) {
//                    EV << "User A: " << logDatabase[i].UserAId << " User A Seq N: " << logDatabase[i].UserASeqNum << " User B: " << logDatabase[i].UserBId << " User B Seq N: " << logDatabase[i].UserBSeqNum
//                            << " Transac: " << logDatabase[i].transactionValue << endl;
//                }
//
//                char text[128];
//                sprintf(text, "I am  #%d and i received dissemination #%d #%d <> #%d #%d $#%d ", myAddress, pk->getUserXID(), pk->getUserXSeqNum(), pk->getUserYID(), pk->getUserYSeqNum(), pk->getTransactionValue());
//                getSimulation()->getActiveEnvir()->alert(text);
//
//            }
/*
 char text[128];
 sprintf(text, "im node: #%d - and im purging node %d Time: %s s", myAddress, anonymizerList[j].nodeId, SIMTIME_STR(simTime()));
 getSimulation()->getActiveEnvir()->alert(text);
 */
