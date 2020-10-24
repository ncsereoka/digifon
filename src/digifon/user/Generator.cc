#include "Generator.h"

using namespace omnetpp;

namespace digifon {

Define_Module(Generator);

Generator::Generator() {
    sendMessageEvent = nullptr;
}

Generator::~Generator() {
    cancelAndDelete(sendMessageEvent);
}

void Generator::initialize() {
    sendMessageEvent = new cMessage("sendMessageEvent");
    scheduleAt(simTime(), sendMessageEvent);
}

void Generator::handleMessage(cMessage *msg) {
    ASSERT(msg == sendMessageEvent);

    cMessage *newData = new cMessage("data");
    send(newData, "out");

    scheduleAt(simTime() + par("messageCreationInterval"), sendMessageEvent);
}

}
;
