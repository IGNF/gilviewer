set( GILVIEWER_PLUGINS_DIR ${PROJECT_BINARY_DIR} )

if(UNIX)
    set( PLUGINS_EXT "so" )
    set( PLUGINS_PRE "lib" )
elseif(WIN32)
    set( PLUGINS_EXT "dll" )
    set( PLUGINS_PRE "" )
endif()

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/src/GilViewer/config/config_plugins.hpp.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/src/GilViewer/config/config_plugins.hpp )

set(sample_plugin_SRCS ./src/GilViewer/plugins/sample_plugin.hpp ./src/GilViewer/plugins/sample_plugin.cpp )

add_library(sample_plugin SHARED ${sample_plugin_SRCS})
target_link_libraries(sample_plugin GilViewer)

