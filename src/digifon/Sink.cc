#include <omnetpp.h>

using namespace omnetpp;

namespace digifon {

class Sink: public cSimpleModule {
private:
    simsignal_t lifetimeSignal;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Sink);

void Sink::initialize() {
    lifetimeSignal = registerSignal("lifetime");
}

void Sink::handleMessage(cMessage *msg) {
    simtime_t lifetime = simTime() - msg->getCreationTime();
    EV << "Received " << msg->getName() << ", lifetime: " << lifetime << "s" << '\n';
    emit(lifetimeSignal, lifetime);
    delete msg;
}

}
;

