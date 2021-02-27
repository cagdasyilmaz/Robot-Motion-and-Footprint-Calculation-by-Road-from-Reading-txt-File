cmake_minimum_required(VERSION 3.16.5)
project(cpp)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_definitions(-std=c++17)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_BUILD_TYPE Debug)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(sources src/main.cpp src/file_manipulation.h src/robot_motion.h 
	src/robot_footprint.h src/locked_stream.h src/thread_functions.h src/log_watch.h)

find_package (Threads)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)
link_directories(/usr/bin)

add_executable(cpp ${sources})

target_link_libraries (cpp ${CMAKE_THREAD_LIBS_INIT})
