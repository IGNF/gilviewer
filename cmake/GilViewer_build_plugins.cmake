set(sample_plugin_SRCS ./src/GilViewer/plugins/sample_plugin.hpp ./src/GilViewer/plugins/sample_plugin.cpp )

add_library(sample_plugin SHARED ${sample_plugin_SRCS})
target_link_libraries(sample_plugin GilViewer)

