cmake_minimum_required(VERSION 2.8)
set( PROJECT calc_server )
project( ${PROJECT} )

set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11" )
add_definitions( -DSHOW_TIME )

find_library(PTHREAD pthread)
find_package( Boost 1.55.0 REQUIRED COMPONENTS system thread program_options )

add_executable( ${PROJECT}
                main.cpp
                calculator.h
                calculator.cpp
                server.h
                server.cpp
                logger.h
                logger.cpp
                calc_handle.h
                calc_handle_factory.h
                big_int/BigInteger.hh
                big_int/BigInteger.cc
                big_int/BigUnsigned.cc
                big_int/BigUnsigned.hh
                big_int/BigUnsignedInABase.cc
                big_int/BigUnsignedInABase.hh
                big_int/BigIntegerUtils.cc
                big_int/BigIntegerUtils.hh )

include_directories( ${Boost_INCLUDE_DIRS} )
link_directories( ${Boost_LIBRARY_DIRS} )
target_link_libraries( ${PROJECT} ${PTHREAD}
                                  ${Boost_LIBRARIES} )
