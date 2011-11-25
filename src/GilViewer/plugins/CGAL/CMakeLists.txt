if(USE_CGAL)
    if(CGAL_FOUND)

        include( ${CGAL_USE_FILE} )

        file( GLOB SRCS *.c *.cpp *.cxx *.h *.hpp *.hxx )
        add_library(GilViewer-CGAL SHARED ${SRCS})
        target_link_libraries(GilViewer-CGAL GilViewer ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})

        install(TARGETS GilViewer-CGAL
            EXPORT GilViewer-targets
            DESTINATION ${GILVIEWER_PLUGIN_INSTALL_PREFIX}
            COMPONENT CGAL )

        INSTALL(FILES cgal_vector_layer.hpp DESTINATION ${GilViewer_INCLUDE_PATH}/plugins/CGAL/ )

    else(CGAL_FOUND)
        message(FATAL_ERROR "CGAL not found!")
    endif(CGAL_FOUND)
endif(USE_CGAL)

