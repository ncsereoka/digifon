#include "WrrScheduler.h"

namespace digifon {

Define_Module(WrrScheduler);

void WrrScheduler::handleControlMessageEvent(cMessage *msg) {
    readUserQueryLengths();
    for (cModule::GateIterator i(this); !i.end(); i++) {
        cGate *gate = *i;
        int gateIndex = gate->getIndex();
        send(generateSchedulerMessage(allocatedChannels[gateIndex]), gate);
    }

    scheduleAt(simTime() + par("schedulingCycleDuration"),
            sendControlMessageEvent);
}

}
