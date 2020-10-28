#ifndef __SCHEDULER_H_
#define __SCHEDULER_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace digifon {

class Scheduler: public cSimpleModule {
private:
    cMessage *sendControlMessageEvent;
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
};

}
;

#endif
