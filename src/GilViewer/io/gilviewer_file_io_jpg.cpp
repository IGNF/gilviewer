#include "gilviewer_file_io_jpg.hpp"
#include <boost/gil/extension/io_new/jpeg_all.hpp>
#include "gilviewer_io_factory.hpp"

using namespace boost;
using namespace boost::gil;
using namespace boost::filesystem;
using namespace std;

boost::shared_ptr<layer> gilviewer_file_io_jpg::load(const string &filename, const ptrdiff_t top_left_x, const ptrdiff_t top_left_y, const ptrdiff_t dim_x, const ptrdiff_t dim_y)
{
    return load_gil_image<jpeg_tag>(filename, point_t(top_left_x, top_left_y), point_t(dim_x, dim_y));
}

void gilviewer_file_io_jpg::save(boost::shared_ptr<layer> layer, const string &filename)
{
    save_gil_view<jpeg_tag>(layer, filename);
}

string gilviewer_file_io_jpg::build_and_get_infos(const std::string &filename)
{
    image_read_info< jpeg_tag > info = read_image_info(filename, jpeg_tag());
    ostringstream infos_str;
    infos_str << "Dimensions: " << info._width << "x" << info._height << "\n";
    infos_str << "Number of components: " << info._num_components << "\n";
    infos_str << "Color space: ";
    switch(info._color_space)
    {
    case JCS_UNKNOWN   : infos_str << "JCS_UNKNOWN\n"; break;
    case JCS_GRAYSCALE : infos_str << "JCS_GRAYSCALE\n"; break;
    case JCS_RGB       : infos_str << "JCS_RGB\n"; break;
    case JCS_YCbCr     : infos_str << "JCS_YCbCr\n"; break;
    case JCS_CMYK      : infos_str << "JCS_CMYK\n"; break;
    case JCS_YCCK      : infos_str << "JCS_YCCK\n"; break;
    default            : infos_str << info._color_space << " (\?\?\?)\n"; break;
    }
    infos_str << "Data precision: " << info._data_precision << "\n";
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
