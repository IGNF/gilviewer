#include "gilviewer_file_io_tiff.hpp"
#include "gilviewer_io_factory.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/gil/extension/io_new/tiff_all.hpp>

#include "../layers/ImageLayer.hpp"
#include "../layers/image_types.hpp"
#include "../tools/error_logger.hpp"

using namespace boost;
using namespace boost::gil;
using namespace boost::filesystem;
using namespace std;

shared_ptr<Layer> gilviewer_file_io_tiff::load(const string &filename)
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

    //image_read_info< tiff_tag > info = read_image_info(filename.string(), tiff_tag());

    ImageLayer::image_ptr image(new ImageLayer::image_t);

    try
    {
        read_image(filename
                   , image->value
                   , tiff_tag());
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

void gilviewer_file_io_tiff::save(shared_ptr<Layer> layer, const string &filename)
{
    shared_ptr<ImageLayer> image_layer = dynamic_pointer_cast<ImageLayer>(layer);
    if(!image_layer)
        throw invalid_argument("Bad layer type!\n");

    write_view( filename
              , image_layer->View()->value
              , gil::tiff_tag()
              );
}

boost::shared_ptr<gilviewer_file_io_tiff> create_gilviewer_file_io_tiff()
{
    return shared_ptr<gilviewer_file_io_tiff>(new gilviewer_file_io_tiff());
}

bool gilviewer_file_io_tiff::Register()
{
    gilviewer_io_factory::Instance()->Register("tif", create_gilviewer_file_io_tiff);
    gilviewer_io_factory::Instance()->Register("tiff", create_gilviewer_file_io_tiff);
    return true;
}

bool register_tiff_ok = gilviewer_file_io_tiff::Register();
