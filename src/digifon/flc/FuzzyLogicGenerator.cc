#include "FuzzyLogicGenerator.h"

using namespace omnetpp;

namespace digifon {

Define_Module(FuzzyLogicGenerator);

void FuzzyLogicGenerator::initialize() {
    flc_time = 0.1;
    simtime_t startTime = (simtime_t) getParentModule()->getSubmodule(
            "scheduler")->par("connectionFoundSec");
    sendMessageEvent = new cMessage("sendMessageEvent");
    scheduleAt(startTime, sendMessageEvent);
}

void FuzzyLogicGenerator::handleMessage(cMessage *msg) {
    ASSERT(msg == sendMessageEvent);
    EV << " START FLC\n";

    send(new cMessage("start_flc"), "out");
    scheduleAt(simTime() + flc_time, sendMessageEvent);
}
}
