if(MINGW)
		# resource compilation for MinGW
		add_custom_command( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/icon.o
				 COMMAND ${CMAKE_WINDRES} -I${CMAKE_SOURCE_DIR} -i${CMAKE_SOURCE_DIR}/data/icons/icon.rc
				 -o ${CMAKE_CURRENT_BINARY_DIR}/icon.o )
		set(VLMC_SRCS ${VLMC_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/icon.o)
		set(LINK_FLAGS -Wl,-subsystem,windows)

endif(MINGW)

install(FILES 16/superderpy.png DESTINATION ${ICON_INSTALL_DIR}/hicolor/16x16/apps)
install(FILES 32/superderpy.png DESTINATION ${ICON_INSTALL_DIR}/hicolor/32x32/apps)
install(FILES 48/superderpy.png DESTINATION ${ICON_INSTALL_DIR}/hicolor/48x48/apps)
install(FILES 64/superderpy.png DESTINATION ${ICON_INSTALL_DIR}/hicolor/64x64/apps)
install(FILES 128/superderpy.png DESTINATION ${ICON_INSTALL_DIR}/hicolor/128x128/apps)
install(FILES 256/superderpy.png DESTINATION ${ICON_INSTALL_DIR}/hicolor/256x256/apps)
install(FILES superderpy.svg DESTINATION ${ICON_INSTALL_DIR}/hicolor/scalable/apps)
install(FILES superderpy.png DESTINATION ${SHARE_INSTALL_PREFIX}/superderpy/data/icons)
