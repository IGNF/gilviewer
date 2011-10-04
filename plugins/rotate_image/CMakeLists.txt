file( GLOB SRCS *.c *.cpp *.cxx *.h *.hpp *.hxx )
add_library(GilViewer-rotate_image SHARED ${SRCS})
target_link_libraries(GilViewer-rotate_image GilViewer)

install(TARGETS GilViewer-rotate_image
   EXPORT GilViewer-targets
   DESTINATION ${GILVIEWER_PLUGIN_INSTALL_PREFIX}
   COMPONENT rotate_image
   )
