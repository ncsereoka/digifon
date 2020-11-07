#include "DummySchedulingAlgorithm.h"

namespace digifon {

void DummySchedulingAlgorithm::reallocateChannels(int userCount,
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
