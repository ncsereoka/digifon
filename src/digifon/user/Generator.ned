package digifon.user;

simple Generator
{
    parameters:
        volatile double messageCreationInterval @unit(s);
        @display("i=block/ifcard");
    gates:
        output out;
}
