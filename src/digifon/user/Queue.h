#ifndef __DIGIFONQUEUE_H
#define __DIGIFONQUEUE_H

#include <omnetpp.h>

using namespace omnetpp;

namespace digifon {

class Queue: public cSimpleModule {
protected:
    cMessage *messageUnderService;
    cMessage *endServiceMessage;
    cQueue queue;
    simsignal_t queueLengthSignal;
    simsignal_t busySignal;
    simsignal_t queueingTimeSignal;

public:
    Queue();
    ~Queue();

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void arrival(cMessage *msg);
    virtual simtime_t startService(cMessage *msg);
    virtual void endService(cMessage *msg);
};

}
;

#endif
