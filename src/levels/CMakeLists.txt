add_library("libsuperderpy-muffinattack-levels" SHARED "actions.c" "../gamestates/level.c" "modules/moonwalk.c" "modules/dodger.c")

SET_TARGET_PROPERTIES("libsuperderpy-muffinattack-levels" PROPERTIES PREFIX "")

target_link_libraries("libsuperderpy-muffinattack-levels" ${ALLEGRO5_LIBRARIES} ${ALLEGRO5_FONT_LIBRARIES} ${ALLEGRO5_TTF_LIBRARIES} ${ALLEGRO5_PRIMITIVES_LIBRARIES} ${ALLEGRO5_AUDIO_LIBRARIES} ${ALLEGRO5_ACODEC_LIBRARIES} ${ALLEGRO5_IMAGE_LIBRARIES} m libsuperderpy)

install(TARGETS "libsuperderpy-muffinattack-levels" DESTINATION ${LIB_INSTALL_DIR})


MACRO(GAMESTATE name)

	add_library("libsuperderpy-muffinattack-${name}" SHARED "${name}.c")

	SET_TARGET_PROPERTIES("libsuperderpy-muffinattack-${name}" PROPERTIES PREFIX "")

	target_link_libraries("libsuperderpy-muffinattack-${name}" ${ALLEGRO5_LIBRARIES} ${ALLEGRO5_FONT_LIBRARIES} ${ALLEGRO5_TTF_LIBRARIES} ${ALLEGRO5_PRIMITIVES_LIBRARIES} ${ALLEGRO5_AUDIO_LIBRARIES} ${ALLEGRO5_ACODEC_LIBRARIES} ${ALLEGRO5_IMAGE_LIBRARIES} m libsuperderpy libsuperderpy-muffinattack-levels)

	install(TARGETS "libsuperderpy-muffinattack-${name}" DESTINATION ${LIB_INSTALL_DIR})

ENDMACRO()

#GAMESTATE("level1")
GAMESTATE("level2")
GAMESTATE("level3")
GAMESTATE("level4")
GAMESTATE("level5")
GAMESTATE("level6")
