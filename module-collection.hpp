#pragma once

#include <string>
#include <set>
#include <cstdint>

/** One entry from /proc/modules
 */
struct module_entry {
	std::string name;
	std::uint64_t size;
	std::uint64_t used_count;
	std::set< std::string > users_names;
	std::string state;
	std::uint64_t address;

	/** Initialize from a line in /proc/modules */
	explicit module_entry( const std::string& line );
};

/** compare for two modules.
 * for our purposes, they are ONLY compared by their name.
 */
bool operator < ( const module_entry & lhs, const module_entry & rhs );

/** simple stream output, prints name
 */
std::ostream& operator << ( std::ostream& where, const module_entry& what );

/**
 * Collection of (loaded) modules
 *
 * supports difference operator, equality check
 */
typedef std::set< module_entry > module_collection;

/** Reads through input (format expected to be like /proc/modules)
 * until EOF, returning a module collection
 */
module_collection parse_proc_modules( std::istream& input );

/** prints comma-separated names
 */
std::ostream& operator << ( std::ostream& where, const module_collection& what );


/** Returns everything in lhs that is NOT in rhs
 * */
module_collection operator - ( const module_collection & lhs, const module_collection & rhs );

/** Do some mumbo jumbo to figure out the best module to test now
 * 
 *  FIXME this simply uses the alphabetically first
 */
module_entry pick_best_test_candidate( const module_collection& to_be_tested );