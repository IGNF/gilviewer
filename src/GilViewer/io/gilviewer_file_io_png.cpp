#include "gilviewer_file_io_png.hpp"
#include "gilviewer_io_factory.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/gil/extension/io_new/png_all.hpp>

#include "../layers/ImageLayer.hpp"
#include "../layers/image_types.hpp"
#include "../tools/error_logger.hpp"

using namespace boost;
using namespace boost::gil;
using namespace boost::filesystem;
using namespace std;

#include <boost/variant/static_visitor.hpp>
#include <boost/variant/apply_visitor.hpp>

struct write_view_png_visitor : public static_visitor<>
{
    write_view_png_visitor(const string& filename) : m_filename(filename) {}

    template <typename ViewType>
    result_type operator()(const ViewType& v) const { write_view( m_filename , v, png_tag() ); }

private:
    string m_filename;
};

shared_ptr<layer> gilviewer_file_io_png::load(const string &filename)
{
    if ( !exists(filename) )
    {
        ostringstream oss;
        oss << "File does not exist: "<<filename<< "!\n" << "File: " <<__FILE__ << "\nLine: " << __LINE__ << "\nFunction: " << __FUNCTION__ << endl;
        error_logger::log_wx_log_message(oss.str());
        return layer::ptrLayerType();
    }

    path path(system_complete(filename));
    string ext(path.extension());

    //image_read_info< png_tag > info = read_image_info(filename.string(), png_tag());

    image_layer::image_ptr image(new image_layer::image_t);

    try
    {
        read_image(filename
                   , image->value
                   , png_tag());
    }
    catch( const std::exception &e )
    {
        ostringstream oss;
        oss << "PNG read error: " << filename << endl;
        oss << e.what() << endl;
		oss << "File: " << __FILE__ << endl;
		oss << "Line: " << __LINE__ << endl;
		oss << "Function: " << __FUNCTION__ << endl;
        error_logger::log_wx_log_message(oss.str());
        return layer::ptrLayerType();
    }

	layer::ptrLayerType layer(new image_layer(image, path.stem(), path.string()));
    layer->add_orientation(filename);

    return layer;
}

void gilviewer_file_io_png::save(shared_ptr<layer> layer, const string &filename)
{
    shared_ptr<image_layer> imagelayer = dynamic_pointer_cast<image_layer>(layer);
    if(!imagelayer)
        throw invalid_argument("Bad layer type!\n");

    write_view_png_visitor writer(filename);
    try
    {
        apply_visitor( writer, imagelayer->variant_view()->value );
    }
    catch( const std::exception &e )
    {
        ostringstream oss;
        oss << "PNG write error: " << filename << endl;
		oss << e.what() << endl;
		oss << "File: " << __FILE__ << endl;
		oss << "Line: " << __LINE__ << endl;
		oss << "Function: " << __FUNCTION__ << endl;
        error_logger::log_wx_log_message(oss.str());
    }
}

boost::shared_ptr<gilviewer_file_io_png> create_gilviewer_file_io_png()
{
    return shared_ptr<gilviewer_file_io_png>(new gilviewer_file_io_png());
}

bool gilviewer_file_io_png::Register()
{
    gilviewer_io_factory::instance()->Register("png", create_gilviewer_file_io_png);
    pair<string,string> familly_description = make_pair<string,string>("Image files","PNG images");
    pair< string, pair<string,string> > to_insert = make_pair< string, pair<string,string> >( "png", familly_description );
    gilviewer_io_factory::instance()->metadata().insert( to_insert );
    return true;
}

bool register_png_ok = gilviewer_file_io_png::Register();
