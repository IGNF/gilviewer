#include "gilviewer_file_io_jpg.hpp"
#include "gilviewer_io_factory.hpp"

using namespace boost;
using namespace boost::gil;
using namespace boost::filesystem;
using namespace std;

#include <boost/variant/static_visitor.hpp>
#include <boost/variant/apply_visitor.hpp>

struct write_view_jpeg_visitor : public static_visitor<>
{
    write_view_jpeg_visitor(const string& filename) : m_filename(filename) {}

    template <typename ViewType>
    result_type operator()(const ViewType& v) const { write_view( m_filename , v, jpeg_tag() ); }

private:
    string m_filename;
};

shared_ptr<layer> gilviewer_file_io_jpg::load(const string &filename)
{
    return load_gil_image<jpeg_tag>(filename);
}

void gilviewer_file_io_jpg::save(shared_ptr<layer> layer, const string &filename)
{
    save_gil_view<jpeg_tag>(layer, filename);
}

string gilviewer_file_io_jpg::build_and_get_infos(const std::string &filename)
{
    image_read_info< jpeg_tag > info = read_image_info(filename, jpeg_tag());
    ostringstream infos_str;
    infos_str << "Dimensions: " << info._width << "x" << info._height << "\n";
    infos_str << "Number of components: " << info._num_components << "\n";
    string colorspace_str;
    if(info._color_space==JCS_UNKNOWN)
        colorspace_str="JCS_UNKNOWN";
    else if(info._color_space==JCS_GRAYSCALE)
        colorspace_str="JCS_GRAYSCALE";
    else if(info._color_space==JCS_RGB)
        colorspace_str="JCS_RGB";
    else if(info._color_space==JCS_YCbCr)
        colorspace_str="JCS_YCbCr";
    else if(info._color_space==JCS_CMYK)
        colorspace_str="JCS_CMYK";
    else if(info._color_space==JCS_YCCK)
        colorspace_str="JCS_YCCK";
    infos_str << "Color space: " << colorspace_str << "\n";
    infos_str << "Data precision: " << info._data_precision << "\n";
    return infos_str.str();
}

boost::shared_ptr<gilviewer_file_io_jpg> create_gilviewer_file_io_jpg()
{
    return shared_ptr<gilviewer_file_io_jpg>(new gilviewer_file_io_jpg());
}

bool gilviewer_file_io_jpg::Register()
{
    gilviewer_io_factory::instance()->Register("jpg", create_gilviewer_file_io_jpg);
    gilviewer_io_factory::instance()->Register("jpeg", create_gilviewer_file_io_jpg);
    pair<string,string> familly_description = make_pair<string,string>("Image files","JPEG images");
    pair< string, pair<string,string> > to_insert = make_pair< string, pair<string,string> >( "jpg", familly_description );
    gilviewer_io_factory::instance()->metadata().insert( to_insert );
    to_insert = make_pair< string, pair<string,string> >( "jpeg", familly_description );
    gilviewer_io_factory::instance()->metadata().insert( to_insert );
    return true;
}

bool register_jpg_ok = gilviewer_file_io_jpg::Register();
