####
#### Creation d'un eecutable d'exemple linaknt sur la libITKViewer
####
IF(UNIX)
	IF(APPLE)	  
		ADD_EXECUTABLE( GilViewerApp MACOSX_BUNDLE ./viewer_app/GilViewer.cpp
		                                              ./viewer_app/GilViewer.h
                                                      ./viewer_app/FrameViewer.cpp
                                                      ./viewer_app/FrameViewer.hpp )
	ELSE()
		ADD_EXECUTABLE( GilViewerApp ./viewer_app/GilViewer.cpp ./viewer_app/FrameViewer.cpp )
	ENDIF()
	ADD_DEFINITIONS(-Wall)
	TARGET_LINK_LIBRARIES( GilViewerApp ${wxWidgets_LIBRARIES} ${Boost_LIBRARIES} GilViewer shapelib tinyxml )
ENDIF(UNIX)
# Si c'est du windows, on lui dit de faire une application windows (et pas une console)
# On en profite aussi pour ajouter les resources
# Et les headers ...
IF(WIN32)
	# J'en peux plus de ces warnings ...
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
	ADD_EXECUTABLE( GilViewerApp WIN32 ./viewer_app/GilViewer.cpp
	                                      ./viewer_app/GilViewer.h
                                          ./viewer_app/FrameViewer.cpp
                                          ./viewer_app/FrameViewer.hpp
                                          ./viewer_app/GilViewer.rc )
    # Comme c'est sous visual (a priori ...), il y a l'auto link de Boost, donc pas besoin d'ajouter les libs ...
   	TARGET_LINK_LIBRARIES( GilViewerApp ${wxWidgets_LIBRARIES} GilViewer shapelib tinyxml ${TIFF_LIBRARIES} ${JPEG_LIBRARIES} ${PNG_LIBRARIES} ${ZLIB_LIBRARIES} )
ENDIF(WIN32)

#set_target_properties( GilViewerApp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )
