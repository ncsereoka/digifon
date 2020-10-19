package digifon.user;

simple Generator
{
    parameters:
        volatile double sendIaTime @unit(s);
        @display("i=block/ifcard");
    gates:
        output out;
}
