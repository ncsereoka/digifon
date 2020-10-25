#include "Queue.h"
#include "../Scheduler_Message_m.h"

namespace digifon {

Define_Module(Queue);

Queue::Queue() {
    messageUnderService = nullptr;
    endServiceMessage = nullptr;
}

Queue::~Queue() {
    delete messageUnderService;
    cancelAndDelete(endServiceMessage);
}

void Queue::initialize() {
    endServiceMessage = new cMessage("end-service");
    queue.setName("queue");

    queueLengthSignal = registerSignal("queueLength");
    busySignal = registerSignal("busy");
    queueingTimeSignal = registerSignal("queueingTime");
    emit(queueLengthSignal, queue.getLength());
    emit(busySignal, false);
}

void Queue::handleMessage(cMessage *msg) {
    if (!strcmp(msg->getName(), "controlMessage")) {
        handleControlMessage(msg);
    } else if (msg == endServiceMessage) {
        handleEndServiceMessage();
    } else if (!messageUnderService) {
        arrival(msg);
        messageUnderService = msg;
        emit(queueingTimeSignal, SIMTIME_ZERO);
        simtime_t serviceTime = startService(messageUnderService);
        scheduleAt(simTime() + serviceTime, endServiceMessage);
        emit(busySignal, true);
    } else {
        arrival(msg);
        queue.insert(msg);
        msg->setTimestamp();
        emit(queueLengthSignal, queue.getLength());
    }
}

void Queue::arrival(cMessage *msg) {
    EV << "[QUEUE#" << this->getParentModule()->getIndex()
              << "] Message arrived: " << msg->getName() << '\n';
}

simtime_t Queue::startService(cMessage *msg) {
    EV << "[QUEUE#" << this->getParentModule()->getIndex()
              << "] Starting service of " << msg->getName() << '\n';
    return par("serviceTime");
}

void Queue::endService(cMessage *msg) {
    EV << "[QUEUE#" << this->getParentModule()->getIndex()
              << "] Completed service of " << msg->getName() << '\n';
    send(msg, "out");
}

void Queue::handleControlMessage(cMessage *controlMessage) {
    int allocatedChannels = ((SchedulerMessage *) controlMessage)->getAllocatedChannels();
    EV << "[QUEUE#" << this->getParentModule()->getIndex()
              << "] Received control message from scheduler: "
              << allocatedChannels << '\n';
    delete controlMessage;
}

void Queue::handleEndServiceMessage() {
    endService(messageUnderService);
    if (queue.isEmpty()) {
        messageUnderService = nullptr;
        emit(busySignal, false);
    } else {
        messageUnderService = (cMessage*) queue.pop();
        emit(queueLengthSignal, queue.getLength());
        emit(queueingTimeSignal,
                simTime() - messageUnderService->getTimestamp());
        simtime_t serviceTime = startService(messageUnderService);
        scheduleAt(simTime() + serviceTime, endServiceMessage);
    }
}

}
;
