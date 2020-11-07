#include "Scheduler.h"
#include "Scheduler_Message_m.h"
#include "algorithm/DummySchedulingAlgorithm.h"

namespace digifon {

Define_Module(Scheduler);

Scheduler::Scheduler() {
    sendControlMessageEvent = nullptr;
    unluckyUserLosesConnectionEvent = nullptr;
    unluckyUserFindsConnectionEvent = nullptr;
    userWeights = nullptr;
    allocatedChannels = nullptr;
    algorithm = nullptr;
}

Scheduler::~Scheduler() {
    cancelAndDelete(sendControlMessageEvent);
    cancelAndDelete(unluckyUserLosesConnectionEvent);
    cancelAndDelete(unluckyUserFindsConnectionEvent);
    delete allocatedChannels;
    delete userWeights;
    delete algorithm;
}

void Scheduler::initialize() {
    userWeights = readInitialWeights();
    radioChannelCount = par("radioChannelCount").intValue();
    unluckyUserId = par("unluckyUserId").intValue();
    userCount = this->gateCount();
    allocatedChannels = new int[userCount];
    algorithm = selectAlgorithm();
    algorithm->reallocateChannels(userCount, allocatedChannels, userWeights,
            radioChannelCount);
    logCurrentChannels();

    sendControlMessageEvent = new cMessage("SchedulerMessage");
    scheduleAt(simTime(), sendControlMessageEvent);

    unluckyUserLosesConnectionEvent = new cMessage("ConnectionLostEvent");
    scheduleAt(simTime() + par("connectionLostSec"),
            unluckyUserLosesConnectionEvent);

    unluckyUserFindsConnectionEvent = new cMessage("ConnectionFoundEvent");
    scheduleAt(simTime() + par("connectionFoundSec"),
            unluckyUserFindsConnectionEvent);
}

void Scheduler::handleMessage(cMessage *msg) {
    if (msg == sendControlMessageEvent) {
        handleControlMessageEvent(msg);
    } else if (msg == unluckyUserLosesConnectionEvent) {
        handleConnectionLostEvent(msg);
    } else if (msg == unluckyUserFindsConnectionEvent) {
        handleConnectionFoundEvent(msg);
    } else {
        throw cRuntimeError("Unknown message received");
    }
}

cMessage* Scheduler::generateSchedulerMessage(int allocatedChannels) {
    SchedulerMessage *schedulerMessage = new SchedulerMessage("controlMessage");
    schedulerMessage->setAllocatedChannels(allocatedChannels);
    return schedulerMessage;
}

int* Scheduler::readInitialWeights() {
    // Parse the initial weights
    std::vector<int> initialWeights = cStringTokenizer(
            par("initialWeights").stringValue()).asIntVector();
    int userCount = this->gateCount();
    if (initialWeights.size() != userCount) {
        throw cRuntimeError("Weights parameter count not equal to gate count.");
    }

    int *weights = new int[userCount];
    for (int i = 0; i < userCount; i++) {
        weights[i] = initialWeights.at(i);
    }
    return weights;
}

void Scheduler::handleControlMessageEvent(cMessage *msg) {
    for (cModule::GateIterator i(this); !i.end(); i++) {
        cGate *gate = *i;
        int gateIndex = gate->getIndex();
        send(generateSchedulerMessage(allocatedChannels[gateIndex]), gate);
    }

    scheduleAt(simTime() + par("schedulingCycleDuration"),
            sendControlMessageEvent);
}

void Scheduler::handleConnectionLostEvent(cMessage *msg) {
    userWeights[unluckyUserId] = 0;
    algorithm->reallocateChannels(userCount, allocatedChannels, userWeights,
            radioChannelCount);

    EV << "USER#" << unluckyUserId << " found connection!\n";
    logCurrentChannels();
}

void Scheduler::handleConnectionFoundEvent(cMessage *msg) {
    userWeights[unluckyUserId] = par("unluckyUserNewWeight").intValue();
    algorithm->reallocateChannels(userCount, allocatedChannels, userWeights,
            radioChannelCount);

    EV << "USER#" << unluckyUserId << " found connection!\n";
    logCurrentChannels();
}

void Scheduler::logCurrentChannels() {
    int userCount = this->gateCount();
    for (int i = 0; i < userCount; i++) {
        EV << "Currently allocated channels to USER#" << i << ": "
                  << allocatedChannels[i] << '\n';
    }
}

SchedulingAlgorithm* Scheduler::selectAlgorithm() {
//    const char* param = par("algorithm").stringValue();
//    if (!strcmp(param, "dummy")) {
//        return new DummySchedulingAlgorithm();
//    }

    return new DummySchedulingAlgorithm();
}

}
;
