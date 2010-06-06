#include "gilviewer_file_io_tiff.hpp"
#include "gilviewer_io_factory.hpp"

#include <boost/filesystem/convenience.hpp>
#include <boost/gil/extension/io/tiff_dynamic_io.hpp>

#include "../layers/ImageLayer.hpp"
#include "../layers/image_types.hpp"

using namespace boost;
using namespace std;

shared_ptr<layer> gilviewer_file_io_tiff::load(const string &filename)
{
    return image_layer::CreateImageLayer(filename);
}

void gilviewer_file_io_tiff::save(shared_ptr<layer> layer, const string &filename)
{
    shared_ptr<image_layer> imagelayer = dynamic_pointer_cast<image_layer>(layer);
    if(!imagelayer)
        throw invalid_argument("Bad layer type!\n");

    gil::tiff_write_view( filename.c_str() , imagelayer->View()->value );
}

boost::shared_ptr<gilviewer_file_io_tiff> create_gilviewer_file_io_tiff()
{
    return shared_ptr<gilviewer_file_io_tiff>(new gilviewer_file_io_tiff());
}

bool gilviewer_file_io_tiff::Register()
{
    gilviewer_io_factory::Instance()->Register("tif", create_gilviewer_file_io_tiff);
    gilviewer_io_factory::Instance()->Register("tiff", create_gilviewer_file_io_tiff);
    return true;
}

bool register_tiff_ok = gilviewer_file_io_tiff::Register();
