package digifon.simulations;

import digifon.user.User;
import digifon.Scheduler;
import digifon.Sink;

network RadioSystem
{
    parameters:
        int userCount = default(3);
        volatile double connectionLostSec @unit(s);
        volatile double connectionFoundSec @unit(s);
        int lag = default(5);
        @display("i=device/antennatower");
    submodules:
        scheduler: Scheduler {
            parameters:
                @display("p=50,50");
        }
        users[userCount]: User {
            parameters:
                @display("p=200,50,c,100");
        }
        sink: Sink {
        	parameters:
        	    @display("p=350,50");
        }
	connections allowunconnected:
	    for i=0..userCount-1 {
	        scheduler.out++ --> users[i].in;
            users[i].out --> sink.in++;
	    }
}