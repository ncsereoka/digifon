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
                @display("p=150,50");
        }
        sink: Sink {
        	parameters:
        	    @display("p=250,50");
        }
}