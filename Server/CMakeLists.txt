cmake_minimum_required (VERSION 2.6)
project (SBGServer)

set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

find_package(Boost 1.54.0 REQUIRED COMPONENTS system)

set(websocketpp_INCLUDE_DIR /usr/local/src/websocketpp)

include_directories("${PROJECT_BINARY_DIR}")
include_directories( ${websocketpp_INCLUDE_DIR} )
include_directories( ${Boost_INCLUDE_DIR} )
 
add_executable(SBGServer Server.cpp Packet.cpp PlayerSession.cpp stdafx.cpp)

target_link_libraries (SBGServer ${Boost_LIBRARIES})