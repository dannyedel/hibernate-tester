#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "module-collection.hpp"
#include "helpers.hpp"

using namespace std;

int main(int argc,char** argv) {
	vector<string> parameters = parse_argc_argv(argc,argv);

	// Construct full-modules from textfile
	module_collection target_modules{};
	{
		string filename{ parameters.at(0) };
		ifstream procfile;
		procfile.exceptions( ios::failbit | ios::badbit );
		procfile.open(filename);
		clog << "Opened " << filename << endl;
		target_modules = parse_proc_modules(procfile);
	}
	clog << target_modules;
}
