#include "../config/config.hpp"
#if GILVIEWER_USE_IMAGEIO

#include "gilviewer_file_io_imageio.hpp"
#include "gilviewer_io_factory.hpp"
#include <ign/imageio/ImageInput.h>

using namespace boost;
using namespace boost::gil;
using namespace boost::filesystem;
using namespace std;

template<typename Value, typename Layout>
void image_copy(image_layer::image_ptr& p, void *buf, int w, int h, int linespace)
{
    typedef boost::gil::pixel<Value,Layout> Pixel;
    boost::gil::image<Pixel, false> img(w,h);
    boost::gil::copy_pixels( boost::gil::interleaved_view(w,h, (Pixel *)buf, linespace) , boost::gil::view(img) );
    p->value = img;
}

template<typename Layout>
void image_copy(ign::imageio::eTypePixel t, image_layer::image_ptr& p, void *buf, int w, int h, int linespace)
{
    switch(t)
    {
    case ign::imageio::eUnsignedChar : image_copy<unsigned char,Layout>(p,buf,w,h,linespace); break;
    case ign::imageio::eSignedShort  : image_copy<signed short,Layout>(p,buf,w,h,linespace); break;
    case ign::imageio::eUnsignedShort : image_copy<unsigned short,Layout>(p,buf,w,h,linespace); break;
    case ign::imageio::eUnsignedInt : image_copy<unsigned int,Layout>(p,buf,w,h,linespace); break;
    case ign::imageio::eFloat : image_copy<float,Layout>(p,buf,w,h,linespace); break;
    case ign::imageio::eBool : image_copy<bool,Layout>(p,buf,w,h,linespace); break;
    case ign::imageio::eOther : break;
    }
}

namespace ign {
    namespace imageio {
        size_t sizeof_pixeltype(ign::imageio::eTypePixel t)
        {
            switch(t)
            {
            case eUnsignedChar : return sizeof(unsigned char);
            case eSignedShort : return sizeof(signed short);
            case eUnsignedShort : return sizeof(unsigned short);
            case eUnsignedInt : return sizeof(unsigned int);
            case eFloat : return sizeof(float);
            case eBool : return sizeof(bool);
            case eOther : return 0;
            }
            return 0;
        }
    }
}


shared_ptr<layer> gilviewer_file_io_imageio::load(const string &filename, const ptrdiff_t top_left_x, const ptrdiff_t top_left_y, const ptrdiff_t dim_x, const ptrdiff_t dim_y)
{

    ign::imageio::ImageInput image_input(filename);
    if (!image_input.Valide())
    {
        GILVIEWER_LOG_ERROR("Image is invalid!!!");
        return shared_ptr<layer>();
    }
    ign::imageio::eTypePixel pixel_type = image_input.Type();
    int width  = image_input.Size().real();
    int height = image_input.Size().imag();
    int bands  = image_input.NbBands();

    int x0 = top_left_x;
    int y0 = top_left_y;
    int x1 = top_left_x+dim_x;
    int y1 = top_left_y+dim_y;
    if(x0<0) x0=0;
    if(y0<0) y0=0;
    if(x1>width  || dim_x==0) x1=width;
    if(y1>height || dim_y==0) y1=height;
    int w = x1-x0;
    int h = y1-y0;

    int dezoom = 1;
    int linespace = bands*w;
    int bandspace = 1;
    // todo: this only works if all bands have the same type : should use image.Type(0), image.Type(1)...
    size_t pixel_size = ign::imageio::sizeof_pixeltype(pixel_type);
    void *buf = new char[w*h*bands*pixel_size];
    image_input.Read(x0,y0,0,w,h,bands,dezoom,buf,pixel_type,bands,linespace,bandspace);

    image_layer::image_ptr image(new image_layer::image_t);
    switch(bands)
    {
    case 1 : image_copy<boost::gil::gray_layout_t>(pixel_type,image,buf,w,h,linespace*pixel_size); break;
    case 3 : image_copy<boost::gil:: rgb_layout_t>(pixel_type,image,buf,w,h,linespace*pixel_size); break;
    case 4 : image_copy<boost::gil::rgba_layout_t>(pixel_type,image,buf,w,h,linespace*pixel_size); break;
    default :
        GILVIEWER_LOG_ERROR("bands!=1, 3 or 4 !!!");
        return layer::ptrLayerType();
    }

    layer::ptrLayerType layer(new image_layer(image, filename, filename));
    layer->add_orientation(filename);
    layer->infos( build_and_get_infos(filename) );
    GILVIEWER_LOG_ERROR("OK" << pixel_type << bands);
    return layer;
}

void gilviewer_file_io_imageio::save(shared_ptr<layer> layer, const string &filename)
{
    //save_gil_view<imageio_tag>(layer, filename);
}

string gilviewer_file_io_imageio::build_and_get_infos(const std::string &filename)
{
    // TODO
    return "TODO";
}

shared_ptr<gilviewer_file_io_imageio> create_gilviewer_file_io_imageio()
{
    return shared_ptr<gilviewer_file_io_imageio>(new gilviewer_file_io_imageio());
}

bool gilviewer_file_io_imageio::Register()
{
    gilviewer_io_factory::instance()->Register("imageio", create_gilviewer_file_io_imageio);
    pair<string,string> familly_description = make_pair<string,string>("Image files","IMAGEIO images");
    pair< string, pair<string,string> > to_insert = make_pair< string, pair<string,string> >( "imageio", familly_description );
    gilviewer_io_factory::instance()->metadata().insert( to_insert );
    return true;
}

bool register_imageio_ok = gilviewer_file_io_imageio::Register();

#endif // GILVIEWER_USE_IMAGEIO
