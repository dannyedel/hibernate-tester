#pragma once
#include "module-collection.hpp"

#include <boost/asio.hpp>

class server {
private:
	/** FIXME assign a better port **/
	static const int server_port = 6666;
	
	static const std::string known_good_modules_filename;
	/** Target list of modules */
	module_collection target_modules;
	/** List of known good modules, initially empty */
	module_collection known_good;
	/** List of known bad modules */
	module_collection known_bad;
	/** List of currently loaded modules on the client */
	module_collection currently_loaded;
	
	module_entry told_to_test;
public:
	/** Initializes server, starts listening */
	explicit server( const module_collection& target, const module_collection& good,  const module_collection& bad );
	
	/** run the server. This loop will only terminate if client
	 * completes all test cases or does something unexpected.
	 */
	void run();
	
private:
	std::string readline(boost::asio::ip::tcp::socket& socket, boost::asio::streambuf& buffer);
	module_collection readProcModules(boost::asio::ip::tcp::socket& socket, boost::asio::streambuf& buffer);
	
	
	void writeline(boost::asio::ip::tcp::socket& socket, const std::string& line);
	
	/** Serves the bashscript to the server */
	void download( boost::asio::ip::tcp::socket& socket );
	
	/** Accepts the client's known good modules */
	void knowngood( boost::asio::ip::tcp::socket& socket, boost::asio::streambuf& buffer);
	
	/** Tells the client what to load */
	void loadwhat( boost::asio::ip::tcp::socket& socket );
	
	/** Tells the client what to un-load */
	void unloadwhat( boost::asio::ip::tcp::socket& socket );

	/** Accepts the client's loaded modules */
	void loaded( boost::asio::ip::tcp::socket& socket, boost::asio::streambuf& buffer);
};