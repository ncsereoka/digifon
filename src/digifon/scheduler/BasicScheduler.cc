#include "BasicScheduler.h"

#include <omnetpp.h>

using namespace omnetpp;

namespace digifon {

Define_Module(BasicScheduler);

void BasicScheduler::handleControlMessageEvent(cMessage *msg) {
    readUserQueryLengths();
    reallocateChannels(userCount, allocatedChannels, userWeights, userQueryLengths, radioChannelCount);
    for (cModule::GateIterator i(this); !i.end(); i++) {
        cGate *gate = *i;
        int gateIndex = gate->getIndex();
        send(generateSchedulerMessage(allocatedChannels[gateIndex]), gate);
    }

    scheduleAt(simTime() + par("schedulingCycleDuration"),
            sendControlMessageEvent);
}

void BasicScheduler::reallocateChannels(int userCount,
        int *allocatedChannels, int *weights, int *queryLengths,
        int channelCount) {
    // First, do a dummy allocation, not taking into account query lengths.
    dummyAllocation(userCount, allocatedChannels, weights, channelCount);

    // Shave off any extra
    int channelsToBeRedistributed = 0;
    int *auxWeights = new int[userCount];
    int *auxLengths = new int[userCount];
    for (int i = 0; i < userCount; i++) {
        if (allocatedChannels[i] >= queryLengths[i]) {
            channelsToBeRedistributed +=
                    (allocatedChannels[i] - queryLengths[i]);
            allocatedChannels[i] = queryLengths[i];
            auxLengths[i] = 0;
            auxWeights[i] = 0;
        } else {
            auxLengths[i] = queryLengths[i] - allocatedChannels[i];
            auxWeights[i] = weights[i];
        }
    }

    // If we have some extra channels that can be redistributed,
    // do it according to the initial weights,
    // again with a dummy allocation.
    if (channelsToBeRedistributed > 0) {
        int *auxChannels = new int[userCount];
        dummyAllocation(userCount, auxChannels, auxWeights,
                channelsToBeRedistributed);
        for (int i = 0; i < userCount; i++) {
            allocatedChannels[i] += auxChannels[i];
        }
        delete[] auxChannels;
    }

    delete[] auxWeights;
    delete[] auxLengths;
}

void BasicScheduler::dummyAllocation(int userCount,
        int *allocatedChannels, int *weights, int channelCount) {
    int initialWeightSum = 0;
    for (int i = 0; i < userCount; i++) {
        initialWeightSum += weights[i];
    }

    // Allocate the initial channels
    int newWeightSum = 0;
    double factor = channelCount / (double) initialWeightSum;
    for (int i = 0; i < userCount; i++) {
        // Don't stress about channels if the weight is zero
        if (weights[i] == 0) {
            allocatedChannels[i] = 0;
        } else {
            int newCount = weights[i] * factor;
            if (newCount < 1) {
                newCount = 1;
            }
            allocatedChannels[i] = newCount;
            newWeightSum += newCount;
        }
    }

    // Distribute any remaining channels
    int diff = newWeightSum - channelCount;
    int term = diff < 0 ? 1 : -1;
    for (int i = 0; i < userCount && diff != 0; i++) {
        if (weights[i] != 0) {
            allocatedChannels[i] += term;
            diff += term;
        }
    }
}

}
;
