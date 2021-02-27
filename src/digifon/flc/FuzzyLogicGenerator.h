#ifndef __FUZZY_LOGIC_GENERATOR_H_
#define __FUZZY_LOGIC_GENERATOR_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace digifon {

class FuzzyLogicGenerator: public cSimpleModule {
private:
    cMessage *sendMessageEvent;

protected:
    simtime_t flc_time;
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

}

#endif
