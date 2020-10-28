#include "Scheduler.h"
#include "Scheduler_Message_m.h"

namespace digifon {

Define_Module(Scheduler);

Scheduler::Scheduler() {
    sendControlMessageEvent = nullptr;
    allocatedChannels = nullptr;
}

Scheduler::~Scheduler() {
    cancelAndDelete(sendControlMessageEvent);
    delete allocatedChannels;
}

void Scheduler::initialize() {
    sendControlMessageEvent = new cMessage("SchedulerMessage");
    scheduleAt(simTime(), sendControlMessageEvent);

    int outputGatesCount = this->gateCount();
    allocatedChannels = new int[outputGatesCount];

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
        allocatedChannels[i] = newCount;
        newWeightSum += newCount;
    }

    int diff = newWeightSum - weightSum;
    if (diff < 0) {
        allocatedChannels[0] -= diff;
    } else {
        for (int i = 0; i < outputGatesCount; i++) {
            if (allocatedChannels[i] > diff) {
                allocatedChannels[i] -= diff;
                break;
            }
        }
    }
}

void Scheduler::handleMessage(cMessage *msg) {
    ASSERT(msg == sendControlMessageEvent);

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

}
;
