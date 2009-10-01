# Options pour construire une librarie (et la choisir en .a ou .so)
SET( GILVIEWER_LIBRARY_TYPE SHARED )
# Verification du type de bibliotheque a construire
OPTION( BUILD_GILVIEWERLIB_SHARED "Build GilViewer as a SHARED library." ON )
IF( BUILD_GILVIEWERLIB_SHARED )
	SET( GILVIEWER_LIBRARY_TYPE SHARED )
ELSE()
	SET( GILVIEWER_LIBRARY_TYPE STATIC )
ENDIF()

ADD_LIBRARY( GilViewer  ${GILVIEWER_LIBRARY_TYPE} ${ALL_VIEWER_SOURCES} ${ALL_VIEWER_HEADERS})
TARGET_LINK_LIBRARIES( GilViewer ${wxWidgets_LIBRARIES} ${Boost_LIBRARIES} shapelib tinyxml )

#set_target_properties( GilViewer PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib )
#set_target_properties( GilViewer PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib )
