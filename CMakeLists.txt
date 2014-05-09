project(string_parsing)
cmake_minimum_required(VERSION 2.6)

find_package(Boost COMPONENTS regex)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)

add_executable(string_parsing

	main.cpp
)

target_link_libraries(string_parsing
	${Boost_REGEX_LIBRARY}
)

