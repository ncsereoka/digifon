#ifndef __SCHEDULER_H_
#define __SCHEDULER_H_

#include <omnetpp.h>
#include "algorithm/SchedulingAlgorithm.h"

using namespace omnetpp;

namespace digifon {

class Scheduler: public cSimpleModule {
private:
    cMessage *sendControlMessageEvent;
    cMessage *unluckyUserLosesConnectionEvent;
    cMessage *unluckyUserFindsConnectionEvent;
    int *userWeights;
    int *allocatedChannels;
    int *userQueryLengths;
    SchedulingAlgorithm *algorithm;
    int radioChannelCount;
    int userCount;
    int unluckyUserId;

public:
    Scheduler();
    virtual ~Scheduler();

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

private:
    cMessage* generateSchedulerMessage(int allocatedChannels);
    int* readInitialWeights();
    void handleControlMessageEvent(cMessage *msg);
    void handleConnectionLostEvent(cMessage *msg);
    void handleConnectionFoundEvent(cMessage *msg);
    void logCurrentChannels();
    SchedulingAlgorithm* selectAlgorithm();
    void readUserQueryLengths();
};

}
;

#endif
