#include <omnetpp.h>

using namespace omnetpp;

namespace digifon {

class Generator: public cSimpleModule {
private:
    cMessage *sendMessageEvent;

public:
    Generator();
    virtual ~Generator();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Generator);

Generator::Generator() {
    sendMessageEvent = nullptr;
}

Generator::~Generator() {
    cancelAndDelete(sendMessageEvent);
}

void Generator::initialize() {
    sendMessageEvent = new cMessage("sendMessageEvent");
    scheduleAt(simTime(), sendMessageEvent);
}

void Generator::handleMessage(cMessage *msg) {
    ASSERT(msg == sendMessageEvent);

    cMessage *job = new cMessage("job");
    send(job, "out");

    scheduleAt(simTime() + par("sendIaTime").longValue(), sendMessageEvent);
}

}
;
