#ifndef __DUMMYSCHEDULER_H_
#define __DUMMYSCHEDULER_H_

#include "AbstractScheduler.h"

namespace digifon {

class DummyScheduler: public AbstractScheduler {
private:
    virtual void scheduleAllocableChannels();
protected:
    void dummyAllocation(int userCount, int *allocatedChannels, int *weights,
            int channelCount);
};

}

#endif
