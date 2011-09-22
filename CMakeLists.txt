CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

################################################
##
## Pour avoir une version release, utiliser la ligne de commande suivante :
##    (c)cmake . -DCMAKE_BUILD_TYPE:STRING=Release
## Pour la version debug (par defaut ...), je vous laisse deviner ;-)
##
################################################


#Nom du projet
PROJECT(GilViewer)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/src )

set(BUILD_SHARED_LIBS ON)
message( STATUS ${CMAKE_BUILD_TYPE} )
 
IF(UNIX) 
        add_definitions(-Wall -fPIC)
        SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG" CACHE STRING "Flags used by the compiler during debug builds." FORCE)
        #add_definitions(-pedantic)
ENDIF(UNIX) 

SET(CMAKE_CONFIG_DIR cmake)

####
#### On recherche tous les fichiers ".h", ".hpp", ".hxx" et "*.inl"
####
SET(SRC_DIR src/GilViewer)
FILE( GLOB ALL_CONFIG_HEADER_FILES ${SRC_DIR}/config/*.hpp )
FILE( GLOB ALL_CONVENIENT_HEADER_FILES ${SRC_DIR}/convenient/*.hpp )
FILE( GLOB_RECURSE ALL_GUI_HEADER_FILES ${SRC_DIR}/gui/*.hpp ${SRC_DIR}/gui/*.fbp )
FILE( GLOB ALL_IO_HEADER_FILES ${SRC_DIR}/io/*.hpp )
FILE( GLOB ALL_LAYERS_HEADER_FILES ${SRC_DIR}/layers/*.hpp )
FILE( GLOB ALL_TOOLS_HEADER_FILES ${SRC_DIR}/tools/*.hpp )
FILE( GLOB ALL_PLUGINS_HEADER_FILES ${SRC_DIR}/plugins/*.hpp )
# Une variable regroupant tous les headers
SET( ALL_VIEWER_HEADERS ${ALL_CONFIG_HEADER_FILES}
                        ${ALL_CONVENIENT_HEADER_FILES}
                        ${ALL_GUI_HEADER_FILES}
                        ${ALL_IO_HEADER_FILES}
                        ${ALL_LAYERS_HEADER_FILES}
                        ${ALL_TOOLS_HEADER_FILES}
                        ${ALL_PLUGINS_HEADER_FILES} )

#### Definition des dossiers contenant les sources
#### ATTENTION : toutes les sources de ces dossiers seront incluses
#### ATTENTION : si un fichier est ajoute dans un de ces dossiers mais que le CMakeLists.txt n'est pas modifie, il faut relancer le cmake
####  a la main avant de compiler par make (ca, c'est pas vrai sous windows ! en fait je n'en suis pas si sur ...)
AUX_SOURCE_DIRECTORY(${SRC_DIR}/config SRC_CONFIG)
AUX_SOURCE_DIRECTORY(${SRC_DIR}/convenient SRC_CONVENIENT)
AUX_SOURCE_DIRECTORY(${SRC_DIR}/gui SRC_GUI)
AUX_SOURCE_DIRECTORY(${SRC_DIR}/gui/wxfb SRC_GUI_WXFB)
AUX_SOURCE_DIRECTORY(${SRC_DIR}/io SRC_IO)
AUX_SOURCE_DIRECTORY(${SRC_DIR}/layers SRC_CALQUES)
AUX_SOURCE_DIRECTORY(${SRC_DIR}/tools SRC_OUTILS) 
#AUX_SOURCE_DIRECTORY(${SRC_DIR}/plugins SRC_PLUGINS)
set(SRC_PLUGINS ${SRC_DIR}/plugins/plugin_base.cpp ${SRC_DIR}/plugins/plugin_manager.cpp)

# Une variable regroupant toutes les sources ...
SET( ALL_VIEWER_SOURCES ${SRC_CONFIG} ${SRC_CONVENIENT} ${SRC_GUI} ${SRC_GUI_WXFB} ${SRC_IO} ${SRC_CALQUES} ${SRC_OUTILS} ${SRC_PLUGINS} )

if(WIN32)
    if(MSVC)
        # Remove gil iterator related warnings...
        add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
        add_definitions(-D_SCL_SECURE_NO_WARNINGS)
        #message( STATUS "MSVC_VERSION: " ${MSVC_VERSION} )
        #Visual C++, 32-bit, version 6.0         1200
        #Visual C++, 32-bit, version .net 2002   1300
        #Visual C++, 32-bit, version .net 2003   1310
        #Visual C++, 32-bit, version 2005        1400 (vc80)
        #Visual C++, 32-bit, version 2005 SP1    14?? (vc80_sp1)
        #Visual C++, 32-bit, version 2008        1500 (vc90)
        #MSVC90  (Visual C++ 2008 SP1) MSVC_VERSION=1500 (Compiler  Version 15.00.30729.01)
        #MSVC100 (Visual C++ 2010) MSVC_VERSION=1600 (Compiler  Version 16.00.30319.01)
		#MESSAGE(STATUS " MSVC 71 " ${MSVC71})
        if(MSVC_VERSION EQUAL 1310 OR MSVC_VERSION EQUAL 1400 OR MSVC_VERSION EQUAL 1500 OR MSVC_VERSION EQUAL 1600 )
        else()
            aux_source_directory(${SRC_DIR}/gui/msvc SRC_MSVC)
            set( ALL_VIEWER_SOURCES ${ALL_VIEWER_SOURCES} ${SRC_MSVC} )
        endif()
        if(MSVC_VERSION EQUAL 1600)
            add_definitions( -D_HAS_CPP0X=0)
        endif()
    endif(MSVC)
endif(WIN32)


###########################################################
### Recherche des packages necessaires a la compilation ###
###########################################################
if(WIN32)
    set(CMAKE_PREFIX_PATH "C:/Program Files/MATIS/")
endif(WIN32)

find_package(JPEG REQUIRED)
if(JPEG_FOUND)
    include_directories(${JPEG_INCLUDE_DIR})
else()
    message(FATAL_ERROR "JPEG not found ! Please set JPEG path ...")
endif()

if(NOT WIN32)
    find_package(PNG REQUIRED)
    if(PNG_FOUND)
        include_directories(${PNG_INCLUDE_DIR})
    else()
        message(FATAL_ERROR "PNG not found ! Please set PNG path ...")
    endif()

    find_package(ZLIB REQUIRED)
    if(ZLIB_FOUND)
        include_directories(${ZLIB_INCLUDE_DIR})
    else()
        message(FATAL_ERROR "ZLIB not found ! Please set ZLIB path ...")
    endif()

    #Find wxWidgets
    set(wxWidgets_EXCLUDE_COMMON_LIBRARIES TRUE)
        find_package(wxWidgets REQUIRED adv base core xml aui xrc html)
    if(wxWidgets_FOUND)
        include(${wxWidgets_USE_FILE})
    else()
        message(FATAL_ERROR "wxWidgets not found ! Please set wxWidgets path ...")
    endif()
else()
    #Find wxWidgets
    set(wxWidgets_EXCLUDE_COMMON_LIBRARIES TRUE)
    set(wxWidgets_USE_STATIC OFF)
    find_package(wxWidgets REQUIRED adv base core xml aui xrc html zlib png expat)
    if(wxWidgets_FOUND)
        message (STATUS "wxWidgets_DEFINITIONS " "${wxWidgets_DEFINITIONS}")
        #add_definitions(${wxWidgets_DEFINITIONS})
        # not set definitions like that because on MSVC 2010 this add wrong terme on AdditionalOptions flags
        include(${wxWidgets_USE_FILE})
        include_directories( ${wxWidgets_ROOT_DIR}/src/png )
        include_directories( ${wxWidgets_ROOT_DIR}/src/zlib )
    else()
        message(FATAL_ERROR "wxWidgets not found ! Please set wxWidgets path ...")
    endif()
endif()

# Find BOOST
set( Boost_ADDITIONAL_VERSIONS 
           "1.39.0" "1.39" 
           "1.40.0" "1.40" 
           "1.41.0" "1.41" 
           "1.42.0" "1.42" 
            "1.43.0" "1.43" 
            "1.44.0" "1.44"
            "1.46" "1.46.1"
           "1.47" "1.47.0"
     )
set(Boost_USE_STATIC_LIBS OFF CACHE BOOL "use boost static lib")
set(Boost_USE_MULTITHREAD ON CACHE BOOL "use boost multi thread lib")
set(BOOST_ROOT "" CACHE PATH "path to boost root directory")
set(Boost_NO_SYSTEM_PATHS TRUE CACHE BOOL "use boost path env variable")
FIND_PACKAGE( Boost 1.37 COMPONENTS filesystem system thread serialization )
IF( NOT Boost_FOUND )
    MESSAGE(FATAL_ERROR "Boost not found ! Please set Boost path ...")
ELSE()
        INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS} )
    link_directories( ${Boost_LIBRARY_DIRS} )
	if( WIN32 )
		add_definitions( -DBOOST_ALL_NO_LIB )
	endif()
ENDIF()

####
#### Ajout du fichier contenant les regles de construction des libs externes : tinyxml
####
INCLUDE( ${CMAKE_CONFIG_DIR}/GilViewer_build_external_lib.cmake )

#####################################
###                     recherche des packages optionnels                        ###
#####################################

find_package(ImageIO)
option(USE_ImageIO "Build GilViewer with ImageIO" ImageIO_FOUND)

#####################################
###                     Fin recherche des packages                         ###
#####################################

### Generation du fichier de resources
if( WIN32 )
    set(wxrc_EXECUTABLE ${wxWidgets_wxrc_EXECUTABLE})
else()
    set(wxrc_EXECUTABLE wxrc)
endif()
add_custom_command( OUTPUT ${CMAKE_SOURCE_DIR}/${SRC_DIR}/gui/resources/resources.cpp
                    COMMAND ${wxrc_EXECUTABLE} ${CMAKE_SOURCE_DIR}/${SRC_DIR}/gui/resources/resources.xrc -c -o ${CMAKE_SOURCE_DIR}/${SRC_DIR}/gui/resources/resources.cpp
                    DEPENDS ${CMAKE_SOURCE_DIR}/${SRC_DIR}/gui/resources/resources.xrc )

SET( ALL_VIEWER_SOURCES ${ALL_VIEWER_SOURCES} ${CMAKE_SOURCE_DIR}/${SRC_DIR}/gui/resources/resources.cpp )

####
#### Construction de la librarie
####
INCLUDE( ${CMAKE_CONFIG_DIR}/GilViewer_build_library.cmake )

####
#### Construction du sample
####
INCLUDE( ${CMAKE_CONFIG_DIR}/GilViewer_build_sample.cmake )

####
#### Construction des plugins
####
INCLUDE( ${CMAKE_CONFIG_DIR}/GilViewer_build_plugins.cmake )

# Ajout du repertoire d'include des lib externes (pour l'instant,  tinyxml)
INCLUDE_DIRECTORIES( BEFORE "extern/" )
# Ajout du repertoire d'include des sources du viewer
INCLUDE_DIRECTORIES( BEFORE  "src/" )

# Ajout d'une dependance du viewer a tinyxml. Ca permet de compiler tinyxml AVANT le viewer ...
ADD_DEPENDENCIES( GilViewer tinyxml )
# Et enfin, le viewer depend de la lib ...
ADD_DEPENDENCIES( GilViewerApp GilViewer )
# De meme que les samples ...
ADD_DEPENDENCIES( sample_vector_layer GilViewer )
ADD_DEPENDENCIES( sample_subimage GilViewer )

####
#### Choice for install mode.
####     - None : no install target
####     - Install : create a install target
####     - Package : create a packake
SET(INSTALL_MODE "Install" CACHE STRING "Choose install mode between : None, Install, Package")
IF(${INSTALL_MODE} STREQUAL "Install")
    # If an install target is selected, propose to install langage files
    set( LANGAGES_SUPPORT_FILES "" )
    # Currently available langages are: French
    option( INSTALL_LANGAGE_FRENCH "Install french langage support" ON )
        set( LANGAGES_FR_SUPPORT_FILES ${CMAKE_SOURCE_DIR}/languages/fr/libGilViewer.mo ${CMAKE_SOURCE_DIR}/languages/fr/GilViewerApp.mo )
    INCLUDE( ${CMAKE_CONFIG_DIR}/GilViewer_install.cmake )
    MESSAGE( STATUS " Install file include " )
ENDIF(${INSTALL_MODE} STREQUAL "Install")
IF(${INSTALL_MODE} STREQUAL "Package")
    INCLUDE( ${CMAKE_CONFIG_DIR}/GilViewer_package.cmake )
    MESSAGE( STATUS " Install file package" )
ENDIF(${INSTALL_MODE} STREQUAL "Package")

