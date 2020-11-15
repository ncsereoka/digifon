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

    cModule *radioSystem = getParentModule()->getParentModule();
    double networkLoad = radioSystem->par("networkLoad").doubleValue();
    int userCount = radioSystem->par("userCount").intValue();
    int radioChannelCount = radioSystem->par("radioChannelCount").intValue();
    intervalScaler = userCount / (radioChannelCount * networkLoad);
}

void Generator::handleMessage(cMessage *msg) {
    ASSERT(msg == sendMessageEvent);

    cMessage *newData = new cMessage("data");
    send(newData, "out");

    simtime_t restTime = par("messageCreationInterval").doubleValue();
    restTime *= intervalScaler;

    scheduleAt(simTime() + restTime, sendMessageEvent);
}

}
;
