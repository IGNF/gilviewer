#include "gilviewer_io_factory.hpp"

#include "gilviewer_file_io_tiff.hpp"

void register_all_file_formats()
{
    gilviewer_file_io_tiff::Register();
}
