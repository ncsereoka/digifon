#ifndef __GENERATOR_H_
#define __GENERATOR_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace digifon {

class Generator: public cSimpleModule {
private:
    cMessage *sendMessageEvent;

public:
    Generator();
    ~Generator();

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

}
;

#endif
