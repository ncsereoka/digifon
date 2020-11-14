#ifndef __ABSTRACT_SCHEDULER_H_
#define __ABSTRACT_SCHEDULER_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace digifon {

class AbstractScheduler: public cSimpleModule {
protected:
    cMessage *sendControlMessageEvent;
    cMessage *unluckyUserLosesConnectionEvent;
    cMessage *unluckyUserFindsConnectionEvent;
    int *userWeights;
    int *allocatedChannels;
    int *userQueryLengths;
    int radioChannelCount;
    int userCount;
    int unluckyUserId;

public:
    AbstractScheduler();
    virtual ~AbstractScheduler();

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void handleControlMessageEvent(cMessage *msg) = 0;
    void readUserQueryLengths();
    void logCurrentChannels();
    cMessage* generateSchedulerMessage(int allocatedChannels);

private:
    int* readInitialWeights();
    void handleConnectionLostEvent(cMessage *msg);
    void handleConnectionFoundEvent(cMessage *msg);
};

}
;

#endif
