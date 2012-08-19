# Try to find allegro 5
#
#  ALLEGRO5_FOUND - system has allegro5
#  ALLEGRO5_INCLUDE_DIRS - the allrgo5 include directory
#  ALLEGRO5_LIBRARIES - Link these to use allegro5
#

FIND_PATH(ALLEGRO5_INCLUDE_DIR allegro.h
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

FIND_LIBRARY(ALLEGRO5_LIBRARY
  NAMES allegro
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

MESSAGE(STATUS "${ALLEGRO5_LIBRARY}")

IF(ALLEGRO5_LIBRARY)
  SET(ALLEGRO5_LIBRARIES "${ALLEGRO5_LIBRARY}" CACHE STRING "Allegro5 Libraries")
  SET(ALLEGRO5_INCLUDE_DIRS "${ALLEGRO5_INCLUDE_DIR}" CACHE STRING "Allegro5 Include Dirs")
  SET(ALLEGRO5_FOUND TRUE)
  #MESSAGE(STATUS ${ALLEGRO5_LIBRARY})
ENDIF(ALLEGRO5_LIBRARY)

MARK_AS_ADVANCED(ALLEGRO5_INCLUDE_DIR ALLEGRO5_INCLUDE_DIRS ALLEGRO5_LIBRARIES ALLEGRO5_LIBRARY)
