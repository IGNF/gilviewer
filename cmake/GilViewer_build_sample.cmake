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
        endif()
        message(STATUS ${GILVIEWER_LINK_EXTERNAL_LIBRARIES})
        target_link_libraries( GilViewerApp ${GILVIEWER_LINK_EXTERNAL_LIBRARIES}
            GilViewer
            )
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
    # Comme c'est sous visual (a priori ...), il y a l'auto link de Boost, donc pas besoin d'ajouter les libs ...
    target_link_libraries( GilViewerApp ${wxWidgets_LIBRARIES} GilViewer tinyxml ${GDAL_LIBRARY} )
endif()

message( STATUS "Scanning samples :" )
file( GLOB list "samples/*" )
list( SORT list )
foreach( entry ${list} )
  if ( IS_DIRECTORY ${entry} )
    message( STATUS "Configuring  ${entry} sample" )
    if ( EXISTS ${entry}/CMakeLists.txt )
      add_subdirectory( ${entry} )
    endif()
  endif()
endforeach()
