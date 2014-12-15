#include "gilviewer_file_io_bmp.hpp"
#include "gilviewer_io_factory.hpp"

using namespace boost;
using namespace boost::gil;
using namespace boost::filesystem;
using namespace std;

string gilviewer_file_io_bmp::get_infos(const std::string &filename)
{
    // TODO: complete
    if(!_info_read)
        _info = read_image_info(filename, bmp_tag());
    ostringstream infos_str;
    infos_str << "Dimensions: " << _info._width << "x" << _info._height << "\n";
    return infos_str.str();
}

boost::shared_ptr<gilviewer_file_io_bmp> create_gilviewer_file_io_bmp()
{
    return boost::shared_ptr<gilviewer_file_io_bmp>(new gilviewer_file_io_bmp());
}

bool gilviewer_file_io_bmp::Register(gilviewer_io_factory *factory)
{
    factory->insert("bmp", "Image","BMP", create_gilviewer_file_io_bmp);
    return true;
}
