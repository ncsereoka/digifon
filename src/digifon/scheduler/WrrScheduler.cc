#include "WrrScheduler.h"

namespace digifon {

Define_Module(WrrScheduler);

void WrrScheduler::initialize() {
    AbstractScheduler::initialize();
    userToServe = 0;
}

void WrrScheduler::scheduleAllocableChannels() {
    bool emptyCycle = false;

    int channelsLeft = radioChannelCount;
    while (channelsLeft > 0) {
        const int channelsRequiredByCurrentUser = std::min(
                userWeights[userToServe], userQueryLengths[userToServe]);

        if (channelsRequiredByCurrentUser > channelsLeft) {
            allocatedChannels[userToServe] += channelsLeft;
            channelsLeft = 0;
        } else {
            allocatedChannels[userToServe] += channelsRequiredByCurrentUser;
            channelsLeft -= channelsRequiredByCurrentUser;
            userQueryLengths[userToServe] -= channelsRequiredByCurrentUser;

            // Mechanism to make sure we don't enter an infinite loop.
            // Checks whether we made a full cycle without allocating a single channel.
            if (channelsRequiredByCurrentUser == 0) {
                if (userToServe == 0) {
                    if (emptyCycle) {
                        return;
                    } else {
                        emptyCycle = true;
                    }
                }
            } else {
                emptyCycle = false;
            }

            // Go to the next user.
            userToServe++;
            if (userToServe == userCount) {
                userToServe = 0;
            }
        }
    }
}

}
