cmake_minimum_required(VERSION 3.10.0)
project(libber VERSION 0.1 CXX)

add_library(lib ${CMAKE_SOURCE_DIR}/asn1/ber/pch.cpp ${CMAKE_SOURCE_DIR}/asn1/ber/types.cpp)

target_include_directories(lib PUBLIC ${CMAKE_SOURCE_DIR}/inc)


if(WIN32)
elseif(APPLE)
endif()
