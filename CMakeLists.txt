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

IF(UNIX)
	INCLUDE_DIRECTORIES( /usr/local/include /usr/include)
ENDIF(UNIX)

SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib )
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )

SET(CMAKE_CONFIG_DIR cmake)

MESSAGE(STATUS " parent ALL_SHPLIB_INC_FILES " ${ALL_SHPLIB_INC_FILES} )

####
#### On recherche tous les fichiers ".h", ".hpp", ".hxx" et "*.inl"
####
SET(SRC_DIR src/GilViewer)
FILE( GLOB ALL_CONVENIENT_HEADER_FILES ${SRC_DIR}/convenient/*.h ${SRC_DIR}/convenient/*.hpp ${SRC_DIR}/convenient/*.hxx ${SRC_DIR}/convenient/*.inl )
FILE( GLOB ALL_GUI_HEADER_FILES ${SRC_DIR}/gui/*.h ${SRC_DIR}/gui/*.hpp ${SRC_DIR}/gui/*.hxx ${SRC_DIR}/gui/*.inl )
FILE( GLOB ALL_IO_HEADER_FILES ${SRC_DIR}/io/*.h ${SRC_DIR}/io/*.hpp ${SRC_DIR}/io/*.hxx ${SRC_DIR}/io/*.inl )
FILE( GLOB ALL_LAYERS_HEADER_FILES ${SRC_DIR}/layers/*.h ${SRC_DIR}/layers/*.hpp ${SRC_DIR}/layers/*.hxx ${SRC_DIR}/layers/*.inl )
FILE( GLOB ALL_TOOLS_HEADER_FILES ${SRC_DIR}/tools/*.h ${SRC_DIR}/tools/*.hpp ${SRC_DIR}/tools/*.hxx ${SRC_DIR}/tools/*.inl )
FILE( GLOB ALL_VECTORUTILS_HEADER_FILES ${SRC_DIR}/vectorutils/*.h ${SRC_DIR}/vectorutils/*.hpp ${SRC_DIR}/vectorutils/*.hxx ${SRC_DIR}/vectorutils/*.inl )
FILE( GLOB ALL_WRAPPERS_HEADER_FILES ${SRC_DIR}/wrappers/*.h ${SRC_DIR}/wrappers/*.hpp ${SRC_DIR}/wrappers/*.hxx ${SRC_DIR}/wrappers/*.inl )
# Une variable regroupant tous les headers
SET( ALL_VIEWER_HEADERS ${ALL_CONVENIENT_HEADER_FILES}
                        ${ALL_GUI_HEADER_FILES}
                        ${ALL_IO_HEADER_FILES}
                        ${ALL_LAYERS_HEADER_FILES}
                        ${ALL_TOOLS_HEADER_FILES}
                        ${ALL_VECTORUTILS_HEADER_FILES}
                        ${ALL_WRAPPERS_HEADER_FILES} )

#### Definition des dossiers contenant les sources
#### ATTENTION : toutes les sources de ces dossiers seront incluses
#### ATTENTION : si un fichier est ajoute dans un de ces dossiers mais que le CMakeLists.txt n'est pas modifie, il faut relancer le cmake 
####  a la main avant de compiler par make (ca, c'est pas vrai sous windows ! en fait je n'en suis pas si sur ...)
AUX_SOURCE_DIRECTORY(${SRC_DIR}/layers SRC_CALQUES)
AUX_SOURCE_DIRECTORY(${SRC_DIR}/convenient SRC_CONVENIENT)
AUX_SOURCE_DIRECTORY(${SRC_DIR}/gui SRC_GUI)
AUX_SOURCE_DIRECTORY(${SRC_DIR}/tools SRC_OUTILS)
AUX_SOURCE_DIRECTORY(${SRC_DIR}/vectorutils SRC_VECTOR_UTILS)
AUX_SOURCE_DIRECTORY(${SRC_DIR}/io SRC_IO)
AUX_SOURCE_DIRECTORY(${SRC_DIR}/wrappers SRC_WRAPPERS)

# Une variable regroupant toutes les sources ...
SET( ALL_VIEWER_SOURCES ${SRC_CALQUES} ${SRC_CONVENIENT} ${SRC_GUI} ${SRC_OUTILS} ${SRC_VECTOR_UTILS} ${SRC_IO} ${SRC_WRAPPERS} )

if(WIN32)
    if(MSVC)
        #message( STATUS "MSVC_VERSION: " ${MSVC_VERSION} )
        #Visual C++, 32-bit, version 6.0         1200
        #Visual C++, 32-bit, version .net 2002   1300
        #Visual C++, 32-bit, version .net 2003   1310
        #Visual C++, 32-bit, version 2005        1400 (vc80)
        #Visual C++, 32-bit, version 2005 SP1    14?? (vc80_sp1)
        #Visual C++, 32-bit, version 2008        1500 (vc90)

        if(MSVC_VERSION EQUAL 1310 OR MSVC_VERSION EQUAL 1400 OR MSVC_VERSION EQUAL 1500 OR ${MSVC71} )
        else()
			aux_source_directory(${SRC_DIR}/gui/msvc SRC_MSVC)
			set( ALL_VIEWER_SOURCES ${ALL_VIEWER_SOURCES} ${SRC_MSVC} )
        endif()
        
    endif(MSVC)
endif(WIN32)


#####################################
### Recherche des packages necessaires a la compilation ###
#####################################
#Find wxWidgets
if ( WIN32 )
	FIND_PACKAGE(wxWidgets REQUIRED adv base core xml aui xrc html tiff jpeg png)
	IF(wxWidgets_FOUND)
		add_definitions(${wxWidgets_DEFINITIONS})
		INCLUDE(${wxWidgets_USE_FILE})
		include_directories( ${wxWidgets_ROOT_DIR}/src/jpeg )
		include_directories( ${wxWidgets_ROOT_DIR}/src/tiff )
		include_directories( ${wxWidgets_ROOT_DIR}/src/png )
		include_directories( ${wxWidgets_ROOT_DIR}/src/zlib )
	ELSE()
		MESSAGE(FATAL_ERROR "wxWidgets not found ! Please set wxWidgets path ...")
	ENDIF()
else()
	FIND_PACKAGE(wxWidgets REQUIRED adv base core xml aui xrc html )
	IF(wxWidgets_FOUND)  
		add_definitions(${wxWidgets_DEFINITIONS})
		INCLUDE(${wxWidgets_USE_FILE})
	ELSE()
		MESSAGE(FATAL_ERROR "wxWidgets not found ! Please set wxWidgets path ...")
	ENDIF()
endif()
		
# Find BOOST
set( Boost_ADDITIONAL_VERSIONS "1.39.0" "1.39" "1.40.0" "1.40" )
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE( Boost 1.37 COMPONENTS filesystem system thread )
IF( NOT Boost_FOUND )
	MESSAGE(FATAL_ERROR "Boost not found ! Please set Boost path ...")
ELSE()
	INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS} )
	link_directories( ${Boost_LIBRARY_DIRS} )
ENDIF()

####
#### Ajout du fichier contenant les regles de construction des libs externes : shapelib et tinyxml
####
INCLUDE( ${CMAKE_CONFIG_DIR}/GilViewer_build_external_lib.cmake )

#####################################
###                     Fin recherche des packages                         ###
#####################################

### Generation du fichier de resources
if( WIN32 )
	add_custom_command( OUTPUT ${CMAKE_SOURCE_DIR}/${SRC_DIR}/gui/resources/resources.cpp
		                COMMAND ${wxWidgets_wxrc_EXECUTABLE} ${CMAKE_SOURCE_DIR}/${SRC_DIR}/gui/resources/resources.xrc -c -o ${CMAKE_SOURCE_DIR}/${SRC_DIR}/gui/resources/resources.cpp
			            DEPENDS ${CMAKE_SOURCE_DIR}/${SRC_DIR}/gui/resources/resources.xrc )
else()
	add_custom_command( OUTPUT ${CMAKE_SOURCE_DIR}/${SRC_DIR}/gui/resources/resources.cpp
		                COMMAND wxrc ${CMAKE_SOURCE_DIR}/${SRC_DIR}/gui/resources/resources.xrc -c -o ${CMAKE_SOURCE_DIR}/${SRC_DIR}/gui/resources/resources.cpp
			            DEPENDS ${CMAKE_SOURCE_DIR}/${SRC_DIR}/gui/resources/resources.xrc )
endif()

SET( ALL_VIEWER_SOURCES ${ALL_VIEWER_SOURCES} ${CMAKE_SOURCE_DIR}/${SRC_DIR}/gui/resources/resources.cpp )

####
#### Construction de la librarie
####
INCLUDE( ${CMAKE_CONFIG_DIR}/GilViewer_build_library.cmake )

####
#### Construction du sample
####
INCLUDE( ${CMAKE_CONFIG_DIR}/GilViewer_build_sample.cmake )

# Ajout du repertoire d'include des lib externes (pour l'instant,  shapelib et tinyxml)
INCLUDE_DIRECTORIES( BEFORE "extern/" )
# Ajout du repertoire d'include des sources du viewer
INCLUDE_DIRECTORIES( BEFORE  "src/" )

# Ajout d'une dependance du viewer a la shapelib. Ca permet de compiler la shapelib AVANT le viewer ...		
ADD_DEPENDENCIES( GilViewer shapelib )
# Idem pout Tinyxml
ADD_DEPENDENCIES( GilViewer tinyxml )
# Et enfin, le viewer depend de la lib ...
ADD_DEPENDENCIES( GilViewerApp GilViewer )

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

