if(UNIX)
    add_executable(sample_ImageGroundTruthEditor
        ImageGroundTruthApplication.cpp
        ImageGroundTruthEditor.cpp
        ImageGroundTruthManager.cpp
        ImageGroundTruthPanelViewer.cpp
        GroundTruthBox.cpp
        STL_ExtensionJPB/STL_manager.cpp)
    target_link_libraries( sample_ImageGroundTruthEditor ${GILVIEWER_LINK_EXTERNAL_LIBRARIES} GilViewer )
endif()
if(WIN32)
    add_executable(sample_ImageGroundTruthEditor WIN32 ImageGroundTruthApplication.cpp
        ImageGroundTruthEditor.cpp
        ImageGroundTruthManager.cpp
        ImageGroundTruthPanelViewer.cpp
        GroundTruthBox.cpp
        STL_ExtensionJPB/STL_manager.cpp)
    target_link_libraries( sample_ImageGroundTruthEditor ${GILVIEWER_LINK_EXTERNAL_LIBRARIES} GilViewer )
endif()
