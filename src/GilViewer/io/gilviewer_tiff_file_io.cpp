#include "gilviewer_tiff_file_io.hpp"
#include "gilviewer_io_factory.hpp"

void gilviewer_tiff_file_io::load()
{
}

void gilviewer_tiff_file_io::save()
{
}

boost::shared_ptr<gilviewer_tiff_file_io> create_gilviewer_tiff_file_io()
{
        return boost::shared_ptr<gilviewer_tiff_file_io>(new gilviewer_tiff_file_io());
}

bool gilviewer_tiff_file_io::Register()
{
        gilviewer_io_factory::Instance()->Register("tif", create_gilviewer_tiff_file_io);
        return true;
}

bool register_tiff_ok = gilviewer_tiff_file_io::Register();
