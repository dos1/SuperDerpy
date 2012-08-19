SET(SRC_LIST
  about.c
  allegro_utils.c
  config.c
  disclaimer.c
  intro.c
  level.c
  loading.c
  main.c
  map.c
  menu.c
  moonwalk.c
  pause.c
  timeline.c
  levels/actions.c
  levels/callbacks.c
)

find_package(Allegro5 REQUIRED)
find_package(Allegro5Font REQUIRED)
find_package(Allegro5TTF REQUIRED)
find_package(Allegro5Primitives REQUIRED)
find_package(Allegro5Audio REQUIRED)
find_package(Allegro5ACodec REQUIRED)
find_package(Allegro5Image REQUIRED)
if(${APPLE})
  find_package(Allegro5Main REQUIRED)
else(${APPLE})
  set(ALLEGRO5_MAIN_INCLUDE_DIRS "")
endif(${APPLE})

include_directories(${ALLEGRO5_INCLUDE_DIRS} ${ALLEGRO5_FONT_INCLUDE_DIRS} ${ALLEGRO5_TTF_INCLUDE_DIRS} ${ALLEGRO5_PRIMITIVES_INCLUDE_DIRS} ${ALLEGRO5_AUDIO_INCLUDE_DIRS} ${ALLEGRO5_ACODEC_INCLUDE_DIRS} ${ALLEGRO5_IMAGE_INCLUDE_DIRS} ${ALLEGRO5_MAIN_INCLUDE_DIRS})

add_executable(superderpy ${SRC_LIST})

target_link_libraries(superderpy ${ALLEGRO5_LIBRARIES} ${ALLEGRO5_FONT_LIBRARIES} ${ALLEGRO5_TTF_LIBRARIES} ${ALLEGRO5_PRIMITIVES_LIBRARIES} ${ALLEGRO5_AUDIO_LIBRARIES} ${ALLEGRO5_ACODEC_LIBRARIES} ${ALLEGRO5_IMAGE_LIBRARIES} ${ALLEGRO5_MAIN_LIBRARIES} m)

install(TARGETS superderpy DESTINATION bin)
