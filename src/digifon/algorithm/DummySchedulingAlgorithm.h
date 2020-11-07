#ifndef __DUMMY_SCHEDULING_ALGORITHM_H_
#define __DUMMY_SCHEDULING_ALGORITHM_H_

#include "SchedulingAlgorithm.h"

namespace digifon {

/*
 * Algorithm which is unaware of the length of the users' queues.
 * It will allocate a fixed amount of channels no matter the length of the queue of a specific user.
 */
class DummySchedulingAlgorithm: public SchedulingAlgorithm {
public:
    virtual void reallocateChannels(int userCount, int *allocatedChannels,
            int *weights, int *queryLengths, int channelCount);
};

}
;

#endif
