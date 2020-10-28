package digifon;

simple Scheduler
{
    parameters:
        volatile double schedulingCycleDuration @unit(s);
        string initialWeights;
        int radioChannelCount;
        int unluckyUserId;
        double connectionLostSec @unit(s);
        double connectionFoundSec @unit(s);
        @display("i=block/fork");
    gates:
        output out[];
}
