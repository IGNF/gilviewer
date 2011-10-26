#include "gilviewer_file_io_cr2.hpp"
#include "GilViewer/io/gilviewer_io_factory.hpp"

using namespace boost;
using namespace boost::gil;
using namespace boost::filesystem;
using namespace std;

#include "GilViewer/plugins/plugin_base.hpp"
IMPLEMENT_PLUGIN(gilviewer_file_io_cr2);

string gilviewer_file_io_cr2::get_infos(const std::string &filename)
{
    if(!_info_read)
        _info = read_image_info(filename, cr2_tag());
    ostringstream infos_str;
    infos_str << "Dimensions: " << _info._width << "x" << _info._height << "\n";
    return "";
}

void gilviewer_file_io_cr2::save(boost::shared_ptr<layer> /*layer*/, const std::string &/*filename*/)
{
    GILVIEWER_LOG_ERROR("cr2 format cannot be written!");
}

shared_ptr<gilviewer_file_io_cr2> create_gilviewer_file_io_cr2()
{
    return shared_ptr<gilviewer_file_io_cr2>(new gilviewer_file_io_cr2());
}

bool gilviewer_file_io_cr2::Register(gilviewer_io_factory *factory)
{
    factory->insert("cr2", "Image","CR2", create_gilviewer_file_io_cr2);
    return true;
}
