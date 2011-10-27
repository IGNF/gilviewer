#include "gilviewer_file_io_raw.hpp"
#include "GilViewer/io/gilviewer_io_factory.hpp"

using namespace boost;
using namespace boost::gil;
using namespace boost::filesystem;
using namespace std;

#include "GilViewer/plugins/plugin_base.hpp"
IMPLEMENT_PLUGIN(gilviewer_file_io_raw);

string gilviewer_file_io_raw::get_infos(const std::string &filename)
{
    if(!_info_read)
        _info = read_image_info(filename, raw_tag());
    ostringstream infos_str;
    infos_str << "Dimensions: " << _info._width << "x" << _info._height << "\n";
    infos_str << "Number of components : " << _info._samples_per_pixel << "\n";
    infos_str << "Bits per pixel : " << _info._bits_per_pixel << "\n";
    infos_str << "Camera manufacturer : " << _info._camera_manufacturer << "\n";
    infos_str << "Camera model : " << _info._camera_model << "\n";
    infos_str << "Number of raw images : " << _info._raw_images_count << "\n";
    infos_str << "DNG version : " << _info._dng_version << "\n\n";

    infos_str << "Raw dimensions : " << _info._raw_width << "x" << _info._raw_height << "\n";
    infos_str << "Visible dimensions : " << _info._visible_width << "x" << _info._visible_height << "\n";
    infos_str << "Top margin : " << _info._top_margin << "\n";
    infos_str << "Left margin : " << _info._left_margin << "\n";
    infos_str << "Pixel aspect : " << _info._pixel_aspect << "\n";
    switch(_info._flip)
    {
    case 0: infos_str << "Flip : 0°\n\n"; break;
    case 3: infos_str << "Flip : 180°\n\n"; break;
    case 5: infos_str << "Flip : 90° ccw\n\n"; break;
    case 6: infos_str << "Flip : 90° cw\n\n"; break;
    }

    infos_str << "Iso speed : " << _info._iso_speed << "\n";
    infos_str << "Shutter : " << _info._shutter << "\n";
    infos_str << "Aperture : " << _info._aperture << "\n";
    infos_str << "Focal length : " << _info._focal_length << "\n";
    infos_str << "Timestamp : " << ctime(&_info._timestamp) << "\n";
    infos_str << "Shot order : " << _info._shot_order << "\n";
    infos_str << "Image description : " << _info._image_description << "\n";
    infos_str << "Artist : " << _info._artist << "\n\n";

    infos_str << "[libraw version: " << _info._libraw_version << "]\n";
    infos_str << "[Unpack function: " << _info._unpack_function_name << "]\n";

    return infos_str.str();
}

void gilviewer_file_io_raw::save(boost::shared_ptr<layer> /*layer*/, const std::string &/*filename*/)
{
    GILVIEWER_LOG_ERROR("raw format cannot be written!");
}

shared_ptr<gilviewer_file_io_raw> create_gilviewer_file_io_raw()
{
    return shared_ptr<gilviewer_file_io_raw>(new gilviewer_file_io_raw());
}

bool gilviewer_file_io_raw::Register(gilviewer_io_factory *factory)
{
    factory->insert("cr2", "Image", "RAW", create_gilviewer_file_io_raw); // canon
    factory->insert("nef", "Image", "RAW", create_gilviewer_file_io_raw); // nikon
    factory->insert("pef", "Image", "RAW", create_gilviewer_file_io_raw); // pentax
    factory->insert("sr2", "Image", "RAW", create_gilviewer_file_io_raw); // sony
    factory->insert("raw", "Image", "RAW", create_gilviewer_file_io_raw); //
    factory->insert("dng", "Image", "RAW", create_gilviewer_file_io_raw); //
    //factory->insert("3fr", "Image", "RAW", create_gilviewer_file_io_raw); // hasselblad // does not work: begins with a number?
    return true;
}
