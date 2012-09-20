IF(${PACKAGE_BUILD})

 IF(WIN32)
  FILE(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/win32/*.dll")
  INSTALL(FILES ${files} DESTINATION ${BIN_INSTALL_DIR})
 ENDIF(WIN32)

 IF(UNIX)
  FILE(GLOB files "${CMAKE_SOURCE_DIR}/lib/linux-x86/*.so*")
  INSTALL(FILES ${files} DESTINATION ${LIB_INSTALL_DIR}/superderpy)
 ENDIF(UNIX)

ENDIF(${PACKAGE_BUILD})

# TODO: handle libraries in Mac OS X version if needed
# TODO: handle other architectures
