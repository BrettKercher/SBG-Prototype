cmake_minimum_required (VERSION 2.6)
project (SBGServer)

set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.54.0 REQUIRED COMPONENTS system date_time regex)

set(websocketpp_INCLUDE_DIR "S:/cygwin64/usr/local/src/websocketpp")

include_directories("${PROJECT_BINARY_DIR}")
include_directories( ${websocketpp_INCLUDE_DIR} )
include_directories( ${Boost_INCLUDE_DIR} )
 
 set(SOURCES Server.cpp Packet.cpp PlayerSession.cpp stdafx.cpp)
 set(HEADERS Server.h Packet.h PlayerSession.h stdafx.h)
 
add_executable(SBGServer ${SOURCES} ${HEADERS})

target_link_libraries (SBGServer ${Boost_LIBRARIES})