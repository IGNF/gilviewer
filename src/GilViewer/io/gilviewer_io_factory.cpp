#include "gilviewer_io_factory.hpp"

#include "gilviewer_file_io_jpg.hpp"
#include "gilviewer_file_io_png.hpp"
#include "gilviewer_file_io_tiff.hpp"

#include "gilviewer_file_io_shp.hpp"

void register_all_image_file_formats()
{
    gilviewer_file_io_jpg::Register();
    gilviewer_file_io_png::Register();
    gilviewer_file_io_tiff::Register();
}

void register_all_vector_file_formats()
{
    gilviewer_file_io_shp::Register();
}

void register_all_file_formats()
{
    register_all_image_file_formats();
    register_all_vector_file_formats();
}
