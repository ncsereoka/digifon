package digifon;

simple Scheduler
{
    parameters:
        volatile double schedulingCycleDuration @unit(s);
        int radioChannelCount;
        @display("i=block/fork");
    gates:
        output out[];
}
