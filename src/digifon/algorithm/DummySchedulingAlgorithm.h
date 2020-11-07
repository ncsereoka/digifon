#ifndef __DUMMY_SCHEDULING_ALGORITHM_H_
#define __DUMMY_SCHEDULING_ALGORITHM_H_

#include "SchedulingAlgorithm.h"

namespace digifon {

class DummySchedulingAlgorithm: public SchedulingAlgorithm {
public:
    virtual void reallocateChannels(int userCount, int *allocatedChannels,
            int *weights, int channelCount);
};

}
;

#endif
