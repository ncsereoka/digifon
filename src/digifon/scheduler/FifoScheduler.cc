#include "FifoScheduler.h"

using namespace omnetpp;

namespace digifon {

Define_Module(FifoScheduler);

void FifoScheduler::scheduleAllocableChannels() {
}

void FifoScheduler::handleSchedulingEvent() {
    int channelsLeft = radioChannelCount;
    while (channelsLeft > 0) {
        bool messageExists = false;
        int selectedTarget = -1;
        simtime_t earliestTime = simTime();
        for (int i = 0; i < userCount; i++) {
            // Checking the weight we know whether we should serve the unlucky user.
            if (userWeights[i] > 0 && userQueues[i]->getLength() > 0) {
                messageExists = true;

                simtime_t creationTime =
                        userQueues[i]->getFront()->getCreationTime();
                if (creationTime < earliestTime) {
                    earliestTime = creationTime;
                    selectedTarget = i;
                }
            }
        }

        if (messageExists) {
            send(generateSchedulerMessage(1),
                    getUserGateByIndex(selectedTarget));
            channelsLeft--;
        } else {
            break;
        }
    }

    scheduleAt(simTime() + par("schedulingCycleDuration"), schedulingEvent);
}

}
