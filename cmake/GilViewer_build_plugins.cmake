set( GILVIEWER_PLUGINS_DIR ${PROJECT_BINARY_DIR} )

if(UNIX)
    set( PLUGINS_EXT "so" )
    set( PLUGINS_PRE "lib" )
elseif(WIN32)
    set( PLUGINS_EXT "dll" )
    set( PLUGINS_PRE "" )
endif()

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/src/GilViewer/config/config_plugins.hpp.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/src/GilViewer/config/config_plugins.hpp )

set(rotate_image_plugin_SRCS ./src/GilViewer/plugins/sample_plugin.hpp ./src/GilViewer/plugins/sample_plugin.cpp )

add_library(rotate_image_plugin SHARED ${rotate_image_plugin_SRCS})
target_link_libraries(rotate_image_plugin GilViewer)


#set(bertrand_plugin_SRCS
#    ./src/GilViewer/plugins/Bertrand_plugin.hpp
#    ./src/GilViewer/plugins/Bertrand_plugin.cpp
#    /home/bcannelle/Programmation/Developpements/SVN/Modele_Donnees/These/code/detectionciel/ImageBinaire.cpp
#    /home/bcannelle/Programmation/Developpements/SVN/Modele_Donnees/These/code/detectionciel/ImageBinaire.h
#    )
#
#add_library(bertrand_plugin SHARED ${bertrand_plugin_SRCS})
#target_link_libraries(bertrand_plugin GilViewer)



#set(ciel_plugin_SRCS
#    ./src/GilViewer/plugins/ciel_plugin.hpp
#    ./src/GilViewer/plugins/ciel_plugin.cpp
#    )
#
#add_library(ciel_plugin SHARED ${ciel_plugin_SRCS})
#target_link_libraries(ciel_plugin GilViewer)
