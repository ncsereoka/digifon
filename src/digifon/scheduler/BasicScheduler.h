#ifndef __BASICSCHEDULER_H_
#define __BASICSCHEDULER_H_

#include "AbstractScheduler.h"

#include <omnetpp.h>

using namespace omnetpp;

namespace digifon {

class BasicScheduler: public AbstractScheduler {
private:
    virtual void handleControlMessageEvent(cMessage *msg);
    void reallocateChannels(int userCount, int *allocatedChannels, int *weights,
            int *queryLengths, int channelCount);
    void dummyAllocation(int userCount, int *allocatedChannels, int *weights,
            int channelCount);
};

}

#endif
