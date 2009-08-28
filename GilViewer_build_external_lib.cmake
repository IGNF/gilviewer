##################
### Build de la ShapeLib ###
##################
# Option pour selectionner une compilation sous forme de bibliotheque statique ou dynamique
OPTION(BUILD_SHAPELIB_SHARED "Build shapelib library shared." ON)

# Verification du type de bibliotheque a construire
IF(BUILD_SHAPELIB_SHARED)
  SET(SHAPELIB_LIBRARY_TYPE SHARED)
ELSE(BUILD_SHAPELIB_SHARED)
  SET(SHAPELIB_LIBRARY_TYPE STATIC)
ENDIF(BUILD_SHAPELIB_SHARED)

# On fixe une variable globale regroupant tous les fichiers source de la shapelib
SET( SRC_SHPLIB extern/shapelib )
SET( ALL_SHPLIB_SRC_FILES ${SRC_SHPLIB}/shpopen.c ${SRC_SHPLIB}/dbfopen.c ${SRC_SHPLIB}/shptree.c )
		   
# On ajoute les repertoires d'include
INCLUDE_DIRECTORIES( ${INCLUDE_DIRECTORIES} ${SRC_SHPLIB} )
# Creation de la lib
ADD_LIBRARY( shapelib ${SHAPELIB_LIBRARY_TYPE} ${ALL_SHPLIB_SRC_FILES} )

set_target_properties( shapelib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )
set_target_properties( shapelib PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )

##################
###      Fin ShapeLib       ###
##################

##################
###   Build de TinyXML  ###
##################

# Option pour selectionner une compilation sous forme de .lib ou de .dll
OPTION(BUILD_TINYXML_SHARED "Build tinyxml library shared." ON)

# Verification du type de bibliotheque a construire
IF(BUILD_TINYXML_SHARED)
  SET(TINYXML_LIBRARY_TYPE SHARED)
ELSE(BUILD_TINYXML_SHARED)
  SET(TINYXML_LIBRARY_TYPE STATIC)
ENDIF(BUILD_TINYXML_SHARED)

# On fixe une variable globale regroupant tous les fichiers source de tinyxml
SET( SRC_TINYXMLLIB extern/tinyxml )
SET( ALL_TINYXML_SRC_FILES ${SRC_TINYXMLLIB}/tinystr.cpp ${SRC_TINYXMLLIB}/tinystr.h ${SRC_TINYXMLLIB}/tinyxml.cpp ${SRC_TINYXMLLIB}/tinyxml.h ${SRC_TINYXMLLIB}/tinyxmlerror.cpp ${SRC_TINYXMLLIB}/tinyxmlparser.cpp )
		   
# On ajoute les repertoires d'include
INCLUDE_DIRECTORIES( ${SRC_TINYXMLLIB} )
# Creation de la lib
ADD_LIBRARY( tinyxml ${TINYXML_LIBRARY_TYPE} ${ALL_TINYXML_SRC_FILES} )

set_target_properties( tinyxml PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )
set_target_properties( tinyxml PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )

##################
###      Fin TinyXML       ###
##################
