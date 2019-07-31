#include <map>
#include <omnetpp.h>
#include "Packet_m.h"

using namespace omnetpp;

/**
 * Demonstrates static routing, utilizing the cTopology class.
 */
class Routing: public cSimpleModule {
private:
    int myAddress;

    typedef std::map<int, int> RoutingTable;  // destaddr -> gateindex
    RoutingTable rtable;

    simsignal_t dropSignal;
    simsignal_t outputIfSignal;
public:
    std::vector<int> neighbourNodeAddresses;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void sendOut(Packet *pk);
};
