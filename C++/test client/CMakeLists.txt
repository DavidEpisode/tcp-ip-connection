cmake_minimum_required(VERSION 3.12)
# set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)

set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg-export-20190310-162947/scripts/buildsystems/vcpkg.cmake")
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "")
endif()
message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}.")

SET(CMAKE_C_USE_RESPONSE_FILE_FOR_OBJECTS 1)
SET(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS 1)
SET(CMAKE_C_RESPONSE_FILE_LINK_FLAG "@")
SET(CMAKE_CXX_RESPONSE_FILE_LINK_FLAG "@")
SET(CMAKE_NINJA_FORCE_RESPONSE_FILE 1 CACHE INTERNAL "")

project(test-client C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
include(targets)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MT /FS /O2 /Oi /openmp")
endif()

find_package(OpenCV 3 REQUIRED)
find_package(gflags CONFIG REQUIRED)
find_package(glog CONFIG REQUIRED)

message(STATUS "Found OpenCV At ${OpenCV_DIR}")
message(STATUS "Found glog At ${glog_DIR}")

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_ENABLE_EXTENDED_ALIGNED_STORAGE)
add_definitions(-DGLOG_NO_ABBREVIATED_SEVERITIES)
add_definitions(-DWIN32_LEAN_AND_MEAN)


include_directories(include
    ${OpenCV_INCLUDE_DIRS}
    ${glog_INCLUDE_DIR}
    )

#add_subdirectory(src)
add_executable(test_client main.cpp client.cpp client.hpp)
default_properties(test_client)
set_property(TARGET test_client PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build/")

target_link_libraries( test_client
    ${OpenCV_LIBS}
    gflags_shared
    glog::glog)