# Try to find allegro 5
#
#  ALLEGRO5_IMAGE_FOUND - system has allegro5
#  ALLEGRO5_IMAGE_INCLUDE_DIRS - the allrgo5 include directory
#  ALLEGRO5_IMAGE_LIBRARIES - Link these to use allegro5
#

FIND_PATH(ALLEGRO5_IMAGE_INCLUDE_DIR allegro_image.h
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

FIND_LIBRARY(ALLEGRO5_IMAGE_LIBRARY
  NAMES allegro_image
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

MESSAGE(STATUS "${ALLEGRO5_IMAGE_LIBRARY}")

IF(ALLEGRO5_IMAGE_LIBRARY)
  SET(ALLEGRO5_IMAGE_LIBRARIES "${ALLEGRO5_IMAGE_LIBRARY}" CACHE STRING "Allegro5 Image Libraries")
  SET(ALLEGRO5_IMAGE_INCLUDE_DIRS "${ALLEGRO5_IMAGE_INCLUDE_DIR}" CACHE STRING "Allegro5 Image Include Dirs")
  SET(ALLEGRO5_IMAGE_FOUND TRUE)
ENDIF(ALLEGRO5_IMAGE_LIBRARY)

MARK_AS_ADVANCED(ALLEGRO5_IMAGE_INCLUDE_DIR ALLEGRO5_IMAGE_INCLUDE_DIRS ALLEGRO5_IMAGE_LIBRARIES ALLEGRO5_IMAGE_LIBRARY)
