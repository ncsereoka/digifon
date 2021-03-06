#include "ImprovedDummyScheduler.h"

#include <omnetpp.h>

using namespace omnetpp;

namespace digifon {

Define_Module(ImprovedDummyScheduler);

void ImprovedDummyScheduler::scheduleAllocableChannels() {
    reallocateChannels(userCount, allocatedChannels, userWeights,
            userQueueLengths, radioChannelCount);
}

void ImprovedDummyScheduler::reallocateChannels(int userCount,
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

}
;
