project(hibernate-tester)

cmake_minimum_required(VERSION 2.6)
find_package(Boost REQUIRED COMPONENTS system thread regex)

include_directories(${Boost_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_definitions(-ggdb -Wall -Werror -Wextra -Weffc++ -pedantic -pedantic-errors)

set(hibernate_sources
	hibernate-test
	module-collection
	helpers
	server
)


add_custom_command(
	OUTPUT client_sh.h
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/client.sh client.sh
	COMMAND xxd -i client.sh client_sh.h
	DEPENDS client.sh
)

add_executable(hibernate-tester ${hibernate_sources} client_sh.h)


target_link_libraries(hibernate-tester ${Boost_LIBRARIES})
