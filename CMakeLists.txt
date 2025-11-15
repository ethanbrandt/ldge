cmake_minimum_required(VERSION 4.2)
project (ldge VERSION 0.1.0 LANGUAGES C CXX)

add_executable( ldge
	src/main.cpp
)

set_property(TARGET ldge PROPERTY CXX_STANDARD 20)