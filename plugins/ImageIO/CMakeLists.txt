if(USE_ImageIO)
    if(ImageIO_FOUND)

        include_directories(${ImageIO_INCLUDE_DIRS})
        link_directories(${ImageIO_LIBRARY_DIRS})
        add_definitions(${ImageIO_DEFINITIONS})

        file( GLOB SRCS *.c *.cpp *.cxx *.h *.hpp *.hxx )
        add_library(GilViewer-ImageIO SHARED ${SRCS})
        target_link_libraries(GilViewer-ImageIO GilViewer ${ImageIO_LIBRARIES})

    else()
        message(FATAL_ERROR "ImageIO not found!")
    endif()
endif()




