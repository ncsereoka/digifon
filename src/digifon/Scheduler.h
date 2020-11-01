#ifndef __SCHEDULER_H_
#define __SCHEDULER_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace digifon {

class Scheduler: public cSimpleModule {
private:
    cMessage *sendControlMessageEvent;
    cMessage *unluckyUserLosesConnectionEvent;
    cMessage *unluckyUserFindsConnectionEvent;
    int *userWeights;
    int *allocatedChannels;

public:
    Scheduler();
    virtual ~Scheduler();

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

private:
    cMessage* generateSchedulerMessage(int allocatedChannels);
    void reallocateChannels();
    int* readInitialWeights();
    void handleControlMessageEvent(cMessage *msg);
    void handleConnectionLostEvent(cMessage *msg);
    void handleConnectionFoundEvent(cMessage *msg);
    void logCurrentChannels();
};

}
;

#endif
