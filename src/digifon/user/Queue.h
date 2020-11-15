#ifndef __DIGIFONQUEUE_H
#define __DIGIFONQUEUE_H

#include <omnetpp.h>

using namespace omnetpp;

namespace digifon {

class Queue: public cSimpleModule {
public:
    int getLength();
protected:
    cQueue queue;
    simsignal_t queueLengthSignal;

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void arrival(cMessage *msg);
    virtual void departure(cMessage *msg);

private:
    virtual void handleSchedulerMessage(cMessage *schedulerMessage);
};

}
;

#endif
