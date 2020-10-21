package digifon.user;

module User
{
    parameters:
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
        generator.out --> queue.data_in;
        in --> queue.control_in;
        queue.out --> out;
}
