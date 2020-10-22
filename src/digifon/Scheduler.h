#ifndef __SCHEDULER_H_
#define __SCHEDULER_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace digifon {

class Scheduler: public cSimpleModule {
private:
    cMessage *sendControlMessageEvent;
    cMessage *generateSchedulerMessage(int allocatedChannels);

public:
    Scheduler();
    virtual ~Scheduler();

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

}
;

#endif
