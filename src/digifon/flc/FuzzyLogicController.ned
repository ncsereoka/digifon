package digifon.flc;

simple FuzzyLogicController
{
	parameters: 
	    // The config file should go in the same folder as the .ini scenario file.
		xml config= xmldoc("flc_config.xml"); 
        @display("i=block/cogwheel");
	gates:
		input in;
}