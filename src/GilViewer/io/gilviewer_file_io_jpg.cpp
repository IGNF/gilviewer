#include "gilviewer_file_io_jpg.hpp"
#include "gilviewer_io_factory.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/gil/extension/io_new/jpeg_all.hpp>

#include "../layers/ImageLayer.hpp"
#include "../layers/image_types.hpp"
#include "../tools/error_logger.hpp"

using namespace boost;
using namespace boost::gil;
using namespace boost::filesystem;
using namespace std;

shared_ptr<Layer> gilviewer_file_io_jpg::load(const string &filename)
{
    if ( !exists(filename) )
    {
        ostringstream oss;
        oss << "File does not exist: "<<filename<< "!\n" << "File: " <<__FILE__ << "\nLine: " << __LINE__ << "\nFunction: " << __FUNCTION__ << endl;
        error_logger::log_wx_log_message(oss.str());
        return Layer::ptrLayerType();
    }

    path path(system_complete(filename));
    string ext(path.extension());
    to_lower(ext);

    //image_read_info< jpeg_tag > info = read_image_info(filename.string(), jpeg_tag());

    ImageLayer::image_ptr image(new ImageLayer::image_t);

    try
    {
        read_image(filename
                   , image->value
                   , jpeg_tag());
    }
    catch( const std::exception &e )
    {
        ostringstream oss;
        oss << "Read error: "<<filename<< "!\n" << "File: " <<__FILE__ << "\nLine: " << __LINE__ << "\nFunction: " << __FUNCTION__ << endl;
        oss << e.what() << endl;
        error_logger::log_wx_log_message(oss.str());
        return Layer::ptrLayerType();
    }

    Layer::ptrLayerType layer(new ImageLayer(image, path.stem(), path.string()));
    layer->add_orientation(filename);

    return layer;
}

void gilviewer_file_io_jpg::save(shared_ptr<Layer> layer, const string &filename)
{
    shared_ptr<ImageLayer> image_layer = dynamic_pointer_cast<ImageLayer>(layer);
    if(!image_layer)
        throw invalid_argument("Bad layer type!\n");

    try
    {
        write_view( filename
                    , image_layer->View()->value
                    , gil::jpeg_tag()
                    );
    }
    catch( const std::exception &e )
    {
        ostringstream oss;
        oss << "Write error: "<<filename<< "!\n" << "File: " <<__FILE__ << "\nLine: " << __LINE__ << "\nFunction: " << __FUNCTION__ << endl;
        error_logger::log_wx_log_message(oss.str());
    }
}

boost::shared_ptr<gilviewer_file_io_jpg> create_gilviewer_file_io_jpg()
{
    return shared_ptr<gilviewer_file_io_jpg>(new gilviewer_file_io_jpg());
}

bool gilviewer_file_io_jpg::Register()
{
    gilviewer_io_factory::Instance()->Register("jpg", create_gilviewer_file_io_jpg);
    gilviewer_io_factory::Instance()->Register("jpeg", create_gilviewer_file_io_jpg);
    return true;
}

bool register_jpg_ok = gilviewer_file_io_jpg::Register();
