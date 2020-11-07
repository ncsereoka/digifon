#ifndef __SCHEDULING_ALGORITHM_H_
#define __SCHEDULING_ALGORITHM_H_

namespace digifon {

class SchedulingAlgorithm {
public:
    virtual ~SchedulingAlgorithm() {};
    virtual void reallocateChannels(int userCount, int *allocatedChannels,
            int *weights, int channelCount) = 0;
};

}
;

#endif
