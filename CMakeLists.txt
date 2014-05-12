project(string_parsing)
cmake_minimum_required(VERSION 2.6)

find_package(Boost REQUIRED COMPONENTS regex)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -std=c++11 -Wno-unused-local-typedefs")

include_directories(${Boost_INCLUDE_DIRS})

add_executable(string_parsing

	test_tools.hpp
	simple_id.hpp
	simple_id_cstr.hpp
	main.cpp
)

target_link_libraries(string_parsing
	${Boost_REGEX_LIBRARY}
)

