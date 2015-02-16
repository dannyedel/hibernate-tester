project(hibernate-tester)

cmake_minimum_required(VERSION 2.6)
find_package(Boost REQUIRED COMPONENTS system thread regex)

include_directories(${Boost_INCLUDE_DIR})

add_definitions(-ggdb -Wall -Werror -Wextra -Weffc++ -pedantic -pedantic-errors)

set(hibernate_sources
	hibernate-test
	module-collection
	helpers
	server
)

add_executable(hibernate-tester ${hibernate_sources})
target_link_libraries(hibernate-tester ${Boost_LIBRARIES})
