#ifndef __ABSTRACTQUEUE_H
#define __ABSTRACTQUEUE_H

#include <omnetpp.h>

using namespace omnetpp;

namespace digifon {

/**
 * Abstract base class for single-server queues. Subclasses should
 * define startService() and endService(), and may override other
 * virtual functions.
 */
class AbstractQueue: public cSimpleModule {
protected:
    cMessage *msgServiced;
    cMessage *endServiceMsg;
    cQueue queue;
    simsignal_t qlenSignal;
    simsignal_t busySignal;
    simsignal_t queueingTimeSignal;

public:
    AbstractQueue();
    virtual ~AbstractQueue();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    // hook functions to (re)define behaviour
    virtual void arrival(cMessage *msg) {
    }
    virtual simtime_t startService(cMessage *msg) = 0;
    virtual void endService(cMessage *msg) = 0;
};

}
;

#endif
