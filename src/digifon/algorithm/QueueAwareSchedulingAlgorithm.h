#ifndef __QUEUEAWARE_SCHEDULING_ALGORITHM_H_
#define __QUEUEAWARE_SCHEDULING_ALGORITHM_H_

#include "SchedulingAlgorithm.h"

namespace digifon {

/*
 * Algorithm which is aware of the length of the users' queues.
 * It will take into account the length of each queue and adjust each allocation accordingly.
 */
class QueueAwareSchedulingAlgorithm: public SchedulingAlgorithm {
public:
    virtual void reallocateChannels(int userCount, int *allocatedChannels,
            int *weights, int *queryLengths, int channelCount);
private:
    void dummyAllocation(int userCount, int *allocatedChannels, int *weights,
            int channelCount);
};

}
;

#endif
