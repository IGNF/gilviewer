if(USE_GDAL_OGR)
    if(GDAL_FOUND)

        include_directories(${GDAL_INCLUDE_DIR})
        link_directories(${GDAL_LIBRARY_DIRS})
        add_definitions(${GDAL_DEFINITIONS})

        file( GLOB SRCS *.c *.cpp *.cxx *.h *.hpp *.hxx )
        add_library(GilViewer-GDAL SHARED ${SRCS})
        target_link_libraries(GilViewer-GDAL GilViewer ${GDAL_LIBRARY})

        install(TARGETS GilViewer-GDAL
            EXPORT GilViewer-targets
            DESTINATION ${GILVIEWER_PLUGIN_INSTALL_PREFIX}
            COMPONENT GDAL )

        INSTALL(FILES ogr_vector_layer.hpp DESTINATION ${GilViewer_INCLUDE_PATH}/plugins/GDAL/ )

    else(GDAL_FOUND)
        message(FATAL_ERROR "GDAL not found!")
    endif(GDAL_FOUND)
endif(USE_GDAL_OGR)

