#ifndef __ABSTRACT_SCHEDULER_H_
#define __ABSTRACT_SCHEDULER_H_

#include <omnetpp.h>
#include "../user/Queue.h"

using namespace omnetpp;

namespace digifon {

class AbstractScheduler: public cSimpleModule {
protected:
    cMessage *schedulingEvent;
    cMessage *unluckyUserLosesConnectionEvent;
    cMessage *unluckyUserFindsConnectionEvent;
    int *userWeights;
    int *allocatedChannels;
    int *userQueueLengths;
    int radioChannelCount;
    int userCount;
    int unluckyUserId;
    int unluckyUserInitialWeight;
    Queue **userQueues;

public:
    AbstractScheduler();
    virtual ~AbstractScheduler();

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void scheduleAllocableChannels() = 0;
    void readUserQueueLengths();
    void logCurrentChannels();
    cMessage* generateSchedulerMessage(int allocatedChannels);
    int getQueueLengthByUserIndex(int userIndex);
    cMessage* getUserQueueFrontbyIndex(int userIndex);
    cGate *getUserGateByIndex(int userIndex);
    void resetAllocatedChannels();
    void sendAllocatedChannels();
    virtual void handleSchedulingEvent();

private:
    int* readInitialWeights();
    void handleConnectionLostEvent();
    void handleConnectionFoundEvent();
    Queue **readUserQueues();
};

}
;

#endif
