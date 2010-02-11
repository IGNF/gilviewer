# Options pour construire une librarie (et la choisir en .a ou .so)
if( UNIX )
	SET( GILVIEWER_LIBRARY_TYPE SHARED )
	# Verification du type de bibliotheque a construire
	OPTION( BUILD_GILVIEWERLIB_SHARED "Build GilViewer as a SHARED library." ON )
	IF( BUILD_GILVIEWERLIB_SHARED )
		SET( GILVIEWER_LIBRARY_TYPE SHARED )
	ELSE()
		SET( GILVIEWER_LIBRARY_TYPE STATIC )
	ENDIF()
# Currently, dll are not supported on Windows platforms
else()
	set( GILVIEWER_LIBRARY_TYPE STATIC )
endif()

ADD_LIBRARY( GilViewer ${GILVIEWER_LIBRARY_TYPE} ${ALL_VIEWER_SOURCES} ${ALL_VIEWER_HEADERS})
TARGET_LINK_LIBRARIES( GilViewer ${wxWidgets_LIBRARIES} ${Boost_LIBRARIES} shapelib tinyxml gdal1.5.0 )

