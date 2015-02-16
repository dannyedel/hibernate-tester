#include "helpers.hpp"
#include <stdexcept>

using namespace std;

vector< string > parse_argc_argv(const int argc, const char*const*const argv)
{
	vector< string > ret;
	for ( int i =0; i < argc; i++ ) {
		ret.emplace_back( argv[i] );
	}
	return ret;
}

eifstream::eifstream(const string& filename):
	stream( filename )
{
	if ( ! stream.is_open() ) {
		throw std::runtime_error("Unable to open file "+filename);
	}
}

eifstream::operator std::ifstream& () {
	return stream;
}
