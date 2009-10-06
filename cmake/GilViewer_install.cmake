#################################################
###           INSTALL 						  ###
#################################################
GET_FILENAME_COMPONENT(GILVIEWER_CMAKE_SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

IF(UNIX)
	SET(INSTALL_PREFIX "/usr/local" CACHE PATH " install path" )
	SET(GilViewer_INSTALL_PATH ${INSTALL_PREFIX})
	SET(GilViewer_INCLUDE_PATH ${INSTALL_PREFIX}/include/GilViewer)
	SET(shapelib_INCLUDE_PATH ${INSTALL_PREFIX}/include/shapelib)
	SET(tinyxml_INCLUDE_PATH ${INSTALL_PREFIX}/include/tinyxml)
	SET(RELATIVE_INCLUDE_PATH "../../include/GilViewer")
	SET(RELATIVE_INCLUDE_PATH_TINYXML "../../include/tinyxml")
	SET(RELATIVE_INCLUDE_PATH_SHAPELIB "../../include/shapelib")
	SET(TARGET_CONFIG_INSTALL_PATH ${INSTALL_PREFIX}/lib/GilViewer)
ENDIF(UNIX)
IF(WIN32)
	SET(GilViewer_INSTALL_DIR "GilViewer")
	SET(INSTALL_PREFIX "C:/Program Files/MATIS" CACHE PATH " install path")
	SET(GilViewer_INSTALL_PATH ${INSTALL_PREFIX}/${GilViewer_INSTALL_DIR})
	SET(GilViewer_INCLUDE_PATH ${INSTALL_PREFIX}/${GilViewer_INSTALL_DIR}/include)
	SET(shapelib_INCLUDE_PATH ${GilViewer_INSTALL_PATH}/../shapelib/include)
	SET(tinyxml_INCLUDE_PATH ${GilViewer_INSTALL_PATH}/../tinyxml/include)
	SET(RELATIVE_INCLUDE_PATH "../GilViewer/include")
	SET(RELATIVE_INCLUDE_PATH_TINYXML "../tinyxml/include")
	SET(RELATIVE_INCLUDE_PATH_SHAPELIB "../shapelib/include")
	SET(TARGET_CONFIG_INSTALL_PATH ${INSTALL_PREFIX}/cmake)
ENDIF(WIN32)
SET(CMAKE_INSTALL_PREFIX ${INSTALL_PREFIX} CACHE INTERNAL  " real install path" FORCE)

SET(export_lib "")
SET(GilViewer_SHAPELIB_SHARED ON)
SET(GilViewer_TINYXML_SHARED ON)

IF( BUILD_GILVIEWERLIB_SHARED )
	MESSAGE(STATUS " INSTALL : libGilViewer Shared ")
	SET( export_lib GilViewer )
ELSE(BUILD_GILVIEWERLIB_SHARED)
	MESSAGE(STATUS " INSTALL : libGilViewer Static ")
ENDIF(BUILD_GILVIEWERLIB_SHARED)

IF( BUILD_SHAPELIB_SHARED )
	MESSAGE(STATUS " INSTALL : libshapelib Shared ")
	SET( export_lib ${export_lib} shapelib )
	MESSAGE(STATUS " ALL_SHPLIB_INC_FILES" ${ALL_SHPLIB_INC_FILES})
	INSTALL(FILES ${ALL_SHPLIB_INC_FILES} DESTINATION ${shapelib_INCLUDE_PATH})
ELSE(BUILD_SHAPELIB_SHARED)
	MESSAGE(STATUS " INSTALL : libshapelib Static ")
	SET(GilViewer_SHAPELIB_SHARED OFF)
ENDIF(BUILD_SHAPELIB_SHARED)

IF( BUILD_TINYXML_SHARED )
	MESSAGE(STATUS " INSTALL : libtinyxml Shared ")
	SET( export_lib ${export_lib} tinyxml )
	MESSAGE(STATUS " ALL_TINYXML_INC_FILES" ${ALL_TINYXML_INC_FILES})
	INSTALL(FILES ${ALL_TINYXML_INC_FILES} DESTINATION ${tinyxml_INCLUDE_PATH})
ELSE(BUILD_TINYXML_SHARED)
	MESSAGE(STATUS " INSTALL : libtinyxml Static ")
	SET(GilViewer_TINYXML_SHARED OFF)
ENDIF(BUILD_TINYXML_SHARED)



CONFIGURE_FILE(${GILVIEWER_CMAKE_SELF_DIR}/GilViewerConfig.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/GilViewerConfig.cmake @ONLY)
#install header of gilviewer
INSTALL(FILES ${ALL_CONVENIENT_HEADER_FILES} DESTINATION ${GilViewer_INCLUDE_PATH}/convenient)
INSTALL(FILES ${ALL_GUI_HEADER_FILES} DESTINATION ${GilViewer_INCLUDE_PATH}/gui )
INSTALL(FILES ${ALL_GUI_WXFB_HEADER_FILES} DESTINATION ${GilViewer_INCLUDE_PATH}/gui/wxFB )
INSTALL(FILES ${ALL_IO_HEADER_FILES} DESTINATION ${GilViewer_INCLUDE_PATH}/io )
INSTALL(FILES ${ALL_LAYERS_HEADER_FILES} DESTINATION ${GilViewer_INCLUDE_PATH}/layers )
INSTALL(FILES ${ALL_TOOLS_HEADER_FILES} DESTINATION ${GilViewer_INCLUDE_PATH}/tools )
INSTALL(FILES ${ALL_VECTORUTILS_HEADER_FILES} DESTINATION ${GilViewer_INCLUDE_PATH}/vectorutils )
INSTALL(FILES ${ALL_WRAPPERS_HEADER_FILES} DESTINATION ${GilViewer_INCLUDE_PATH}/wrappers )
#install lib and export target
INSTALL (TARGETS ${export_lib} EXPORT GilViewer-targets DESTINATION ${GilViewer_INSTALL_PATH}/lib COMPONENT library ) 

#install config file
INSTALL (EXPORT GilViewer-targets DESTINATION ${TARGET_CONFIG_INSTALL_PATH} )
INSTALL (FILES GilViewerConfig.cmake DESTINATION ${TARGET_CONFIG_INSTALL_PATH})

MESSAGE(STATUS " test  GIL_MATIS " ${GIL_MATIS})
IF(NOT GIL_MATIS)
	MESSAGE(STATUS " install gil matis ")
	INSTALL(CODE "
		 MESSAGE(STATUS \"install gil matis extension\")
   		 EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_directory  
		 		\"${INC_BOOST}/gil/extension/matis\" 
				\"${Boost_INCLUDE_DIRS}/boost/gil/extension\" )
		" )
	# extensions io 
	FIND_PATH(GIL_IO dynamic_io.hpp
			PATH ${Boost_INCLUDE_DIRS}/boost/gil/extension/io
	)
	IF(NOT GIL_IO )
		MESSAGE(STATUS " gil io not found : copy io to " ${Boost_INCLUDE_DIRS}/boost/gil/extension/io)
		INSTALL(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory  \"${INC_BOOST}/gil/extension/io\" \"\${Boost_INCLUDE_DIRS}/boost/gil/extensionio\" )" )
	ELSE(NOT GIL_IO)
		MESSAGE(STATUS " gil io found : change file if different " ${INC_BOOST}/boost/gil/extension/io)
		FILE( GLOB IO_FILES ${INC_BOOST}/gil/extension/io/*.hpp)
		MESSAGE(STATUS " IO_FILES " ${IO_FILES} )
		FOREACH(FILE ${IO_FILES} )
			MESSAGE(STATUS " io file " ${FILE})
			GET_FILENAME_COMPONENT(NAME_FILE  "${FILE}" NAME)
			INSTALL(CODE  "execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different \"${FILE}\" \"${Boost_INCLUDE_DIRS}/boost/gil/extension/io/${NAME_FILE}\") ")
		ENDFOREACH(FILE ${IO_FILES} )
	ENDIF(NOT GIL_IO )
	
	# extensions dynamic_image 
	FIND_PATH(GIL_DYNAMIC_IMAGE any_image.hpp
			PATH ${Boost_INCLUDE_DIRS}/boost/gil/extension/dynamic_image
	)
	IF(NOT GIL_DYNAMIC_IMAGE)
		MESSAGE(STATUS " gil dynamic_image not found : copy io to " ${Boost_INCLUDE_DIRS}/boost/gil/extension/dynamic_image)
		INSTALL(CODE  "execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory  \"${INC_BOOST}/boost/gil/extension/dynamic_image\" \"${Boost_INCLUDE_DIRS}/boost/gil/extension/dynamic_image\") ")
	ELSE(NOT GIL_DYNAMIC_IMAGE)
		MESSAGE(STATUS " gil dynamic image found : change file if different ")
		FILE( GLOB DYN_FILES ${INC_BOOST}/gil/extension/dynamic_image/*.hpp)
		MESSAGE(STATUS " DYN_FILES " ${DYN_FILES} )
		FOREACH(FILE ${DYN_FILES} )
			MESSAGE(STATUS " dynamic file " ${FILE})
			GET_FILENAME_COMPONENT(NAME_FILE  "${FILE}" NAME)
			INSTALL(CODE  "execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different \"${FILE}\" \"${Boost_INCLUDE_DIRS}/boost/gil/extension/dynamic_image/${NAME_FILE}\") ")
		ENDFOREACH(FILE ${DYN_FILES} )
	ENDIF(NOT GIL_DYNAMIC_IMAGE)
	
	# extensions numeric
	FIND_PATH(GIL_NUMERIC algorithm.hpp
			PATH ${Boost_INCLUDE_DIRS}/boost/gil/extension/numeric
	)
	IF(NOT GIL_NUMERIC )
		MESSAGE(STATUS " gil numeric not found : copy io to " ${Boost_INCLUDE_DIRS}/boost/gil/extension/numeric)
		INSTALL(CODE  "execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory  \"${INC_BOOST}/boost/gil/extension/numeric\" \"${Boost_INCLUDE_DIRS}/boost/gil/extension/numeric\")")
	ELSE(NOT GIL_NUMERIC)
		MESSAGE(STATUS " gil numeric found : change files if different ")
		FILE( GLOB NUM_FILES ${INC_BOOST}/gil/extension/numeric/*.hpp)
		MESSAGE(STATUS " NUM_FILES " ${NUM_FILES} )
		FOREACH(FILE ${NUM_FILES} )
			MESSAGE(STATUS " numeric file " ${FILE})
			GET_FILENAME_COMPONENT(NAME_FILE  "${FILE}" NAME)
			INSTALL(CODE  "execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different \"${FILE}\" \"${Boost_INCLUDE_DIRS}/boost/gil/extension/numeric/${NAME_FILE}\")")
		ENDFOREACH(FILE ${NUM_FILES} )
	ENDIF(NOT GIL_NUMERIC)
ENDIF(NOT GIL_MATIS)
