add_library("libsuperderpy-muffinattack-disclaimer" SHARED "disclaimer.c")

SET_TARGET_PROPERTIES("libsuperderpy-muffinattack-disclaimer" PROPERTIES PREFIX "")

target_link_libraries("libsuperderpy-muffinattack-disclaimer" ${ALLEGRO5_LIBRARIES} ${ALLEGRO5_FONT_LIBRARIES} ${ALLEGRO5_TTF_LIBRARIES} ${ALLEGRO5_PRIMITIVES_LIBRARIES} ${ALLEGRO5_AUDIO_LIBRARIES} ${ALLEGRO5_ACODEC_LIBRARIES} ${ALLEGRO5_IMAGE_LIBRARIES} ${ALLEGRO5_MAIN_LIBRARIES} m libsuperderpy)

install(TARGETS "libsuperderpy-muffinattack-disclaimer" DESTINATION ${LIB_INSTALL_DIR})

