package digifon.scheduler;

simple WrrScheduler
{
    parameters:
        volatile double schedulingCycleDuration @unit(s);
        string initialWeights;
        int radioChannelCount;
        int unluckyUserId;
        int unluckyUserNewWeight;
        bool modifyUnluckyUserWeight = default(false);
        double connectionLostSec @unit(s);
        double connectionFoundSec @unit(s);
        @display("i=block/fork");
    gates:
        output out[];
}
