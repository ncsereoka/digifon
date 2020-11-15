package digifon.simulations.dummy_scheduler;

import digifon.user.User;
import digifon.scheduler.DummyScheduler;
import digifon.Sink;

network RadioSystemWithDummyScheduler
{
    parameters:
        int userCount = default(4);
        int radioChannelCount = default(30);
        double networkLoad = default(0.4);
        @display("i=device/antennatower");
    submodules:
        scheduler: DummyScheduler {
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