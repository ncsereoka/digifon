#include "Queue.h"

namespace digifon {

Define_Module(Queue);

simtime_t Queue::startService(cMessage *msg) {
    EV << "Starting service of " << msg->getName() << '\n';
    return par("serviceTime");
}

void Queue::endService(cMessage *msg) {
    EV << "Completed service of " << msg->getName() << '\n';
    send(msg, "out");
}

}
;

