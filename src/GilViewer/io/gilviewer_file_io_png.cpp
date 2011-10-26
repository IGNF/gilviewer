#include "gilviewer_file_io_png.hpp"
#include "gilviewer_io_factory.hpp"

using namespace boost;
using namespace boost::gil;
using namespace boost::filesystem;
using namespace std;

string gilviewer_file_io_png::get_infos(const std::string &filename)
{
    if(!_info_read)
        _info = read_image_info(filename, png_tag());
    ostringstream infos_str;
    infos_str << "Dimensions: " << _info._width << "x" << _info._height << "\n";
    switch(_info._interlace_method)
    {
    case PNG_INTERLACE_NONE  : infos_str << "Interlacing: None\n"; break;
    case PNG_INTERLACE_ADAM7 : infos_str << "Interlacing: Adam7"; break;
    default                  : infos_str << "Interlacing: \?\?\? ("<<_info._interlace_method<<")\n"; break;
    }
    infos_str << "Bit depth: " << _info._bit_depth << " bit(s)\n";
    infos_str << "Color type: " << _info._color_type << "\n";
    infos_str << "Channel(s): " << (unsigned int) _info._num_channels << "\n";
    infos_str << "Comment(s) : "<<_info._num_text<<"\n";
    for(int i=0; i<_info._num_text; ++i)
        infos_str << _info._text[i]._key << " = " << _info._text[i]._text << " ("<<_info._text[i]._compression<<")\n";

    infos_str << "Further infos: TODO\n";
    return infos_str.str();
}

boost::shared_ptr<gilviewer_file_io_png> create_gilviewer_file_io_png()
{
    return boost::shared_ptr<gilviewer_file_io_png>(new gilviewer_file_io_png());
}

bool gilviewer_file_io_png::Register(gilviewer_io_factory *factory)
{
    factory->insert("png", "Image","PNG", create_gilviewer_file_io_png);
    return true;
}
