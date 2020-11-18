#ifndef __FIFOSCHEDULER_H_
#define __FIFOSCHEDULER_H_

#include "AbstractScheduler.h"

namespace digifon {

class FifoScheduler: public AbstractScheduler {
protected:
    virtual void scheduleAllocableChannels();
    virtual void handleSchedulingEvent();
};

}

#endif
