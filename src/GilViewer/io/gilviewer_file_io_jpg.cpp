#include <stdio.h>

#include "gilviewer_io_factory.hpp"
#include "gilviewer_file_io_jpg.hpp"


using namespace boost;
using namespace boost::gil;
using namespace boost::filesystem;
using namespace std;

string gilviewer_file_io_jpg::get_infos(const std::string &filename)
{
    if(!_info_read)
        _info = read_image_info(filename, jpeg_tag());
    ostringstream infos_str;
    infos_str << "Dimensions: " << _info._width << "x" << _info._height << "\n";
    infos_str << "Number of components: " << _info._num_components << "\n";
    infos_str << "Color space: ";
    switch(_info._color_space)
    {
    case JCS_UNKNOWN   : infos_str << "JCS_UNKNOWN\n"; break;
    case JCS_GRAYSCALE : infos_str << "JCS_GRAYSCALE\n"; break;
    case JCS_RGB       : infos_str << "JCS_RGB\n"; break;
    case JCS_YCbCr     : infos_str << "JCS_YCbCr\n"; break;
    case JCS_CMYK      : infos_str << "JCS_CMYK\n"; break;
    case JCS_YCCK      : infos_str << "JCS_YCCK\n"; break;
    default            : infos_str << _info._color_space << " (\?\?\?)\n"; break;
    }
    infos_str << "Data precision: " << _info._data_precision << "\n";
    return infos_str.str();
}

boost::shared_ptr<gilviewer_file_io_jpg> create_gilviewer_file_io_jpg()
{
    return boost::shared_ptr<gilviewer_file_io_jpg>(new gilviewer_file_io_jpg());
}

bool gilviewer_file_io_jpg::Register(gilviewer_io_factory *factory)
{
    factory->insert("jpg", "Image","JPEG", create_gilviewer_file_io_jpg);
    factory->insert("jpeg","Image","JPEG", create_gilviewer_file_io_jpg);
    return true;
}
