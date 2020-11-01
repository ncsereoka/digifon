#include "Scheduler.h"
#include "Scheduler_Message_m.h"

namespace digifon {

Define_Module(Scheduler);

Scheduler::Scheduler() {
    sendControlMessageEvent = nullptr;
    unluckyUserLosesConnectionEvent = nullptr;
    unluckyUserFindsConnectionEvent = nullptr;
    userWeights = nullptr;
    allocatedChannels = nullptr;
}

Scheduler::~Scheduler() {
    cancelAndDelete(sendControlMessageEvent);
    cancelAndDelete(unluckyUserLosesConnectionEvent);
    cancelAndDelete(unluckyUserFindsConnectionEvent);
    delete allocatedChannels;
    delete userWeights;
}

void Scheduler::initialize() {
    userWeights = readInitialWeights();
    allocatedChannels = new int[this->gateCount()];
    reallocateChannels();
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

void Scheduler::reallocateChannels() {
    int userCount = this->gateCount();
    int initialWeightSum = 0;
    for (int i = 0; i < userCount; i++) {
        initialWeightSum += userWeights[i];
    }

    // Allocate the initial channels
    int newWeightSum = 0;
    int radioChannelCount = par("radioChannelCount").intValue();
    double factor = radioChannelCount / (double) initialWeightSum;
    for (int i = 0; i < userCount; i++) {
        // Don't stress about channels if the weight is zero
        if (userWeights[i] == 0) {
            allocatedChannels[i] = 0;
        } else {
            int newCount = userWeights[i] * factor;
            if (newCount < 1) {
                newCount = 1;
            }
            allocatedChannels[i] = newCount;
            newWeightSum += newCount;
        }
    }

    // Distribute any remaining channels
    int diff = newWeightSum - radioChannelCount;
    int term = diff < 0 ? 1 : -1;
    for (int i = 0; i < userCount && diff != 0; i++) {
        if (userWeights[i] != 0) {
            allocatedChannels[i] += term;
            diff += term;
        }
    }
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
    int unluckyUserId = par("unluckyUserId").intValue();
    userWeights[unluckyUserId] = 0;
    reallocateChannels();

    EV << "USER#" << unluckyUserId << " found connection!\n";
    logCurrentChannels();
}

void Scheduler::handleConnectionFoundEvent(cMessage *msg) {
    int unluckyUserId = par("unluckyUserId").intValue();
    userWeights[unluckyUserId] = par("unluckyUserNewWeight").intValue();
    reallocateChannels();

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

}
;
