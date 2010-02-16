#include "gilviewer_tiff_file_io.hpp"
#include "gilviewer_io_factory.hpp"

#include "GilViewer/layers/ImageLayer.hpp"

using namespace boost;
using namespace std;

void gilviewer_tiff_file_io::load(shared_ptr<Layer>& layer, const string &filename)
{
    layer = ImageLayer::CreateImageLayer(filename);
}

void gilviewer_tiff_file_io::save(shared_ptr<Layer>& layer, const string &filename)
{
    layer->Save(filename);
}

boost::shared_ptr<gilviewer_tiff_file_io> create_gilviewer_tiff_file_io()
{
        return shared_ptr<gilviewer_tiff_file_io>(new gilviewer_tiff_file_io());
}

bool gilviewer_tiff_file_io::Register()
{
        gilviewer_io_factory::Instance()->Register("tif", create_gilviewer_tiff_file_io);
        gilviewer_io_factory::Instance()->Register("tiff", create_gilviewer_tiff_file_io);
        return true;
}

bool register_tiff_ok = gilviewer_tiff_file_io::Register();
