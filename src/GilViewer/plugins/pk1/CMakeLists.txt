option(USE_PK1 "Build GilViewer with PK1 file support" ON)
if(USE_PK1)
    file( GLOB SRCS *.c *.cpp *.cxx *.h *.hpp *.hxx )
    add_library(GilViewer-pk1 SHARED ${SRCS})
    target_link_libraries(GilViewer-pk1 GilViewer)

    install(TARGETS GilViewer-pk1
        EXPORT GilViewer-targets
        DESTINATION ${GILVIEWER_PLUGIN_INSTALL_PREFIX}
        COMPONENT pk1 )
endif()




