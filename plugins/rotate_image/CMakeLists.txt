set(rotate_image_plugin_SRCS rotate_image.hpp rotate_image.cpp )

add_library(rotate_image_plugin SHARED ${rotate_image_plugin_SRCS})
target_link_libraries(rotate_image_plugin GilViewer)


