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

int* Scheduler::initializeAllocatedChannels() {
    // Parse the initial weights
    std::vector<int> initialWeights = cStringTokenizer(
            par("initialWeights").stringValue()).asIntVector();
    int userCount = this->gateCount();
    if (initialWeights.size() != userCount) {
        throw cRuntimeError("Weights parameter count not equal to gate count.");
    }

    int weightSum = 0;
    for (std::vector<int>::iterator it = initialWeights.begin();
            it != initialWeights.end(); ++it) {
        weightSum += *it;
    }

    // Allocate the initial channels
    int *channels = new int[userCount];
    int newWeightSum = 0;
    int radioChannelCount = par("radioChannelCount").intValue();
    double factor = radioChannelCount / (double)weightSum;
    for (int i = 0; i < userCount; i++) {
        int newCount = initialWeights.at(i) * factor;
        if (newCount < 1) {
            newCount = 1;
        }
        channels[i] = newCount;
        newWeightSum += newCount;
    }

    // Distribute any remaining channels
    int diff = newWeightSum - radioChannelCount;
    int term = diff < 0 ? 1 : -1;
    for (int i = 0; i < userCount && diff != 0; i++) {
        channels[i] += term;
        diff += term;
    }

    // Logging to see the count of allocated channels
    for (int i = 0; i < userCount; i++) {
        EV << "Currently allocated channels to USER#" << i << ": "
                  << channels[i] << '\n';
    }

    return channels;
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
    // Save the number of channels previously allocated to the unlucky user
    int unluckyUserId = par("unluckyUserId").intValue();
    int unluckyUserChannels = allocatedChannels[unluckyUserId];
    allocatedChannels[unluckyUserId] = 0;

    int userCount = this->gateCount();
    int luckyUserCount = userCount - 1;

    // Calculate the bonus the others are going to get
    int bonus = unluckyUserChannels / luckyUserCount;
    if (bonus == 0) {
        bonus = 1;
    }

    // Allocate the bonus to the others
    for (int i = 0; i < userCount && unluckyUserChannels > 0; i++) {
        if (i != unluckyUserId) {
            allocatedChannels[i] += bonus;
            unluckyUserChannels -= bonus;
        }
    }

    // Logging to see the count of allocated channels
    EV << "USER#" << unluckyUserId << " lost connection!\n";
    for (int i = 0; i < userCount; i++) {
        EV << "Currently allocated channels to USER#" << i << ": "
                  << allocatedChannels[i] << '\n';
    }
}

void Scheduler::handleConnectionFoundEvent(cMessage *msg) {
    int unluckyUserId = par("unluckyUserId").intValue();
    int userCount = this->gateCount();

    // Logging to see the count of allocated channels
    EV << "USER#" << unluckyUserId << " found connection!\n";
    for (int i = 0; i < userCount; i++) {
        EV << "Currently allocated channels to USER#" << i << ": "
                  << allocatedChannels[i] << '\n';
    }
}

}
;
