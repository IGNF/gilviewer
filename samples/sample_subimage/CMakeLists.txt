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

#copy the data image
add_custom_command(TARGET sample_subimage
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}" ARGS -E copy_if_different \"${CMAKE_SOURCE_DIR}/data/09-Paris.JPG\" \"${CMAKE_CURRENT_BINARY_DIR}\"
)
