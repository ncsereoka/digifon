package digifon.scheduler;

simple DummyScheduler
{
    parameters:
        volatile double schedulingCycleDuration @unit(s);
        string initialWeights;
        int radioChannelCount;
        int unluckyUserId;
        int unluckyUserNewWeight;
        double connectionLostSec @unit(s);
        double connectionFoundSec @unit(s);
        @display("i=block/fork");
    gates:
        output out[];
}
