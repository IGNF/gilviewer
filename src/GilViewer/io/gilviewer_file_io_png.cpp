#include "gilviewer_file_io_png.hpp"
#include <boost/gil/extension/io_new/png_all.hpp>
#include "gilviewer_io_factory.hpp"

using namespace boost;
using namespace boost::gil;
using namespace boost::filesystem;
using namespace std;

shared_ptr<layer> gilviewer_file_io_png::load(const string &filename, const ptrdiff_t top_left_x, const ptrdiff_t top_left_y, const ptrdiff_t dim_x, const ptrdiff_t dim_y)
{
    return load_gil_image<png_tag>(filename, point_t(top_left_x, top_left_y), point_t(dim_x, dim_y));
}

void gilviewer_file_io_png::save(shared_ptr<layer> layer, const string &filename)
{
    save_gil_view<png_tag>(layer, filename);
}

string gilviewer_file_io_png::build_and_get_infos(const std::string &filename)
{
    image_read_info< png_tag > info = read_image_info(filename, png_tag());
    ostringstream infos_str;
    infos_str << "Dimensions: " << info._width << "x" << info._height << "\n";
    switch(info._interlace_method)
    {
    case PNG_INTERLACE_NONE  : infos_str << "Interlacing: None\n"; break;
    case PNG_INTERLACE_ADAM7 : infos_str << "Interlacing: Adam7"; break;
    default                  : infos_str << "Interlacing: \?\?\? ("<<info._interlace_method<<")\n"; break;
    }
    infos_str << "Bit depth: " << info._bit_depth << " bit(s)\n";
    infos_str << "Color type: " << info._color_type << "\n";
    infos_str << "Channel(s): " << (unsigned int) info._num_channels << "\n";
    infos_str << "Comment(s) : "<<info._num_text<<"\n";
    for(int i=0; i<info._num_text; ++i)
        infos_str << info._text[i]._key << " = " << info._text[i]._text << " ("<<info._text[i]._compression<<")\n";

    infos_str << "Further infos: TODO\n";
    return infos_str.str();
}

shared_ptr<gilviewer_file_io_png> create_gilviewer_file_io_png()
{
    return shared_ptr<gilviewer_file_io_png>(new gilviewer_file_io_png());
}

bool gilviewer_file_io_png::Register(gilviewer_io_factory *factory)
{
    factory->insert("png", "Image","PNG", create_gilviewer_file_io_png);
    return true;
}
