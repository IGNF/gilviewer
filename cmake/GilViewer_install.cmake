#################################################
###           INSTALL 						  ###
#################################################
GET_FILENAME_COMPONENT(GILVIEWER_CMAKE_SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

IF(UNIX)
	SET(INSTALL_PREFIX "/usr/local" CACHE PATH " install path" )
	SET(GilViewer_INSTALL_PATH ${INSTALL_PREFIX})
    SET(GilViewer_INCLUDE_PATH ${INSTALL_PREFIX}/include/GilViewer)
	SET(tinyxml_INCLUDE_PATH ${INSTALL_PREFIX}/include/tinyxml)
	SET(RELATIVE_INCLUDE_PATH "../../include/GilViewer")
    SET(RELATIVE_INCLUDE_PATH_TINYXML "../../include/tinyxml")
        SET(TARGET_CONFIG_INSTALL_PATH ${INSTALL_PREFIX}/${INTEMEDIATE_FOLDER}/lib/GilViewer)
ENDIF(UNIX)
IF(WIN32)
	SET(GilViewer_INSTALL_DIR "include/GilViewer")
	SET(INSTALL_PREFIX "C:/Program Files/MATIS" CACHE PATH " install path")
	SET(GilViewer_INSTALL_PATH ${INSTALL_PREFIX}/${GilViewer_INSTALL_DIR})
    SET(GilViewer_INCLUDE_PATH ${INSTALL_PREFIX}/${GilViewer_INSTALL_DIR})
	SET(tinyxml_INCLUDE_PATH ${GilViewer_INSTALL_PATH}/../tinyxml)
	SET(RELATIVE_INCLUDE_PATH "../include/GilViewer")
    SET(RELATIVE_INCLUDE_PATH_TINYXML "../include/tinyxml")
	SET(TARGET_CONFIG_INSTALL_PATH ${INSTALL_PREFIX}/cmake)
ENDIF(WIN32)
SET(CMAKE_INSTALL_PREFIX ${INSTALL_PREFIX} CACHE INTERNAL  " real install path" FORCE)

SET(export_lib "")
SET(GilViewer_TINYXML_SHARED ON)

IF( BUILD_GILVIEWERLIB_SHARED )
	MESSAGE(STATUS " INSTALL : libGilViewer Shared ")
	SET( export_lib GilViewer )
ELSE(BUILD_GILVIEWERLIB_SHARED)
	MESSAGE(STATUS " INSTALL : libGilViewer Static ")
	SET(GilViewer_TINYXML_SHARED OFF)
	SET( export_lib GilViewer )
ENDIF(BUILD_GILVIEWERLIB_SHARED)

IF( BUILD_TINYXML_SHARED )
	MESSAGE(STATUS " INSTALL : libtinyxml Shared ")
	SET( export_lib ${export_lib} tinyxml )
ELSE(BUILD_TINYXML_SHARED)
	MESSAGE(STATUS " INSTALL : libtinyxml Static ")
	SET(GilViewer_TINYXML_SHARED OFF)
	SET( export_lib ${export_lib} tinyxml )
ENDIF(BUILD_TINYXML_SHARED)

MESSAGE(STATUS " ALL_TINYXML_INC_FILES" ${ALL_TINYXML_INC_FILES})
INSTALL(FILES ${ALL_TINYXML_INC_FILES} DESTINATION ${tinyxml_INCLUDE_PATH})



CONFIGURE_FILE(${GILVIEWER_CMAKE_SELF_DIR}/GilViewerConfig.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/GilViewerConfig.cmake @ONLY)
#install header of gilviewer
INSTALL(FILES ${ALL_CONFIG_HEADER_FILES} DESTINATION ${GilViewer_INCLUDE_PATH}/config)
INSTALL(FILES ${ALL_CONVENIENT_HEADER_FILES} DESTINATION ${GilViewer_INCLUDE_PATH}/convenient)
INSTALL(FILES ${ALL_GUI_HEADER_FILES} DESTINATION ${GilViewer_INCLUDE_PATH}/gui )
INSTALL(FILES ${ALL_IO_HEADER_FILES} DESTINATION ${GilViewer_INCLUDE_PATH}/io )
INSTALL(FILES ${ALL_LAYERS_HEADER_FILES} DESTINATION ${GilViewer_INCLUDE_PATH}/layers )
INSTALL(FILES ${ALL_TOOLS_HEADER_FILES} DESTINATION ${GilViewer_INCLUDE_PATH}/tools )
#install lib and export target
# TODO
if(WIN32)
	INSTALL (TARGETS ${export_lib} EXPORT GilViewer-targets DESTINATION ${GilViewer_INSTALL_PATH}/../../lib COMPONENT library ) 
else()
	INSTALL (TARGETS ${export_lib} EXPORT GilViewer-targets DESTINATION ${GilViewer_INSTALL_PATH}/lib COMPONENT library ) 
endif()

#install config file
# TODO
if(WIN32)
else()
	INSTALL (EXPORT GilViewer-targets DESTINATION ${TARGET_CONFIG_INSTALL_PATH} )
endif()
INSTALL (FILES GilViewerConfig.cmake DESTINATION ${TARGET_CONFIG_INSTALL_PATH})

#install translation files
if( UNIX )
	message( STATUS " Installing langages support files" )
    install( FILES ${LANGAGES_FR_SUPPORT_FILES} DESTINATION /usr/share/locale/fr/LC_MESSAGES/ )
else( WIN32 )
    # To install in the application directory
endif()


MESSAGE(STATUS " boost_INCLUDE_DIRS " ${Boost_INCLUDE_DIRS})

MESSAGE(STATUS " test  GIL_MATIS " ${GIL_MATIS})


FIND_PATH(GIL_MATIS float_images.hpp
			PATH ${Boost_INCLUDE_DIRS}/boost/gil/extension/matis
)
IF(NOT GIL_MATIS)
	MESSAGE(STATUS " gil matis not found ")
	INSTALL(CODE "
		 MESSAGE(STATUS \"install gil matis extension\")
   		 EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_directory  
		 		\"${CMAKE_SOURCE_DIR}/${INC_BOOST}/gil/extension/matis\" 
				\"${Boost_INCLUDE_DIRS}/boost/gil/extension/matis\" )
		" )
ELSE(NOT GIL_MATIS)
	MESSAGE(STATUS " gil matis found : change file if different " ${INC_BOOST}/boost/gil/extension/matis)
	FILE( GLOB MATIS_FILES ${INC_BOOST}/gil/extension/matis/*.hpp)
	#MESSAGE(STATUS " MATIS_FILES " ${MATIS_FILES} )
	FOREACH(FILE ${MATIS_FILES} )
		#MESSAGE(STATUS " MATIS file " ${FILE})
		GET_FILENAME_COMPONENT(NAME_FILE  "${FILE}" NAME)
		INSTALL(CODE  "
			MESSAGE(STATUS \"copy if different file : \" ${Boost_INCLUDE_DIRS}/boost/gil/extension/matis/${NAME_FILE} )
			execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different 
				\"${FILE}\" 
				\"${Boost_INCLUDE_DIRS}/boost/gil/extension/matis/${NAME_FILE}\"
				) 
		")
	ENDFOREACH(FILE ${MATIS_FILES} )
ENDIF(NOT GIL_MATIS)

# Copy (all) boost::gil base files
FILE( GLOB GILBASE_FILES ${INC_BOOST}/gil/*.hpp)
        #MESSAGE(STATUS " GILBASE_FILES " ${GILBASE_FILES} )
        FOREACH(FILE ${GILBASE_FILES} )
                #MESSAGE(STATUS " gile base file " ${FILE})
                GET_FILENAME_COMPONENT(NAME_FILE  "${FILE}" NAME)
                INSTALL(CODE  "
                        MESSAGE(STATUS \"copy if different file : \" ${Boost_INCLUDE_DIRS}/boost/gil/${NAME_FILE} )
                        execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different \"${FILE}\" \"${Boost_INCLUDE_DIRS}/boost/gil/${NAME_FILE}\")
                ")
ENDFOREACH(FILE ${GILBASE_FILES} )

# extensions io 
FIND_PATH(GIL_IO dynamic_io.hpp
		PATH ${Boost_INCLUDE_DIRS}/boost/gil/extension/io
)
IF(NOT GIL_IO )
        MESSAGE(STATUS " gil io not found : copy io to " ${Boost_INCLUDE_DIRS}/boost/gil/extension/io)
        INSTALL(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory  \"${CMAKE_SOURCE_DIR}/${INC_BOOST}/gil/extension/io\" \"\${Boost_INCLUDE_DIRS}/boost/gil/extension/io\" )" )
ELSE(NOT GIL_IO)
        MESSAGE(STATUS " gil io found : change file if different " ${INC_BOOST}/boost/gil/extension/io)
        FILE( GLOB IO_FILES ${INC_BOOST}/gil/extension/io/*.hpp)
        #MESSAGE(STATUS " IO_FILES " ${IO_FILES} )
        FOREACH(FILE ${IO_FILES} )
                #MESSAGE(STATUS " io file " ${FILE})
                GET_FILENAME_COMPONENT(NAME_FILE  "${FILE}" NAME)
                INSTALL(CODE  "
                        MESSAGE(STATUS \"copy if different file : \" ${Boost_INCLUDE_DIRS}/boost/gil/extension/io/${NAME_FILE} )
                        execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different \"${FILE}\" \"${Boost_INCLUDE_DIRS}/boost/gil/extension/io/${NAME_FILE}\")
                ")
        ENDFOREACH(FILE ${IO_FILES} )
ENDIF(NOT GIL_IO )

# extensions io_new
FIND_PATH(GIL_IONEW tiff_io_old.hpp
                PATH ${Boost_INCLUDE_DIRS}/boost/gil/extension/io_new
)
IF(NOT GIL_IONEW )
        MESSAGE(STATUS " gil io_new not found : copy io_new to " ${Boost_INCLUDE_DIRS}/boost/gil/extension/io_new)
        INSTALL(CODE  "
                MESSAGE(STATUS \"copy if different file : \" ${Boost_INCLUDE_DIRS}/boost/gil/extension/io_new )
               execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory
                   \"${CMAKE_SOURCE_DIR}/${INC_BOOST}/gil/extension/io_new\"
                   \"${Boost_INCLUDE_DIRS}/boost/gil/extension/io_new/\")
        ")
ELSE(NOT GIL_IONEW)
        MESSAGE(STATUS " gil io_new found : change file if different " ${INC_BOOST}/boost/gil/extension/io_new)     
                MESSAGE(STATUS " io_new file " ${FILE})
                GET_FILENAME_COMPONENT(NAME_FILE  "${FILE}" NAME)
                INSTALL(CODE  "
                        MESSAGE(STATUS \"copy if different file : \" ${Boost_INCLUDE_DIRS}/boost/gil/extension/io_new/${NAME_FILE} )
                       execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory
                           \"${CMAKE_SOURCE_DIR}/${INC_BOOST}/gil/extension/io_new\"
                           \"${Boost_INCLUDE_DIRS}/boost/gil/extension/io_new/\")
                ")


ENDIF(NOT GIL_IONEW )

# extensions dynamic_image 
FIND_PATH(GIL_DYNAMIC_IMAGE variant.hpp
		PATH ${Boost_INCLUDE_DIRS}/boost/gil/extension/dynamic_image
)
IF(NOT GIL_DYNAMIC_IMAGE)
        MESSAGE(STATUS " gil dynamic_image not found : copy dynamic_image to " ${Boost_INCLUDE_DIRS}/boost/gil/extension/dynamic_image)
        INSTALL(CODE  "execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory  \"${CMAKE_SOURCE_DIR}/${INC_BOOST}/gil/extension/dynamic_image\" \"${Boost_INCLUDE_DIRS}/boost/gil/extension/dynamic_image\") ")
ELSE(NOT GIL_DYNAMIC_IMAGE)
	MESSAGE(STATUS " gil dynamic image found : change file if different ")
        #FILE( GLOB DYN_FILES ${INC_BOOST}/gil/extension/dynamic_image/*.hpp)
        set( DYN_FILES ${CMAKE_SOURCE_DIR}/${INC_BOOST}/gil/extension/dynamic_image/image_view_factory.hpp )
	#MESSAGE(STATUS " DYN_FILES " ${DYN_FILES} )
	FOREACH(FILE ${DYN_FILES} )
		#MESSAGE(STATUS " dynamic file " ${FILE})
		GET_FILENAME_COMPONENT(NAME_FILE  "${FILE}" NAME)
		INSTALL(CODE  "
			MESSAGE(STATUS \"copy if different file : \" ${Boost_INCLUDE_DIRS}/boost/gil/extension/dynamic_image/${NAME_FILE} )
                        execute_process(COMMAND ${CMAKE_COMMAND} -E copy \"${FILE}\" \"${Boost_INCLUDE_DIRS}/boost/gil/extension/dynamic_image/${NAME_FILE}\")
		")
	ENDFOREACH(FILE ${DYN_FILES} )
ENDIF(NOT GIL_DYNAMIC_IMAGE)

# extensions numeric
FIND_PATH(GIL_NUMERIC algorithm.hpp
		PATH ${Boost_INCLUDE_DIRS}/boost/gil/extension/numeric
)
IF(NOT GIL_NUMERIC )
        MESSAGE(STATUS " gil numeric not found : copy numeric to " ${Boost_INCLUDE_DIRS}/boost/gil/extension/numeric)
	INSTALL(CODE  "execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory  \"${CMAKE_SOURCE_DIR}/${INC_BOOST}/gil/extension/numeric\" \"${Boost_INCLUDE_DIRS}/boost/gil/extension/numeric\")")
ELSE(NOT GIL_NUMERIC)
	MESSAGE(STATUS " gil numeric found : change files if different ")
	FILE( GLOB NUM_FILES ${INC_BOOST}/gil/extension/numeric/*.hpp)
	#MESSAGE(STATUS " NUM_FILES " ${NUM_FILES} )
	FOREACH(FILE ${NUM_FILES} )
		#MESSAGE(STATUS " numeric file " ${FILE})
		GET_FILENAME_COMPONENT(NAME_FILE  "${FILE}" NAME)
		INSTALL(CODE  "
			MESSAGE(STATUS \"copy if different file : \" ${Boost_INCLUDE_DIRS}/boost/gil/extension/numeric/${NAME_FILE} )
			execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different \"${FILE}\" \"${Boost_INCLUDE_DIRS}/boost/gil/extension/numeric/${NAME_FILE}\")
		")
	ENDFOREACH(FILE ${NUM_FILES} )
ENDIF(NOT GIL_NUMERIC)

