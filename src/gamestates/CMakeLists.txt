MACRO(GAMESTATE name)

	add_library("libsuperderpy-muffinattack-${name}" SHARED "${name}.c")

	SET_TARGET_PROPERTIES("libsuperderpy-muffinattack-${name}" PROPERTIES PREFIX "")

	target_link_libraries("libsuperderpy-muffinattack-${name}" ${ALLEGRO5_LIBRARIES} ${ALLEGRO5_FONT_LIBRARIES} ${ALLEGRO5_TTF_LIBRARIES} ${ALLEGRO5_PRIMITIVES_LIBRARIES} ${ALLEGRO5_AUDIO_LIBRARIES} ${ALLEGRO5_ACODEC_LIBRARIES} ${ALLEGRO5_IMAGE_LIBRARIES} m libsuperderpy)

	install(TARGETS "libsuperderpy-muffinattack-${name}" DESTINATION ${LIB_INSTALL_DIR})

ENDMACRO()

#GAMESTATE("menu")
GAMESTATE("disclaimer")
GAMESTATE("intro")
GAMESTATE("map")
#GAMESTATE("level")
GAMESTATE("about")
#GAMESTATE("pause")
#GAMESTATE("loading")
