#ifndef __USER_H_
#define __USER_H_

#include <omnetpp.h>

using namespace omnetpp;

class User: public cModule {
protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif