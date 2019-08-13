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

project(image_server C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
include(targets)
include(FindK4A)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MT /FS /O2 /Oi /openmp")
endif()

find_package(OpenCV 3 REQUIRED)
find_package(gflags CONFIG REQUIRED)
find_package(glog CONFIG REQUIRED)
find_package(Boost REQUIRED)

message(STATUS "Found OpenCV At ${OpenCV_DIR}")
message(STATUS "Found glog At ${glog_DIR}")

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_ENABLE_EXTENDED_ALIGNED_STORAGE)
add_definitions(-DGLOG_NO_ABBREVIATED_SEVERITIES)
add_definitions(-DWIN32_LEAN_AND_MEAN)


include_directories(include
    ${OpenCV_INCLUDE_DIRS}
    ${glog_INCLUDE_DIR}
    ${K4A_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    )

#add_subdirectory(src)
add_executable(image_server main.cpp 
                            server.cpp 
                            server.h
                            k4a_camera.hpp
                            k4a_camera.cpp)
default_properties(image_server)
set_property(TARGET image_server PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build/")

target_link_libraries( image_server
    ${OpenCV_LIBS}
    gflags_shared
    glog::glog
    ${K4A_LIBRARIES})

add_custom_command(TARGET image_server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${K4A_BIN}
       ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>)