#ifndef __WRRSCHEDULER_H_
#define __WRRSCHEDULER_H_

#include "AbstractScheduler.h"

namespace digifon {

class WrrScheduler: public AbstractScheduler {
private:
    virtual void handleControlMessageEvent(cMessage *msg);
};

}

#endif
