#include "AbstractScheduler.h"

#include "../Scheduler_Message_m.h"
#include "../user/Queue.h"

namespace digifon {

AbstractScheduler::AbstractScheduler() {
    schedulingEvent = nullptr;
    unluckyUserLosesConnectionEvent = nullptr;
    unluckyUserFindsConnectionEvent = nullptr;
    userWeights = nullptr;
    userQueryLengths = nullptr;
    allocatedChannels = nullptr;
}

AbstractScheduler::~AbstractScheduler() {
    cancelAndDelete(schedulingEvent);
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

    schedulingEvent = new cMessage("SchedulerMessage");
    scheduleAt(simTime(), schedulingEvent);

    unluckyUserLosesConnectionEvent = new cMessage("ConnectionLostEvent");
    scheduleAt(simTime() + par("connectionLostSec"),
            unluckyUserLosesConnectionEvent);

    unluckyUserFindsConnectionEvent = new cMessage("ConnectionFoundEvent");
    scheduleAt(simTime() + par("connectionFoundSec"),
            unluckyUserFindsConnectionEvent);
}

void AbstractScheduler::handleMessage(cMessage *msg) {
    if (msg == schedulingEvent) {
        handleSchedulingEvent();
    } else if (msg == unluckyUserLosesConnectionEvent) {
        handleConnectionLostEvent();
    } else if (msg == unluckyUserFindsConnectionEvent) {
        handleConnectionFoundEvent();
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

void AbstractScheduler::handleSchedulingEvent() {
    schedule();
    scheduleAt(simTime() + par("schedulingCycleDuration"), schedulingEvent);
}

void AbstractScheduler::handleConnectionLostEvent() {
    userWeights[unluckyUserId] = 0;
    EV << "USER#" << unluckyUserId << " found connection!\n";
}

void AbstractScheduler::handleConnectionFoundEvent() {
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

int AbstractScheduler::getQueryLengthByUserIndex(int userIndex) {
    cModule *userModule =
            getUserGateByIndex(userIndex)->getNextGate()->getOwnerModule();
    Queue *userQueue = check_and_cast<Queue*>(
            userModule->getModuleByPath(".queue"));
    return userQueue->getLength();
}

cGate* AbstractScheduler::getUserGateByIndex(int userIndex) {
    int baseId = gateBaseId("out");
    return gate(baseId + userIndex);
}

}
;
