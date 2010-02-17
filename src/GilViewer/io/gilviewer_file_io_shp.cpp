#include "gilviewer_file_io_shp.hpp"

#include <boost/filesystem/convenience.hpp>

#include "gilviewer_io_factory.hpp"

#include "GilViewer/layers/VectorLayer.hpp"

using namespace boost;
using namespace std;

void gilviewer_file_io_shp::load(shared_ptr<Layer>& layer, const string &filename)
{
    layer = VectorLayer::CreateVectorLayer(boost::filesystem::basename(filename),filename);
}

void gilviewer_file_io_shp::save(shared_ptr<Layer>& layer, const string &filename)
{
    layer->Save(filename);
}

boost::shared_ptr<gilviewer_file_io_shp> create_gilviewer_file_io_shp()
{
    return shared_ptr<gilviewer_file_io_shp>(new gilviewer_file_io_shp());
}

bool gilviewer_file_io_shp::Register()
{
    gilviewer_io_factory::Instance()->Register("shp", create_gilviewer_file_io_shp);
    return true;
}

bool register_shp_ok = gilviewer_file_io_shp::Register();
