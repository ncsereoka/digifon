#ifndef __IMPROVEDDUMMYSCHEDULER_H_
#define __IMPROVEDDUMMYSCHEDULER_H_

#include "DummyScheduler.h"

namespace digifon {

class ImprovedDummyScheduler: public DummyScheduler {
private:
    virtual void scheduleAllocableChannels();
    void reallocateChannels(int userCount, int *allocatedChannels, int *weights,
            int *queryLengths, int channelCount);
};

}

#endif
