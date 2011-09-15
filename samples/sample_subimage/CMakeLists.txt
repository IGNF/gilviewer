if(UNIX)
        add_executable( sample_subimage sample_subimage_viewer.cpp
                                            sample_subimage_viewer.hpp
                                            sample_subimage.cpp
                                            sample_subimage.hpp )
        target_link_libraries( sample_subimage ${GILVIEWER_LINK_EXTERNAL_LIBRARIES} GilViewer )
endif()
if(WIN32)
    add_executable( sample_subimage WIN32 sample_subimage_viewer.cpp
                                              sample_subimage_viewer.hpp
                                              sample_subimage.cpp
                                              sample_subimage.hpp
                                              sample_subimage.rc )
    target_link_libraries( sample_subimage ${GILVIEWER_LINK_EXTERNAL_LIBRARIES} GilViewer )
endif()
