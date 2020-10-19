package digifon.user;

module User
{
    parameters:
        int id;
        @display("i=block/user;bgb=194,200");
    gates:
        input in;
        output out;
    submodules:
        generator: Generator {
            @display("p=100,50");
        }
        queue: Queue {
            @display("p=100,150");
        }
    connections:
        in --> queue.in;
        generator.out --> queue.in;
        queue.out --> out;
}
