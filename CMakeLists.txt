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
INCLUDE_DIRECTORIES( /usr/local/include /usr/include)

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )

####
#### Ajout du fichier contenant les regles de construction des libs externes : shapelib et tinyxml
####
INCLUDE( GilViewer_build_external_lib.cmake )

####
#### On recherche tous les fichiers ".h", ".hpp", ".hxx" et "*.inl"
####
FILE( GLOB ALL_CONVENIENT_HEADER_FILES src/convenient/*.h src/convenient/*.hpp src/convenient/*.hxx src/convenient/*.inl )
FILE( GLOB ALL_GUI_HEADER_FILES src/gui/*.h src/gui/*.hpp src/gui/*.hxx src/gui/*.inl )
FILE( GLOB ALL_GUI_WXFB_HEADER_FILES src/gui/wxFB/*.h src/gui/wxFB/*.hpp src/gui/wxFB/*.hxx src/gui/wxFB/*.inl )
FILE( GLOB ALL_IO_HEADER_FILES src/convenient/*.h src/convenient/*.hpp src/convenient/*.hxx src/convenient/*.inl )
FILE( GLOB ALL_ITK_HEADER_FILES src/itk/*.h src/itk/*.hpp src/itk/*.hxx src/itk/*.inl )
FILE( GLOB ALL_LAYERS_HEADER_FILES src/layers/*.h src/layers/*.hpp src/layers/*.hxx src/layers/*.inl )
FILE( GLOB ALL_TOOLS_HEADER_FILES src/tools/*.h src/tools/*.hpp src/tools/*.hxx src/tools/*.inl )
FILE( GLOB ALL_VECTORUTILS_HEADER_FILES src/vectorutils/*.h src/vectorutils/*.hpp src/vectorutils/*.hxx src/vectorutils/*.inl )
FILE( GLOB ALL_WRAPPERS_HEADER_FILES src/wrappers/*.h src/wrappers/*.hpp src/wrappers/*.hxx src/wrappers/*.inl )
# Une variable regroupant tous les headers
SET( ALL_VIEWER_HEADERS ${ALL_CONVENIENT_HEADER_FILES}
                        ${ALL_GUI_HEADER_FILES}
                        ${ALL_GUI_WXFB_HEADER_FILES}
                        ${ALL_IO_HEADER_FILES}
                        #${ALL_ITK_HEADER_FILES}
                        ${ALL_LAYERS_HEADER_FILES}
                        #${ALL_PLUGINS_HEADER_FILES}
                        ${ALL_TOOLS_HEADER_FILES}
                        ${ALL_VECTORUTILS_HEADER_FILES}
                        ${ALL_WRAPPERS_HEADER_FILES} )

#### Definition des dossiers contenant les sources
#### ATTENTION : toutes les sources de ces dossiers seront incluses
#### ATTENTION : si un fichier est ajoute dans un de ces dossiers mais que le CMakeLists.txt n'est pas modifie, il faut relancer le cmake 
####  a la main avant de compiler par make (ca, c'est pas vrai sous windows ! en fait je n'en suis pas si sur ...)
SET(SRC_DIR src/)
AUX_SOURCE_DIRECTORY(${SRC_DIR}layers SRC_CALQUES)
AUX_SOURCE_DIRECTORY(${SRC_DIR}convenient SRC_CONVENIENT)
AUX_SOURCE_DIRECTORY(${SRC_DIR}gui SRC_GUI)
AUX_SOURCE_DIRECTORY(${SRC_DIR}tools SRC_OUTILS)
AUX_SOURCE_DIRECTORY(${SRC_DIR}vectorutils SRC_VECTOR_UTILS)
AUX_SOURCE_DIRECTORY(${SRC_DIR}io SRC_IO)
AUX_SOURCE_DIRECTORY(${SRC_DIR}wrappers SRC_WRAPPERS)

# Une variable regroupant toutes les sources ...
SET( ALL_VIEWER_SOURCES
     ${SRC_CALQUES} ${SRC_CONVENIENT} ${SRC_GUI} ${SRC_OUTILS} ${SRC_VECTOR_UTILS} ${SRC_IO} ${SRC_WRAPPERS} )


#####################################
### Recherche des packages necessaires a la compilation ###
#####################################
#Find wxWidgets
FIND_PACKAGE(wxWidgets REQUIRED adv base core xml aui xrc html )
IF(wxWidgets_FOUND)  
	INCLUDE(${wxWidgets_USE_FILE})
ELSE()
	# For convenience. When we cannot continue, inform the user
	MESSAGE(FATAL_ERROR "wxWidgets not found ! Please set wxWidgets path ...")
ENDIF()
		
# Find BOOST
set( Boost_ADDITIONAL_VERSIONS "1.39.0" "1.39" "1.40.0" "1.40" )
FIND_PACKAGE( Boost 1.34.1 COMPONENTS filesystem system thread )
IF( NOT Boost_FOUND )
	MESSAGE(FATAL_ERROR "Boost not found ! Please set Boost path ...")
ELSE()
	INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS} )
	link_directories( ${Boost_LIBRARY_DIRS} )
ENDIF()

# Find TIFF
find_package( TIFF )
if( NOT TIFF_FOUND )
	message( FATAL_ERROR "libTIFF not found ! Please set tiff paths ..." )
else()
	include_directories( ${TIFF_INCLUDE_DIR} )
endif()

# Find JPEG
find_package( JPEG )
if( NOT JPEG_FOUND )
	message( FATAL_ERROR "libJPEG not found ! Please set jpeg paths ..." )
else()
	include_directories( ${JPEG_INCLUDE_DIR} )
endif()

# Find PNG
find_package( PNG )
if( NOT PNG_FOUND )
	message( FATAL_ERROR "libPNG not found ! Please set png paths ..." )
else()
	include_directories( ${PNG_INCLUDE_DIR} )
endif()
#####################################
###                     Fin recherche des packages                         ###
#####################################

### Generation du fichier de resources
if( WIN32 )
	add_custom_command( OUTPUT ${CMAKE_SOURCE_DIR}/${SRC_DIR}gui/resources/resources.cpp
		                COMMAND ${wxWidgets_wxrc_EXECUTABLE} ${CMAKE_SOURCE_DIR}/src/gui/resources/resources.xrc -v -c -o ${CMAKE_SOURCE_DIR}/src/gui/resources/resources.cpp
			            DEPENDS ${CMAKE_SOURCE_DIR}/src/gui/resources/resources.xrc )
else()
	add_custom_command( OUTPUT ${CMAKE_SOURCE_DIR}/${SRC_DIR}gui/resources/resources.cpp
		                COMMAND wxrc ${CMAKE_SOURCE_DIR}/src/gui/resources/resources.xrc -v -c -o ${CMAKE_SOURCE_DIR}/src/gui/resources/resources.cpp
			            DEPENDS ${CMAKE_SOURCE_DIR}/src/gui/resources/resources.xrc )
endif()

SET( ALL_VIEWER_SOURCES ${ALL_VIEWER_SOURCES} ${CMAKE_SOURCE_DIR}/${SRC_DIR}gui/resources/resources.cpp )

####
#### Construction de la librarie
####
INCLUDE( GilViewer_build_library.cmake )

####
#### Construction du sample
####
INCLUDE( GilViewer_build_sample.cmake )

# Ajout du repertoire d'include des lib externes (pour l'instant,  shapelib et tinyxml)
INCLUDE_DIRECTORIES( ${INCLUDE_DIRECTORIES} "extern/" )
# Ajout du repertoire d'include des sources du viewer
INCLUDE_DIRECTORIES( ${INCLUDE_DIRECTORIES} "src/" )

# Ajout d'une dependance du viewer a la shapelib. Ca permet de compiler la shapelib AVANT le viewer ...		
ADD_DEPENDENCIES( GilViewer shapelib )
# Idem pout Tinyxml
ADD_DEPENDENCIES( GilViewer tinyxml )
# Et enfin, le viewer depend de la lib ...
ADD_DEPENDENCIES( GilViewerApp GilViewer )
