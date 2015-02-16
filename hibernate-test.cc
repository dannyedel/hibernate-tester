#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "module-collection.hpp"
#include "helpers.hpp"
#include "server.hpp"

using namespace std;

int main(int argc,char** argv) {
	vector<string> parameters = parse_argc_argv(argc,argv);

	// Construct full-modules from textfile
	module_collection target_modules;
	{
		
		eifstream procfile( parameters.at(1) );
		target_modules = parse_proc_modules(procfile);
	}
	clog << "Target modules: " << target_modules;
	
	server s( target_modules );
	
	clog << "Starting server" << endl;
	
	s.run();
}

#if 0
	
	module_collection known_good;
	{
		eifstream goodfile( parameters.at(2) );
		known_good = parse_proc_modules(goodfile);
	}
	clog << "Known to work: " << known_good;
	
	module_collection to_be_tested = target_modules - known_good;
	
	if ( to_be_tested.empty() ) {
		clog << "All tested, we're done." << endl;
		return 0;
	}
	
	module_entry to_modprobe = pick_best_test_candidate(to_be_tested);
	clog << "Next to test: " << to_modprobe << endl;
	
	/** FIXME **/
	cout << "FIXME: LOADING " << to_modprobe << endl;
	
	module_collection actually_loaded;
	{
		eifstream actually( parameters.at(3) );
		actually_loaded = parse_proc_modules(actually);
	}
	module_collection currently_testing = actually_loaded - known_good;
	clog << "Modprobe loaded, NOW TESTING: " << currently_testing;
	
#endif
