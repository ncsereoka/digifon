#include "Scheduler.h"
#include "Scheduler_Message_m.h"

namespace digifon {

Define_Module(Scheduler);

Scheduler::Scheduler() {
    sendControlMessageEvent = nullptr;
}

Scheduler::~Scheduler() {
    cancelAndDelete(sendControlMessageEvent);
}

void Scheduler::initialize() {
    sendControlMessageEvent = new cMessage("SchedulerMessage");
    scheduleAt(simTime(), sendControlMessageEvent);
}

void Scheduler::handleMessage(cMessage *msg) {
    ASSERT(msg == sendControlMessageEvent);

    for (cModule::GateIterator i(this); !i.end(); i++) {
        cGate *gate = *i;
        send(generateSchedulerMessage(4), gate);
    }

    scheduleAt(simTime() + par("schedulingCycleDuration"),
            sendControlMessageEvent);
}

cMessage* Scheduler::generateSchedulerMessage(int allocatedChannels) {
    SchedulerMessage* schedulerMessage = new SchedulerMessage("controlMessage");
    schedulerMessage->setAllocatedChannels(4);
    return schedulerMessage;
}

}
;
