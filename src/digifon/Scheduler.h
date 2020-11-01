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
    int *allocatedChannels;

public:
    Scheduler();
    virtual ~Scheduler();

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

private:
    cMessage* generateSchedulerMessage(int allocatedChannels);
    bool isConnectionNormal();
    int* initializeAllocatedChannels();
    void handleControlMessageEvent(cMessage *msg);
    void handleConnectionLostEvent(cMessage *msg);
    void handleConnectionFoundEvent(cMessage *msg);
};

}
;

#endif
