if(UNIX)
        add_executable( sample_vector_layer sample_vector_layer_viewer.cpp
                                            sample_vector_layer_viewer.hpp
                                            sample_vector_layer.cpp
                                            sample_vector_layer.hpp )
        target_link_libraries( sample_vector_layer ${GILVIEWER_LINK_EXTERNAL_LIBRARIES} GilViewer )
endif()
if(WIN32)
    add_executable( sample_vector_layer WIN32 sample_vector_layer_viewer.cpp
                                              sample_vector_layer_viewer.hpp
                                              sample_vector_layer.cpp
                                              sample_vector_layer.hpp
                                              sample_vector_layer.rc )
    target_link_libraries( sample_vector_layer ${GILVIEWER_LINK_EXTERNAL_LIBRARIES} GilViewer )
endif()
