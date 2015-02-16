#include "server.hpp"

using boost::asio::ip::tcp;
using namespace std;

server::server(const module_collection& target):
	target_modules(target), known_good(), currently_loaded()
{

}

struct eof_exception: public std::runtime_error {
public:
	using std::runtime_error::runtime_error;
};

void server::run()
{
	boost::asio::io_service io_service;
	
	tcp::acceptor acceptor(io_service, tcp::endpoint( tcp::v4(), server::server_port ) );
	
	for(;;) {
		tcp::socket socket(io_service);
		/** Blocking **/
		acceptor.accept(socket);
		
		/** Reading buffer */
		boost::asio::streambuf buffer;
		
		/** Read first line */
		const std::string command_word{ readline(socket, buffer) };
		
		std::clog << "Received command from client: " << command_word << endl;
		
		if ( command_word == "DOWNLOAD" ) {
			download( socket );
		}
		else if ( command_word == "KNOWNGOOD" ) {
			knowngood( socket, buffer );
		}
		else if ( command_word == "LOADED" ) {
			loaded( socket, buffer );
		}
		else if ( command_word == "LOADWHAT" ) {
			loadwhat(socket);
		}
		else {
			cerr << "WARNING: Received unknown command word " << command_word << endl;
		}
	}
}

void server::download(tcp::socket& socket )
{
	writeline(socket, "Not yet implemented");
}
string server::readline(tcp::socket& socket, boost::asio::streambuf& buffer)
{
	boost::system::error_code error;
	boost::asio::read_until(socket, buffer, '\n', error);
	if ( error == boost::asio::error::eof ) {
		throw eof_exception("Socket EOF");
	} else if ( error ) {
		throw boost::system::system_error(error);
	}
	std::istream is(&buffer);
	std::string line;
	std::getline(is, line);
	return line;
}

void server::writeline(tcp::socket& socket, const string& line)
{
	boost::asio::write(socket, boost::asio::buffer(line+'\n'));
}

void server::knowngood(tcp::socket& socket, boost::asio::streambuf& buffer)
{
	module_collection kg = readProcModules(socket, buffer);
	clog << "Received known good modules from client: " << kg << endl;
	known_good = move(kg);
}

module_collection server::readProcModules(tcp::socket& socket, boost::asio::streambuf& buffer)
{
	/** Read complete message into memory */
	
	std::string procmodules;
	
	try {
		for(;;) {
			procmodules += readline(socket,buffer)+'\n';
		}
	} catch( eof_exception&e ) {
	}
	
	std::istringstream is(procmodules);
	
	return parse_proc_modules(is);
}


void server::loaded(tcp::socket& socket, boost::asio::streambuf& buffer)
{
	module_collection loaded = readProcModules(socket, buffer);
	clog << "Received loaded modules from client: " << loaded << endl;
	currently_loaded = move(loaded);
	clog << "INFO: The client is now testing " << currently_loaded - known_good << endl;
}

void server::loadwhat(tcp::socket& socket)
{
	module_collection toBeTested = target_modules - known_good;
	module_entry bestCandidate = pick_best_test_candidate(toBeTested);
	clog << "Telling the client to load " << bestCandidate << endl;
	writeline(socket, bestCandidate.name );
}
