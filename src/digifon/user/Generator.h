#ifndef __GENERATOR_H_
#define __GENERATOR_H_

#include <omnetpp.h>

using namespace omnetpp;

class Generator: public cSimpleModule {
protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
