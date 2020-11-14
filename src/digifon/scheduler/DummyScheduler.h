#ifndef __DUMMYSCHEDULER_H_
#define __DUMMYSCHEDULER_H_

#include "AbstractScheduler.h"

namespace digifon {

class DummyScheduler: public AbstractScheduler {
private:
    virtual void handleControlMessageEvent(cMessage *msg);
    void reallocateChannels(int userCount, int *allocatedChannels, int *weights,
            int *queryLengths, int channelCount);
};

}

#endif
