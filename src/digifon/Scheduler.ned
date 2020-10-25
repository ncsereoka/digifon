package digifon;

simple Scheduler
{
    parameters:
        volatile double schedulingCycleDuration @unit(s);
        string weights;
        int radioChannelCount;
        @display("i=block/fork");
    gates:
        output out[];
}
