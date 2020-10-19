package digifon;

//
// Destroys the packets and collects statistics
//
simple Sink
{
    parameters:
        @display("i=block/sink");
        @signal[lifetime](type="simtime_t");
        @statistic[lifetime](title="lifetime of arrived jobs"; unit=s; record=vector,mean,max; interpolationmode=none);
    gates:
        input in;
}
