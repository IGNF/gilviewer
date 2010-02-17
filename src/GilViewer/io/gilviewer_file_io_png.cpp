#include "gilviewer_file_io_png.hpp"
#include "gilviewer_io_factory.hpp"

#include "GilViewer/layers/ImageLayer.hpp"

using namespace boost;
using namespace std;

void gilviewer_file_io_png::load(shared_ptr<Layer>& layer, const string &filename)
{
    layer = ImageLayer::CreateImageLayer(filename);
}

void gilviewer_file_io_png::save(shared_ptr<Layer>& layer, const string &filename)
{
    layer->Save(filename);
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
