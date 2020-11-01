#include "Queue.h"
#include "../Scheduler_Message_m.h"

namespace digifon {

Define_Module(Queue);

void Queue::initialize() {
    queue.setName("queue");

    queueLengthSignal = registerSignal("queueLength");
    emit(queueLengthSignal, queue.getLength());
}

void Queue::handleMessage(cMessage *msg) {
    if (!strcmp(msg->getName(), "controlMessage")) {
        handleControlMessage(msg);
    } else {
        arrival(msg);
        queue.insert(msg);
        msg->setTimestamp();
        if (simTime().inUnit(SIMTIME_MS) % 10 == 0) {
            emit(queueLengthSignal, queue.getLength());
        }
    }
}

void Queue::handleControlMessage(cMessage *controlMessage) {
    int allocatedChannels =
            ((SchedulerMessage*) controlMessage)->getAllocatedChannels();
    EV << "[QUEUE#" << this->getParentModule()->getIndex() << "] Received "
              << allocatedChannels << " allocated channel(s) from scheduler\n";
    delete controlMessage;

    while (!queue.isEmpty() && allocatedChannels > 0) {
        cMessage *messageToBeSent = (cMessage*) queue.pop();
        departure(messageToBeSent);
        allocatedChannels--;
    }
}

void Queue::arrival(cMessage *msg) {
    EV << "[QUEUE#" << this->getParentModule()->getIndex()
              << "] Message arrived: " << msg->getName() << '\n';
}

void Queue::departure(cMessage *msg) {
    int parentIndex = this->getParentModule()->getIndex();
    EV << "[QUEUE#" << parentIndex << "] Message \"" << msg->getName()
              << "\" sent out from User#" << parentIndex << '\n';
    send(msg, "out");
}

}
;
