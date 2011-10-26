option(USE_RAW_IMAGES_IO "Build GilViewer with RAW images IO file support" ON)
if(USE_RAW_IMAGES_IO)
    file( GLOB SRCS *.c *.cpp *.cxx *.h *.hpp *.hxx )
    add_library(GilViewer-raw_images_io SHARED ${SRCS})
    target_link_libraries(GilViewer-raw_images_io GilViewer raw gomp)

    install(TARGETS GilViewer-raw_images_io
        EXPORT GilViewer-targets
        DESTINATION ${GILVIEWER_PLUGIN_INSTALL_PREFIX}
        COMPONENT raw_images_io )
endif()




