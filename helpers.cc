#include "helpers.hpp"

using namespace std;

vector< string > parse_argc_argv(const int argc, const char*const*const argv)
{
	vector< string > ret;
	for ( int i =0; i < argc; i++ ) {
		ret.emplace_back( argv[i] );
	}
	return ret;
}
