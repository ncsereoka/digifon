#include "AbstractQueue.h"

namespace digifon {

AbstractQueue::AbstractQueue() {
    msgServiced = nullptr;
    endServiceMsg = nullptr;
}

AbstractQueue::~AbstractQueue() {
    delete msgServiced;
    cancelAndDelete(endServiceMsg);
}

void AbstractQueue::initialize() {
    endServiceMsg = new cMessage("end-service");
    queue.setName("queue");

    qlenSignal = registerSignal("qlen");
    busySignal = registerSignal("busy");
    queueingTimeSignal = registerSignal("queueingTime");
    emit(qlenSignal, queue.getLength());
    emit(busySignal, false);
}

void AbstractQueue::handleMessage(cMessage *msg) {
    if (msg == endServiceMsg) {
        endService(msgServiced);
        if (queue.isEmpty()) {
            msgServiced = nullptr;
            emit(busySignal, false);
        } else {
            msgServiced = (cMessage*) queue.pop();
            emit(qlenSignal, queue.getLength());
            emit(queueingTimeSignal, simTime() - msgServiced->getTimestamp());
            simtime_t serviceTime = startService(msgServiced);
            scheduleAt(simTime() + serviceTime, endServiceMsg);
        }
    } else if (!msgServiced) {
        arrival(msg);
        msgServiced = msg;
        emit(queueingTimeSignal, SIMTIME_ZERO);
        simtime_t serviceTime = startService(msgServiced);
        scheduleAt(simTime() + serviceTime, endServiceMsg);
        emit(busySignal, true);
    } else {
        arrival(msg);
        queue.insert(msg);
        msg->setTimestamp();
        emit(qlenSignal, queue.getLength());
    }
}

}
;

