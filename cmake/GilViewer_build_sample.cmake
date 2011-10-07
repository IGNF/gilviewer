####
#### Creation d'un executable d'exemple linkant sur la libGilViewer
####
set( VIEWER_APP_SOURCES ./viewer_app/gilviewer_app.cpp ./viewer_app/gilviewer_app.hpp ./viewer_app/gilviewer_frame.cpp ./viewer_app/gilviewer_frame.cpp )

set( SYSTEM_NAME "UNRECOGNIZED_SYSTEM" )
if(UNIX)
	if(APPLE)
		set( SYSTEM_NAME MACOSX_BUNDLE )
	else()
		set( SYSTEM_NAME "" )
	endif()
endif()
if(WIN32)
	set( SYSTEM_NAME WIN32 )
	set( VIEWER_APP_SOURCES ${VIEWER_APP_SOURCES} ./viewer_app/GilViewer.rc )
endif()

add_executable( GilViewerApp ${SYSTEM_NAME} ${VIEWER_APP_SOURCES} )
target_link_libraries( GilViewerApp ${GILVIEWER_LINK_EXTERNAL_LIBRARIES} GilViewer )

message( STATUS "*** Scanning samples ***" )
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
message( STATUS "*** Done ***" )