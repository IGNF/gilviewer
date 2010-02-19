#include "gilviewer_file_io_jpg.hpp"
#include "gilviewer_io_factory.hpp"

#include <boost/filesystem/convenience.hpp>
#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>

#include "GilViewer/layers/ImageLayer.hpp"
#include "GilViewer/layers/image_types.hpp"

using namespace boost;
using namespace std;

shared_ptr<Layer> gilviewer_file_io_jpg::load(const string &filename)
{
    return ImageLayer::CreateImageLayer(filename);
}

void gilviewer_file_io_jpg::save(shared_ptr<Layer> layer, const string &filename)
{
    shared_ptr<ImageLayer> image_layer = dynamic_pointer_cast<ImageLayer>(layer);
    if(!image_layer)
        throw invalid_argument("Bad layer type!\n");

    gil::jpeg_write_view( filename.c_str() , image_layer->View()->value );
}

boost::shared_ptr<gilviewer_file_io_jpg> create_gilviewer_file_io_jpg()
{
    return shared_ptr<gilviewer_file_io_jpg>(new gilviewer_file_io_jpg());
}

bool gilviewer_file_io_jpg::Register()
{
    gilviewer_io_factory::Instance()->Register("jpg", create_gilviewer_file_io_jpg);
    gilviewer_io_factory::Instance()->Register("jpeg", create_gilviewer_file_io_jpg);
    return true;
}

bool register_jpg_ok = gilviewer_file_io_jpg::Register();
