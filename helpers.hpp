#include <vector>
#include <string>
#include <fstream>


std::vector< std::string > parse_argc_argv(const int argc, const char * const * const argv );

/** Exception-throwing ifstream.
 * Throws exception if file could not be opened
 */
class eifstream {
private:
	std::ifstream stream;
public:
	eifstream( const std::string & filename );
	
	operator std::ifstream& ();
};