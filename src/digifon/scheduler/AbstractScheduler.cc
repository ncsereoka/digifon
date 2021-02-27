#include "AbstractScheduler.h"

#include "../Scheduler_Message_m.h"
#include "../user/Queue.h"

namespace digifon {

AbstractScheduler::AbstractScheduler() {
    schedulingEvent = nullptr;
    unluckyUserLosesConnectionEvent = nullptr;
    unluckyUserFindsConnectionEvent = nullptr;
    userWeights = nullptr;
    userQueueLengths = nullptr;
    allocatedChannels = nullptr;
    userQueues = nullptr;
    allowUnluckyUserWeightChange = false;
}

AbstractScheduler::~AbstractScheduler() {
    cancelAndDelete(schedulingEvent);
    cancelAndDelete(unluckyUserLosesConnectionEvent);
    cancelAndDelete(unluckyUserFindsConnectionEvent);
    delete allocatedChannels;
    delete userWeights;
    delete userQueueLengths;
    delete userQueues;
}

void AbstractScheduler::initialize() {
    userWeights = readInitialWeights();
    radioChannelCount = getParentModule()->par("radioChannelCount").intValue();
    unluckyUserId = par("unluckyUserId").intValue();
    unluckyUserInitialWeight = userWeights[unluckyUserId];
    userCount = getParentModule()->par("userCount").intValue();
    allocatedChannels = new int[userCount];
    userQueueLengths = new int[userCount];
    userQueues = readUserQueues();
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
    if (allowUnluckyUserWeightChange) {
        userWeights[unluckyUserId] = par("unluckyUserNewWeight").intValue();
    }

    readUserQueueLengths();
    resetAllocatedChannels();
    scheduleAllocableChannels();
    sendAllocatedChannels();
    scheduleAt(simTime() + par("schedulingCycleDuration"), schedulingEvent);
}

void AbstractScheduler::handleConnectionLostEvent() {
    userWeights[unluckyUserId] = 0;
    EV << "USER#" << unluckyUserId << " found connection!\n";
}

void AbstractScheduler::handleConnectionFoundEvent() {
    bool modifyWeight = par("modifyUnluckyUserWeight").boolValue();

    allowUnluckyUserWeightChange = true;

// Modify to the new weight or set back the initial one.
    userWeights[unluckyUserId] =
            modifyWeight ?
                    par("unluckyUserNewWeight").intValue() :
                    unluckyUserInitialWeight;

    EV << "USER#" << unluckyUserId << " found connection!\n";
}

void AbstractScheduler::logCurrentChannels() {
    int userCount = this->gateCount();
    for (int i = 0; i < userCount; i++) {
        EV << "Currently allocated channels to USER#" << i << ": "
                  << allocatedChannels[i] << '\n';
    }
}

void AbstractScheduler::readUserQueueLengths() {
    for (cModule::GateIterator i(this); !i.end(); i++) {
        cGate *gate = *i;
        int userIndex = gate->getIndex();
        cModule *userModule = gate->getNextGate()->getOwnerModule();
        Queue *userQueue = check_and_cast<Queue*>(
                userModule->getModuleByPath(".queue"));
        userQueueLengths[userIndex] = userQueue->getLength();
    }
}

int AbstractScheduler::getQueueLengthByUserIndex(int userIndex) {
    cModule *userModule =
            getUserGateByIndex(userIndex)->getNextGate()->getOwnerModule();
    Queue *userQueue = check_and_cast<Queue*>(
            userModule->getModuleByPath(".queue"));
    return userQueue->getLength();
}

cMessage* AbstractScheduler::getUserQueueFrontbyIndex(int userIndex) {
    cModule *userModule =
            getUserGateByIndex(userIndex)->getNextGate()->getOwnerModule();
    Queue *userQueue = check_and_cast<Queue*>(
            userModule->getModuleByPath(".queue"));
    return userQueue->getFront();
}

cGate* AbstractScheduler::getUserGateByIndex(int userIndex) {
    int baseId = gateBaseId("out");
    return gate(baseId + userIndex);
}

void AbstractScheduler::resetAllocatedChannels() {
    for (int i = 0; i < userCount; i++) {
        allocatedChannels[i] = 0;
    }
}

void AbstractScheduler::sendAllocatedChannels() {
    for (cModule::GateIterator i(this); !i.end(); i++) {
        cGate *gate = *i;
        int gateIndex = gate->getIndex();
        send(generateSchedulerMessage(allocatedChannels[gateIndex]), gate);
        EV << "Allocated " << allocatedChannels[gateIndex]
                  << " channels to USER#" << gateIndex << '\n';
    }
}

Queue** AbstractScheduler::readUserQueues() {
    Queue **userQueues = new Queue*[userCount];
    for (int i = 0; i < userCount; i++) {
        cModule *userModule =
                getUserGateByIndex(i)->getNextGate()->getOwnerModule();
        userQueues[i] = check_and_cast<Queue*>(
                userModule->getModuleByPath(".queue"));
    }

    return userQueues;
}

}
;
