# FindK4A
# ----------
# Find Kinect Azure SDK include dirs, library dirs, librarys 
#
# First install Kinect Azure SDK 
# And add the directory "C:\Program Files\Azure Kinect SDK v1.1.0\sdk"(default install directory)
# To environment variable
# 
# Use this module by incoking find_package with the form::
#
#  find_package( K4A [REQUIRED] )
#
# Results for users are reported in following variables::
#
# K4A_FOUND             - Return "True" when K4A found. Otherwise "False"
# K4A_INCLUDE_DIRS      - K4A SDK include directories. (${K4A_DIR}/include)
# K4A_LIBRARY_DIRS      - K4A SDK library directories. (${K4A_DIR}/windows-desktop/amd64/release/lib)
# K4A_BIN_DIRS          - K4A SDK binary directories. (${K4A_DIR}/windows-desktop/amd64/release/bin)
# K4A_LIBRARIES         - K4A SDK libraries files. (${K4A_LIBRARY_DIRS}/*.lib)
# K4A_BIN               - k4A SDK binary files. (${K4A_BIN_DIRS}/*.dll)
#
# This module reads hints about search locations from following environment variables::
#
#    K4A_DIR            - K4A SDK root directory. (This environment variable has been set by installer of Kinect SDK v2.)
#
#
# ================================================================================================================
# Written by David Zhang 
# 7/4/2019
# For artemis
# ================================================================================================================

######## Utility #############

# Check Directory Macro
macro(CHECK_DIR _DIR)
  if(NOT EXISTS "${${_DIR}}")
    message(WARNING "Directory \"${${_DIR}}\" not found.")
    set(K4A_FOUND FALSE)
    unset(_DIR)
  endif()
endmacro()

# Check Files Macro
macro(CHECK_FILES _FILES _DIR)
  set(_MISSING_FILES)
  foreach(_FILE {${_FILES}})
    if(NOT EXISTS "${_FILE}")
      get_filename_component(_FILE ${_FILE} NAME)
      set(_MISSING_FILES "${_MISSING_FILES}${_FILE}, ")
    endif()
  endforeach()
  if(_MISSING_FILES)
    message(WARNING "In directory \" ${${_DIR}}\" not found files: ${_MISSING_FILES}")
    set(K4A_FOUND FALSE)
    unset(_FILES)
  endif()
endmacro()

# Target Platform
# set(TARGET_PLATFORM)
# if(NOT CMAKE_CL_64)
#   set(TARGET_PLATFORM x86)
# else()
#   set(TARGET_PLATFORM x64)
# endif()

######## Find K4A SDK #########

# Found
set(K4A_FOUND TRUE)
if(MSVC_VERSION LESS 1700)
  message(WARNING "Kinect Azure for Windows SDK supported Visual Studio 2012 or later.")
  set(K4A_FOUND FALSE)
endif()

# Root Directory
set(K4A_DIR)
if(K4A_FOUND)
  set(K4A_DIR $ENV{K4A_DIR} CACHE 
      PATH "Kinect Azure for Windows SDK install PAth." FORCE)
  CHECK_DIR(K4A_DIR)
endif()

# Include directories
set(K4A_INCLUDE_DIRS)
if(K4A_FOUND)
  set(K4A_INCLUDE_DIRS ${K4A_DIR}/include)
  CHECK_DIR(K4A_INCLUDE_DIRS)
endif()

# Library directories
set(K4A_LIBRARY_DIRS)
if(K4A_FOUND)
  set(K4A_LIBRARY_DIRS ${K4A_DIR}/windows-desktop/amd64/release/lib)
  CHECK_DIR(K4A_LIBRARY_DIRS)
endif()

# Dependencies
set(K4A_LIBRARIES)
if(K4A_FOUND)
  file(GLOB_RECURSE K4A_LIBRARIES ${K4A_LIBRARY_DIRS}/*.lib)
endif()

# Binary directories
set(K4A_BIN_DIRS)
if(K4A_FOUND)
  set(K4A_BIN_DIRS ${K4A_DIR}/windows-desktop/amd64/release/bin)
  CHECK_DIR(K4A_BIN_DIRS)
endif()

# Runtime
set(K4A_BIN)
if(K4A_FOUND)
  file(GLOB_RECURSE K4A_BIN ${K4A_BIN_DIRS}/*.*)
endif()

message(STATUS "K4A_FOUND : ${K4A_FOUND}")

