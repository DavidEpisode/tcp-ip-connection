################################################################################################
# short command for setting defeault target properties
function(default_properties target)
  set_target_properties(${target} PROPERTIES
    DEBUG_POSTFIX "d"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

    if (NOT ${target} MATCHES "^test_")
      install(TARGETS ${the_target} RUNTIME DESTINATION ".")
    endif()
endfunction()

################################################################################################
# short command for adding application module
# macro(add_application target)
#   FILE(GLOB_RECURSE sources *.cpp)
#   add_executable(${target} ${sources})
#   default_properties(${target})
# endmacro()