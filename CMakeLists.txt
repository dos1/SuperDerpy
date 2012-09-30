cmake_minimum_required(VERSION 2.8)

project(SuperDerpy C)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g")
if(APPLE)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fnested-functions")
  if(CMAKE_INSTALL_PREFIX MATCHES "/usr/local")
    set(CMAKE_INSTALL_PREFIX "/Applications")
    set(BIN_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}")
  endif(CMAKE_INSTALL_PREFIX MATCHES "/usr/local")
endif(APPLE)
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include("cmake/SetPaths.cmake")

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

    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -mwindows")

		# resource compilation for MinGW
		add_custom_command( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/icon.o
				 COMMAND ${CMAKE_WINDRES} -I${CMAKE_SOURCE_DIR} -i${CMAKE_SOURCE_DIR}/data/icons/icon.rc
				 -o ${CMAKE_CURRENT_BINARY_DIR}/icon.o )
		set(VLMC_SRCS ${VLMC_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/icon.o)
		#set(LINK_FLAGS -Wl,-subsystem,windows)

endif(MINGW)

add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(lib)

# uninstall target
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Become Derpy Hooves and fight with muffinzombies in order to rescue Ponyville ponies so they can defeat Discord once again!")
SET(CPACK_PACKAGE_VENDOR "Sebastian Krzyszkowiak")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "2")
SET(CPACK_PACKAGE_VERSION_PATCH "pre")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "Super Derpy")
IF(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  SET(CPACK_GENERATOR "NSIS")
  #SET(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/Utilities/Release/InstallIcon.bmp")
  SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\superderpy.exe")
  SET(CPACK_NSIS_DISPLAY_NAME "Super Derpy: Muffin Attack")
  SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.superderpy.com")
  SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\dosowisko.net")
  SET(CPACK_NSIS_CONTACT "dos@dosowisko.net")
  SET(CPACK_NSIS_CREATE_ICONS "CreateShortCut '\$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${CPACK_NSIS_DISPLAY_NAME}.lnk' '\$INSTDIR\\\\bin\\\\superderpy.exe'")
ELSE(WIN32 AND NOT UNIX)
  SET(CPACK_GENERATOR "DEB" "RPM")
  SET(CPACK_STRIP_FILES "bin/superderpy")
  SET(CPACK_SOURCE_STRIP_FILES "")
  IF(APPLE)
      SET(CPACK_GENERATOR "DragNDrop")
  ENDIF(APPLE)
ENDIF(WIN32 AND NOT UNIX)
IF(UNIX)
  SET(CPACK_SET_DESTDIR "ON")
ENDIF(UNIX)
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Sebastian Krzyszkowiak <dos@dosowisko.net>")
#SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6), libgcc1 (>= 1:3.4.2-12)")

include(CPack)
