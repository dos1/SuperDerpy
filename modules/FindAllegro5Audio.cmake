# Try to find allegro 5
#
#  ALLEGRO5_AUDIO_FOUND - system has allegro5
#  ALLEGRO5_AUDIO_INCLUDE_DIRS - the allrgo5 include directory
#  ALLEGRO5_AUDIO_LIBRARIES - Link these to use allegro5
#

FIND_PATH(ALLEGRO5_AUDIO_INCLUDE_DIR allegro_audio.h
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

FIND_LIBRARY(ALLEGRO5_AUDIO_LIBRARY
  NAMES allegro_audio
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

MESSAGE(STATUS "${ALLEGRO5_AUDIO_LIBRARY}")

IF(ALLEGRO5_AUDIO_LIBRARY)
  SET(ALLEGRO5_AUDIO_LIBRARIES "${ALLEGRO5_AUDIO_LIBRARY}" CACHE STRING "Allegro5 Audio Libraries")
  SET(ALLEGRO5_AUDIO_INCLUDE_DIRS "${ALLEGRO5_AUDIO_INCLUDE_DIR}" CACHE STRING "Allegro5 Audio Include Dirs")
  SET(ALLEGRO5_AUDIO_FOUND TRUE)
ENDIF(ALLEGRO5_AUDIO_LIBRARY)

MARK_AS_ADVANCED(ALLEGRO5_AUDIO_INCLUDE_DIR ALLEGRO5_AUDIO_INCLUDE_DIRS ALLEGRO5_AUDIO_LIBRARIES ALLEGRO5_AUDIO_LIBRARY)
