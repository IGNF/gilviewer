####
#### Creation d'un eecutable d'exemple linaknt sur la libITKViewer
####
if(UNIX)
        if(APPLE)
                add_executable( GilViewerApp MACOSX_BUNDLE ./viewer_app/GilViewer.cpp
		                                              ./viewer_app/GilViewer.h
                                                      ./viewer_app/FrameViewer.cpp
                                                      ./viewer_app/FrameViewer.hpp )
        else()
                add_executable( GilViewerApp ./viewer_app/GilViewer.cpp ./viewer_app/FrameViewer.cpp )
                add_executable( sample_vector_layer ./samples/sample_vector_layer/sample_vector_layer_viewer.cpp ./samples/sample_vector_layer/sample_vector_layer.cpp )
        endif()
        add_definitions(-Wall)
        target_link_libraries( GilViewerApp ${wxWidgets_LIBRARIES} ${Boost_LIBRARIES} GilViewer tinyxml ${GDAL_LIBRARY} )
        target_link_libraries( sample_vector_layer ${wxWidgets_LIBRARIES} ${Boost_LIBRARIES} GilViewer tinyxml ${GDAL_LIBRARY} )
endif()
# Si c'est du windows, on lui dit de faire une application windows (et pas une console)
# On en profite aussi pour ajouter les resources
# Et les headers ...
if(WIN32)
	# J'en peux plus de ces warnings ...
        add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
        add_executable( GilViewerApp WIN32 ./viewer_app/GilViewer.cpp
	                                      ./viewer_app/GilViewer.h
                                          ./viewer_app/FrameViewer.cpp
                                          ./viewer_app/FrameViewer.hpp
                                          ./viewer_app/GilViewer.rc )
    # Comme c'est sous visual (a priori ...), il y a l'auto link de Boost, donc pas besoin d'ajouter les libs ...
        target_link_libraries( GilViewerApp ${wxWidgets_LIBRARIES} GilViewer tinyxml ${GDAL_LIBRARY} )
endif()
