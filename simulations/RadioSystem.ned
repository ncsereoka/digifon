package digifon.simulations;

import digifon.user.User;
import digifon.Scheduler;
import digifon.Sink;

network RadioSystem
{
    parameters:
        int userCount = default(3);
        int connectionLostSec = default(4);
        int connectionFoundSec = default(8);
        int lag = default(5);
        @display("i=device/antennatower");
    submodules:
        scheduler: Scheduler {
            parameters:
                @display("p=50,50");
        }
        user[userCount]: User {
            parameters:
                @display("p=200,50,c,100");
        }
        sink: Sink {
        	parameters:
        	    @display("p=350,50");
        }
	connections allowunconnected:
	    for i=0..userCount-1 {
            user[i].out --> sink.in++;
	    }
}