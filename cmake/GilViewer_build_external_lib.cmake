##################
### Build de la ShapeLib ###
##################
if( UNIX )
	# Option pour selectionner une compilation sous forme de bibliotheque statique ou dynamique
	OPTION(BUILD_SHAPELIB_SHARED "Build shapelib library shared." ON)
	# Verification du type de bibliotheque a construire
	IF(BUILD_SHAPELIB_SHARED)
		SET(SHAPELIB_LIBRARY_TYPE SHARED)
	ELSE()
		SET(SHAPELIB_LIBRARY_TYPE STATIC)
	ENDIF()
# Currently, dll are not supported on Windows platforms
else()
	set( SHAPELIB_LIBRARY_TYPE STATIC )
endif()

# On fixe une variable globale regroupant tous les fichiers source de la shapelib
# tout les autre fichier *.c sont des applications example
SET( SRC_SHPLIB extern/shapelib )
SET( ALL_SHPLIB_SRC_FILES	${SRC_SHPLIB}/dbfopen.c
							${SRC_SHPLIB}/shpopen.c
							${SRC_SHPLIB}/shptree.c
)

SET( ALL_SHPLIB_INC_FILES  "${SRC_SHPLIB}/shapefil.h" )
MESSAGE(STATUS " create ALL_SHPLIB_INC_FILES " ${ALL_SHPLIB_INC_FILES} )

# On ajoute les repertoires d'include
INCLUDE_DIRECTORIES( ${INCLUDE_DIRECTORIES} ${SRC_SHPLIB} )
# Creation de la lib
ADD_LIBRARY( shapelib ${SHAPELIB_LIBRARY_TYPE} ${ALL_SHPLIB_SRC_FILES} ${ALL_SHPLIB_INC_FILES})

##################
###      Fin ShapeLib       ###
##################

##################
###   Build de TinyXML  ###
##################
add_definitions(-DTIXML_USE_STL)
if( UNIX )
	# Option pour selectionner une compilation sous forme de .lib ou de .dll
	OPTION(BUILD_TINYXML_SHARED "Build tinyxml library shared." ON)
	# Verification du type de bibliotheque a construire
	IF(BUILD_TINYXML_SHARED)
	SET(TINYXML_LIBRARY_TYPE SHARED)
	ELSE()
		SET(TINYXML_LIBRARY_TYPE STATIC)
	ENDIF()
else()
# Currently, dll are not supported on Windows platforms
	set( TINYXML_LIBRARY_TYPE STATIC )
endif()

# On fixe une variable globale regroupant tous les fichiers source de tinyxml
SET( SRC_TINYXMLLIB extern/tinyxml )
SET( ALL_TINYXML_SRC_FILES ${SRC_TINYXMLLIB}/tinystr.cpp 
							${SRC_TINYXMLLIB}/tinyxml.cpp 
							${SRC_TINYXMLLIB}/tinyxmlerror.cpp 
							${SRC_TINYXMLLIB}/tinyxmlparser.cpp 
)
SET( ALL_TINYXML_INC_FILES ${SRC_TINYXMLLIB}/tinystr.h 
							${SRC_TINYXMLLIB}/tinyxml.h )
		   
# On ajoute les repertoires d'include
INCLUDE_DIRECTORIES( ${SRC_TINYXMLLIB} )
INCLUDE_DIRECTORIES( extern )

# Currently, dll are not supported on Windows platforms
if( WIN32 )
	set( TINYXML_LIBRARY_TYPE STATIC )
endif()

# Creation de la lib
ADD_LIBRARY( tinyxml ${TINYXML_LIBRARY_TYPE} ${ALL_TINYXML_SRC_FILES} ${ALL_TINYXML_INC_FILES})

############################
###      Fin TinyXML     ###
############################


##################
###   Extension GIL du MATIS  ###
##################
SET( INC_BOOST extern/boost )
INCLUDE_DIRECTORIES( BEFORE ${INC_BOOST})



#add_custom_command(
#	TARGET install
#	PRE_BUILD
#	COMMAND ${CMAKE_COMMAND} -E make_directory ${DOCS_DIR}/trac
#)
