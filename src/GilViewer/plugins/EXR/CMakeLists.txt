option(USE_EXR "Build GilViewer with OpenEXR file support" OFF)
if(USE_EXR)

#########################################################
# Librairies OpenEXR
##############################################"
FIND_LIBRARY(ILMIMF_LIB
        NAMES
        IlmImf
        PATHS
        /usr/lib
        /usr/local/lib)
IF(ILMIMF_LIB)
        SET(LIBS ${LIBS} ${ILMIMF_LIB})
        #MESSAGE(WARNING ${ILMIMF_LIB})
ELSE(ILMIMF_LIB)
        MESSAGE(FATAL_ERROR " Librairie IlmImf non trouvée ")
ENDIF(ILMIMF_LIB)

FIND_LIBRARY(Z_LIB
        NAMES
        z
        PATHS
        /usr/lib
        /usr/local/lib)
IF(Z_LIB)
        SET(LIBS ${LIBS} ${Z_LIB})
        #MESSAGE(WARNING ${Z_LIB})
ELSE(Z_LIB)
        MESSAGE(FATAL_ERROR " Librairie z non trouvée ")
ENDIF(Z_LIB)

FIND_LIBRARY(ILMT_LIB
        NAMES
        IlmThread
        PATHS
        /usr/lib
        /usr/local/lib)
IF(ILMT_LIB)
        SET(LIBS ${LIBS} ${ILMT_LIB})
        #MESSAGE(WARNING ${ILMT_LIB})
ELSE(ILMT_LIB)
        MESSAGE(FATAL_ERROR " Librairie IlmThread non trouvee ")
ENDIF(ILMT_LIB)

FIND_LIBRARY(MATH_LIB
        NAMES
        Imath
        PATHS
        /usr/lib
        /usr/local/lib)
IF(MATH_LIB)
        SET(LIBS ${LIBS} ${MATH_LIB})
        #MESSAGE(WARNING ${MATH_LIB})
ELSE(MATH_LIB)
        MESSAGE(FATAL_ERROR " Librairie Imath non trouvee ")
ENDIF(MATH_LIB)

FIND_LIBRARY(IEX_LIB
        NAMES
        Iex
        PATHS
        /usr/lib
        /usr/local/lib)
IF(IEX_LIB)
        SET(LIBS ${LIBS} ${IEX_LIB})
ELSE(IEX_LIB)
        MESSAGE(FATAL_ERROR " Librairie Iex non trouvee ")
ENDIF(IEX_LIB)

FIND_LIBRARY(HALF_LIB
        NAMES
        Half
        PATHS
        /usr/lib
        /usr/local/lib)
IF(HALF_LIB)
        SET(LIBS ${LIBS} ${HALF_LIB})
        MESSAGE(STATUS ${LIBS})
ELSE(HALF_LIB)
        MESSAGE(FATAL_ERROR " Librairie Half non trouvée ")
ENDIF(HALF_LIB)

###############################################

    SET(OPENEXR_DIR /usr/include/OpenEXR/)
    INCLUDE_DIRECTORIES ( ${OPENEXR_DIR} )


    file( GLOB SRCS *.c *.cpp *.cxx *.h *.hpp *.hxx )
    add_library(GilViewer-exr SHARED ${SRCS})
    target_link_libraries(GilViewer-exr GilViewer ${LIBS})

    install(TARGETS GilViewer-exr
        EXPORT GilViewer-targets
        DESTINATION ${GILVIEWER_PLUGIN_INSTALL_PREFIX}
        COMPONENT exr )
endif()




