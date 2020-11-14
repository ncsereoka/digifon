#include "AbstractScheduler.h"

#include "../Scheduler_Message_m.h"
#include "../user/Queue.h"

namespace digifon {

AbstractScheduler::AbstractScheduler() {
    sendControlMessageEvent = nullptr;
    unluckyUserLosesConnectionEvent = nullptr;
    unluckyUserFindsConnectionEvent = nullptr;
    userWeights = nullptr;
    userQueryLengths = nullptr;
    allocatedChannels = nullptr;
}

AbstractScheduler::~AbstractScheduler() {
    cancelAndDelete(sendControlMessageEvent);
    cancelAndDelete(unluckyUserLosesConnectionEvent);
    cancelAndDelete(unluckyUserFindsConnectionEvent);
    delete allocatedChannels;
    delete userWeights;
    delete userQueryLengths;
}

void AbstractScheduler::initialize() {
    userWeights = readInitialWeights();
    radioChannelCount = par("radioChannelCount").intValue();
    unluckyUserId = par("unluckyUserId").intValue();
    userCount = this->gateCount();
    allocatedChannels = new int[userCount];
    userQueryLengths = new int[userCount];
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

void AbstractScheduler::handleMessage(cMessage *msg) {
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

cMessage* AbstractScheduler::generateSchedulerMessage(int allocatedChannels) {
    SchedulerMessage *schedulerMessage = new SchedulerMessage("controlMessage");
    schedulerMessage->setAllocatedChannels(allocatedChannels);
    return schedulerMessage;
}

int* AbstractScheduler::readInitialWeights() {
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

void AbstractScheduler::handleConnectionLostEvent(cMessage *msg) {
    userWeights[unluckyUserId] = 0;
    EV << "USER#" << unluckyUserId << " found connection!\n";
}

void AbstractScheduler::handleConnectionFoundEvent(cMessage *msg) {
    userWeights[unluckyUserId] = par("unluckyUserNewWeight").intValue();
    EV << "USER#" << unluckyUserId << " found connection!\n";
}

void AbstractScheduler::logCurrentChannels() {
    int userCount = this->gateCount();
    for (int i = 0; i < userCount; i++) {
        EV << "Currently allocated channels to USER#" << i << ": "
                  << allocatedChannels[i] << '\n';
    }
}

void AbstractScheduler::readUserQueryLengths() {
    for (cModule::GateIterator i(this); !i.end(); i++) {
        cGate *gate = *i;
        int userIndex = gate->getIndex();
        cModule *userModule = gate->getNextGate()->getOwnerModule();
        Queue *userQueue = check_and_cast<Queue*>(
                userModule->getModuleByPath(".queue"));
        userQueryLengths[userIndex] = userQueue->getLength();
    }
}

}
;
