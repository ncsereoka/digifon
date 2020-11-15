#ifndef __WRRSCHEDULER_H_
#define __WRRSCHEDULER_H_

#include "AbstractScheduler.h"

namespace digifon {

class WrrScheduler: public AbstractScheduler {
private:
    int userToServe;

protected:
    virtual void initialize();

private:
    virtual void scheduleAllocableChannels();
};

}

#endif
