package digifon.user;

simple Queue
{
    parameters:
        double userQueueLengthLogDensity = default(0.1);
        @display("i=block/queue;q=queue");
        @signal[queueLength](type="long");
        @statistic[queueLength](title="queue length"; source="queueLength"; record=vector,stats; interpolationmode=none);
    gates:
        input control_in;
        input data_in;
        output out;
}