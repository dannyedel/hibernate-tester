#include "module-collection.hpp"
#include <iostream>
#include <sstream>
#include <boost/tokenizer.hpp>
#include <algorithm>

using namespace std;
using namespace boost;

module_collection parse_proc_modules(istream& input)
{
	module_collection ret;
	for( string line; !input.eof() && std::getline(input, line); ) {
		ret.emplace( line );
	}
	return ret;
}

module_entry::module_entry(const string& line):
	name{}, size{}, used_count{}, users_names{}, state{}, address{}
{
	istringstream is(line);
	// split into strings, separated by whitespace
	string list_of_users;
	// simply stream into the variables
	is >> name >> size >> used_count >> list_of_users >> state >> address;
	// boost-tokenizer can split this into a vector
	char_separator<char> sep(",");
	tokenizer<> tokens(list_of_users);
	for( auto& tok: tokens) {
		users_names.emplace( tok );
	}
}

bool operator<(const module_entry& lhs, const module_entry& rhs)
{
	return lhs.name < rhs.name;
}

ostream& operator<<(ostream& where, const module_collection& what)
{
	for( auto it = what.cbegin(); it!=what.cend(); ++it ) {
		if ( it != what.cbegin() )
			where << ", ";
		where << *it;
	}
	where << endl;
	return where;
}

ostream& operator<<(ostream& where, const module_entry& what)
{
	where << what.name;
	return where;
}

module_collection operator-(const module_collection& lhs, const module_collection& rhs)
{
	module_collection difference;
	set_difference( lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend(),
		inserter( difference, difference.begin() )
	);
	return difference;
}

module_entry pick_best_test_candidate(const module_collection& to_be_tested)
{
	if ( to_be_tested.empty() ) {
		throw std::runtime_error("Modules to be tested empty, cant pick test candidate");
	}
	/** FIXME **/
	return *to_be_tested.cbegin();
}

module_collection operator&(const module_collection& lhs, const module_collection& rhs)
{
	module_collection ret;
	set_intersection( lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend(),
		inserter(ret, ret.begin()) );
	return ret;
}

module_collection operator+(const module_collection& lhs, const module_collection& rhs)
{
	module_collection ret;
	set_union( lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend(),
			   inserter(ret, ret.begin()) );
	return ret;
}
