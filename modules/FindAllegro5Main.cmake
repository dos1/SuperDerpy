# Try to find allegro 5
#
#  ALLEGRO5_MAIN_FOUND - system has allegro5
#  ALLEGRO5_MAIN_INCLUDE_DIRS - the allrgo5 include directory
#  ALLEGRO5_MAIN_LIBRARIES - Link these to use allegro5
#

FIND_PATH(ALLEGRO5_MAIN_INCLUDE_DIR allegro_main.h
  HINTS
  $ENV{LUA_DIR}
  PATH_SUFFIXES include/allegro5 include/allegro include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

FIND_LIBRARY(ALLEGRO5_MAIN_LIBRARY
  NAMES allegro_main
  HINTS
  $ENV{ALLEGRO5_DIR}
  PATH_SUFFIXES lib64 lib
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

MESSAGE(STATUS "${ALLEGRO5_MAIN_LIBRARY}")

IF(ALLEGRO5_MAIN_LIBRARY)
  SET(ALLEGRO5_MAIN_LIBRARIES "${ALLEGRO5_MAIN_LIBRARY}" CACHE STRING "Allegro5 Main Libraries")
  SET(ALLEGRO5_MAIN_INCLUDE_DIRS "${ALLEGRO5_MAIN_INCLUDE_DIR}" CACHE STRING "Allegro5 Main Include Dirs")
  SET(ALLEGRO5_MAIN_FOUND TRUE)
ENDIF(ALLEGRO5_MAIN_LIBRARY)

MARK_AS_ADVANCED(ALLEGRO5_MAIN_INCLUDE_DIR ALLEGRO5_MAIN_INCLUDE_DIRS ALLEGRO5_MAIN_LIBRARIES ALLEGRO5_MAIN_LIBRARY)
