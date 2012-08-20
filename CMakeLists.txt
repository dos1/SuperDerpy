cmake_minimum_required(VERSION 2.8)

project(SuperDerpy C)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -ansi -g")
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if(MINGW)
    # Guess MINGDIR from the value of CMAKE_C_COMPILER if it's not set.
    if("$ENV{MINGDIR}" STREQUAL "")
        string(REGEX REPLACE "/bin/[^/]*$" "" MINGDIR "${CMAKE_C_COMPILER}")
        message(STATUS "Guessed MinGW directory: ${MINGDIR}")
    else("$ENV{MINGDIR}" STREQUAL "")
        file(TO_CMAKE_PATH "$ENV{MINGDIR}" MINGDIR)
        message(STATUS "Using MINGDIR: ${MINGDIR}")
    endif("$ENV{MINGDIR}" STREQUAL "")

    # Search in MINGDIR for headers and libraries.
    set(CMAKE_PREFIX_PATH "${MINGDIR}")

    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -mwindows")

endif(MINGW)

add_subdirectory(src)
install(DIRECTORY data DESTINATION share/superderpy)

# uninstall target
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
