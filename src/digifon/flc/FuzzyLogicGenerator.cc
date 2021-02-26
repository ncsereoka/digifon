#include "FuzzyLogicGenerator.h"

using namespace omnetpp;

namespace digifon {

Define_Module(FuzzyLogicGenerator);

void FuzzyLogicGenerator::initialize() {
    flc_time = 1;

    //(simtime_t)getParentModule()->par("flc_time");;
    sendMessageEvent = new cMessage("sendMessageEvent");
    scheduleAt(0, sendMessageEvent);
}

void FuzzyLogicGenerator::handleMessage(cMessage *msg) {
    simtime_t next_time;

    ASSERT(msg == sendMessageEvent);
    EV << " START FLC\n";
    cMessage *msgr = new cMessage("start_flc");
    send(msgr, "out");
    next_time = simTime() + flc_time;
    scheduleAt(next_time, sendMessageEvent);
}

void FuzzyLogicGenerator::finish() {
    EV << "*** Module: " << getFullPath() << "***" << endl;
}

}
