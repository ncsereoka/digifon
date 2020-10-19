#ifndef __SCHEDULER_H_
#define __SCHEDULER_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace digifon {

class Scheduler: public cSimpleModule {
protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

}
;

#endif
