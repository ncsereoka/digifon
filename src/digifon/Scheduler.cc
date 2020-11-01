#include "Scheduler.h"
#include "Scheduler_Message_m.h"

namespace digifon {

Define_Module(Scheduler);

Scheduler::Scheduler() {
    sendControlMessageEvent = nullptr;
    unluckyUserLosesConnectionEvent = nullptr;
    unluckyUserFindsConnectionEvent = nullptr;
    allocatedChannels = nullptr;
}

Scheduler::~Scheduler() {
    cancelAndDelete(sendControlMessageEvent);
    cancelAndDelete(unluckyUserLosesConnectionEvent);
    cancelAndDelete(unluckyUserFindsConnectionEvent);
    delete allocatedChannels;
}

void Scheduler::initialize() {
    allocatedChannels = initializeAllocatedChannels();

    sendControlMessageEvent = new cMessage("SchedulerMessage");
    scheduleAt(simTime(), sendControlMessageEvent);

    unluckyUserLosesConnectionEvent = new cMessage(
            "Unlucky user lost connection.");
    scheduleAt(simTime() + par("connectionLostSec"),
            unluckyUserLosesConnectionEvent);

    unluckyUserFindsConnectionEvent = new cMessage(
            "Unlucky user found connection.");
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

bool Scheduler::isConnectionNormal() {
    simtime_t currentTime = simTime();
    bool beforeConnectionLost = currentTime < par("connectionLostSec");
    bool afterConnectionFound = currentTime > par("connectionFoundSec");
    return beforeConnectionLost || afterConnectionFound;
}

int* Scheduler::initializeAllocatedChannels() {
    int outputGatesCount = this->gateCount();
    int *channels = new int[outputGatesCount];

    std::vector<int> initialWeights = cStringTokenizer(
            par("initialWeights").stringValue()).asIntVector();
    if (initialWeights.size() != outputGatesCount) {
        throw cRuntimeError("Weights parameter count not equal to gate count.");
    }

    int weightSum = 0;
    for (std::vector<int>::iterator it = initialWeights.begin();
            it != initialWeights.end(); ++it) {
        weightSum += *it;
    }

    int newWeightSum = 0;
    int factor = par("radioChannelCount").intValue() / weightSum;
    for (int i = 0; i < outputGatesCount; i++) {
        int newCount = initialWeights.at(i) * factor;
        if (newCount < 1) {
            newCount = 1;
        }
        channels[i] = newCount;
        newWeightSum += newCount;
    }

    int diff = newWeightSum - weightSum;
    if (diff < 0) {
        channels[0] -= diff;
    } else {
        for (int i = 0; i < outputGatesCount; i++) {
            if (channels[i] > diff) {
                channels[i] -= diff;
                break;
            }
        }
    }

    return channels;
}

void Scheduler::handleControlMessageEvent(cMessage *msg) {
    for (cModule::GateIterator i(this); !i.end(); i++) {
        cGate *gate = *i;
        int gateIndex = gate->getIndex();
        bool normalUser = gateIndex != par("unluckyUserId").intValue();

        if (normalUser || isConnectionNormal()) {
            send(generateSchedulerMessage(allocatedChannels[gateIndex]), gate);
        }
    }

    scheduleAt(simTime() + par("schedulingCycleDuration"),
            sendControlMessageEvent);
}

void Scheduler::handleConnectionLostEvent(cMessage *msg) {
    int unluckyUserId = par("unluckyUserId").intValue();
    EV << "USER#" << unluckyUserId << " lost connection!\n";
}

void Scheduler::handleConnectionFoundEvent(cMessage *msg) {
    int unluckyUserId = par("unluckyUserId").intValue();
    EV << "USER#" << unluckyUserId << " found connection!\n";
}

}
;
