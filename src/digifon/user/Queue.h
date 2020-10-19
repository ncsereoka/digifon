#ifndef __QUEUE_H_
#define __QUEUE_H_

#include "AbstractQueue.h"

namespace digifon {

class Queue: public AbstractQueue {
protected:
    virtual simtime_t startService(cMessage *msg) override;
    virtual void endService(cMessage *msg) override;
};

}
;

#endif
