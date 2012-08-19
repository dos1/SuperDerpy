# Try to find allegro 5
#
#  ALLEGRO5_ACODEC_FOUND - system has allegro5
#  ALLEGRO5_ACODEC_INCLUDE_DIRS - the allrgo5 include directory
#  ALLEGRO5_ACODEC_LIBRARIES - Link these to use allegro5
#

FIND_PATH(ALLEGRO5_ACODEC_INCLUDE_DIR allegro_acodec.h
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

FIND_LIBRARY(ALLEGRO5_ACODEC_LIBRARY
  NAMES allegro_acodec
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

MESSAGE(STATUS "${ALLEGRO5_ACODEC_LIBRARY}")

IF(ALLEGRO5_ACODEC_LIBRARY)
  SET(ALLEGRO5_ACODEC_LIBRARIES "${ALLEGRO5_ACODEC_LIBRARY}" CACHE STRING "Allegro5 ACodec Libraries")
  SET(ALLEGRO5_ACODEC_INCLUDE_DIRS "${ALLEGRO5_ACODEC_INCLUDE_DIR}" CACHE STRING "Allegro5 ACodec Include Dirs")
  SET(ALLEGRO5_ACODEC_FOUND TRUE)
ENDIF(ALLEGRO5_ACODEC_LIBRARY)

MARK_AS_ADVANCED(ALLEGRO5_ACODEC_INCLUDE_DIR ALLEGRO5_ACODEC_INCLUDE_DIRS ALLEGRO5_ACODEC_LIBRARIES ALLEGRO5_ACODEC_LIBRARY)
