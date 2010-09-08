####
#### Creation d'un executable d'exemple linkant sur la libGilViewer
####
if(UNIX)
        if(APPLE)
                add_executable( GilViewerApp MACOSX_BUNDLE ./viewer_app/gilviewer_app.cpp
                                                           ./viewer_app/gilviewer_app.hpp
                                                           ./viewer_app/gilviewer_frame.cpp
                                                           ./viewer_app/gilviewer_frame.hpp )
        else()
                add_executable( GilViewerApp ./viewer_app/gilviewer_app.cpp
                                             ./viewer_app/gilviewer_app.hpp
                                             ./viewer_app/gilviewer_frame.cpp
                                             ./viewer_app/gilviewer_frame.hpp )
                add_executable( sample_vector_layer ./samples/sample_vector_layer/sample_vector_layer_viewer.cpp
                                                    ./samples/sample_vector_layer/sample_vector_layer_viewer.hpp
                                                    ./samples/sample_vector_layer/sample_vector_layer.cpp
                                                    ./samples/sample_vector_layer/sample_vector_layer.h )
                add_executable( sample_subimage ./samples/sample_subimage/sample_subimage_viewer.cpp
                                                ./samples/sample_subimage/sample_subimage_viewer.hpp
                                                ./samples/sample_subimage/sample_subimage.cpp
                                                ./samples/sample_subimage/sample_subimage.hpp )
        endif()
        target_link_libraries( GilViewerApp ${GILVIEWER_LINK_EXTERNAL_LIBRARIES} GilViewer )
        target_link_libraries( sample_vector_layer ${GILVIEWER_LINK_EXTERNAL_LIBRARIES} GilViewer )
        target_link_libraries( sample_subimage ${GILVIEWER_LINK_EXTERNAL_LIBRARIES} GilViewer )
endif()
# Si c'est du windows, on lui dit de faire une application windows (et pas une console)
# On en profite aussi pour ajouter les resources
# Et les headers ...
if(WIN32)
    add_executable( GilViewerApp WIN32 ./viewer_app/gilviewer_app.cpp
                                       ./viewer_app/gilviewer_app.hpp
                                       ./viewer_app/gilviewer_frame.cpp
                                       ./viewer_app/gilviewer_frame.hpp
                                       ./viewer_app/GilViewer.rc )
    add_executable( sample_vector_layer WIN32 ./samples/sample_vector_layer/sample_vector_layer_viewer.cpp
                                              ./samples/sample_vector_layer/sample_vector_layer_viewer.hpp
                                              ./samples/sample_vector_layer/sample_vector_layer.cpp
                                              ./samples/sample_vector_layer/sample_vector_layer.h
                                              ./samples/sample_vector_layer/sample_vector_layer.rc )
    add_executable( sample_subimage WIN32 ./samples/sample_subimage/sample_subimage_viewer.cpp
                                          ./samples/sample_subimage/sample_subimage_viewer.hpp
                                          ./samples/sample_subimage/sample_subimage.cpp
                                          ./samples/sample_subimage/sample_subimage.hpp
                                          ./samples/sample_subimage/sample_subimage.rc )
    # Comme c'est sous visual (a priori ...), il y a l'auto link de Boost, donc pas besoin d'ajouter les libs ...
    target_link_libraries( GilViewerApp ${wxWidgets_LIBRARIES} GilViewer tinyxml ${GDAL_LIBRARY} )
    target_link_libraries( sample_vector_layer ${wxWidgets_LIBRARIES} GilViewer tinyxml ${GDAL_LIBRARY} )
    target_link_libraries( sample_subimage ${wxWidgets_LIBRARIES} GilViewer tinyxml ${GDAL_LIBRARY} )
endif()
