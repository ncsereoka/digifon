package digifon.user;

simple Queue
{
    parameters:
        @display("i=block/queue;q=queue");
        @signal[queueLength](type="long");
        @signal[busy](type="bool");
        @signal[queueingTime](type="simtime_t");
        @statistic[queueLength](title="queue length";record=vector,timeavg,max;interpolationmode=sample-hold);
        @statistic[busy](title="server busy state";record=vector?,timeavg;interpolationmode=sample-hold);
        @statistic[queueingTime](title="queueing time at dequeue";unit=s;record=vector,mean,max;interpolationmode=none);
    gates:
        input control_in;
        input data_in;
        output out;
}