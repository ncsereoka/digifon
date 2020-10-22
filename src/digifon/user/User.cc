#include "User.h"

namespace digifon {

Define_Module(User);

void User::initialize() {
    // TODO - Generated method body
}

void User::handleMessage(cMessage *msg) {
    if (!strcmp(msg->getName(), "controlMessage")) {
        EV << "[USER#" << this->getIndex() << "] Received controlMessage\n";
        delete msg;
    }
}

}
;
