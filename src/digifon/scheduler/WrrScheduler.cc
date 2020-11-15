#include "WrrScheduler.h"

namespace digifon {

Define_Module(WrrScheduler);

void WrrScheduler::initialize() {
    AbstractScheduler::initialize();
    userToServe = 0;
}

void WrrScheduler::schedule() {
    int channelsToAllocate = radioChannelCount;
    bool finished = false;
    bool emptyCycle = false;
    while (!finished && channelsToAllocate > 0) {
        const int userQueryLength = getQueryLengthByUserIndex(userToServe);
        const int channelsRequiredByCurrentUser = std::min(
                userWeights[userToServe], userQueryLength);

        if (channelsRequiredByCurrentUser > channelsToAllocate) {
            sendMessageToServedUser(channelsToAllocate);
            channelsToAllocate = 0;
        } else {
            sendMessageToServedUser(channelsRequiredByCurrentUser);
            channelsToAllocate -= channelsRequiredByCurrentUser;

            userToServe++;
            if (userToServe == userCount) {
                userToServe = 0;
            }
        }

        // mechanism to make sure we don't enter an infinite loop
        if (channelsRequiredByCurrentUser == 0) {
            if (userToServe == 0) {
                if (emptyCycle) {
                    finished = true;
                } else {
                    emptyCycle = true;
                }
            }
        } else {
            emptyCycle = false;
        }
    }
}

void WrrScheduler::sendMessageToServedUser(int channelsToAllocate) {
    cGate *userGate = getUserGateByIndex(userToServe);
    send(generateSchedulerMessage(channelsToAllocate), userGate);
}

}
