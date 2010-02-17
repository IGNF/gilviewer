#include "gilviewer_file_io_tiff.hpp"
#include "gilviewer_io_factory.hpp"

#include "GilViewer/layers/ImageLayer.hpp"

using namespace boost;
using namespace std;

shared_ptr<Layer> gilviewer_file_io_tiff::load(const string &filename)
{
    return ImageLayer::CreateImageLayer(filename);
}

void gilviewer_file_io_tiff::save(shared_ptr<Layer>& layer, const string &filename)
{
    layer->Save(filename);
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
