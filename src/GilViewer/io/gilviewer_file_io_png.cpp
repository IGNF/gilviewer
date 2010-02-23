#include "gilviewer_file_io_png.hpp"
#include "gilviewer_io_factory.hpp"

#include <boost/filesystem/convenience.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>

#include "GilViewer/layers/ImageLayer.hpp"
#include "GilViewer/layers/image_types.hpp"

using namespace boost;
using namespace std;

shared_ptr<Layer> gilviewer_file_io_png::load(const string &filename)
{
    return ImageLayer::CreateImageLayer(filename);
}

void gilviewer_file_io_png::save(shared_ptr<Layer> layer, const string &filename)
{
    shared_ptr<ImageLayer> image_layer = dynamic_pointer_cast<ImageLayer>(layer);
    if(!image_layer)
        throw invalid_argument("Bad layer type!\n");

    gil::png_write_view( filename.c_str() , image_layer->View()->value );
}

boost::shared_ptr<gilviewer_file_io_png> create_gilviewer_file_io_png()
{
    return shared_ptr<gilviewer_file_io_png>(new gilviewer_file_io_png());
}

bool gilviewer_file_io_png::Register()
{
    gilviewer_io_factory::Instance()->Register("png", create_gilviewer_file_io_png);
    return true;
}

bool register_png_ok = gilviewer_file_io_png::Register();
