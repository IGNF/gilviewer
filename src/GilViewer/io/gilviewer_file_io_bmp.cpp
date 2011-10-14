#include "gilviewer_file_io_bmp.hpp"
#include <boost/gil/extension/io_new/bmp_all.hpp>
#include "gilviewer_io_factory.hpp"

using namespace boost;
using namespace boost::gil;
using namespace boost::filesystem;
using namespace std;

shared_ptr<layer> gilviewer_file_io_bmp::load(const string &filename, const ptrdiff_t top_left_x, const ptrdiff_t top_left_y, const ptrdiff_t dim_x, const ptrdiff_t dim_y)
{
    return load_gil_image<bmp_tag>(filename, point_t(top_left_x, top_left_y), point_t(dim_x, dim_y));
}

void gilviewer_file_io_bmp::save(shared_ptr<layer> layer, const string &filename)
{
    save_gil_view<bmp_tag>(layer, filename);
}

string gilviewer_file_io_bmp::build_and_get_infos(const std::string &filename)
{
    image_read_info< bmp_tag > info = read_image_info(filename, bmp_tag());
    ostringstream infos_str;
    infos_str << "Dimensions: " << info._width << "x" << info._height << "\n";
    return infos_str.str();
}

shared_ptr<gilviewer_file_io_bmp> create_gilviewer_file_io_bmp()
{
    return shared_ptr<gilviewer_file_io_bmp>(new gilviewer_file_io_bmp());
}

bool gilviewer_file_io_bmp::Register(gilviewer_io_factory *factory)
{
    factory->insert("bmp", "Image","BMP", create_gilviewer_file_io_bmp);
    return true;
}
