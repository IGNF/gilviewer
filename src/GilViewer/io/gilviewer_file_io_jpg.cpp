#include "gilviewer_file_io_jpg.hpp"
#include "gilviewer_io_factory.hpp"

#include "GilViewer/layers/ImageLayer.hpp"

using namespace boost;
using namespace std;

shared_ptr<Layer> gilviewer_file_io_jpg::load(const string &filename)
{
    return ImageLayer::CreateImageLayer(filename);
}

void gilviewer_file_io_jpg::save(shared_ptr<Layer>& layer, const string &filename)
{
    layer->Save(filename);
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
