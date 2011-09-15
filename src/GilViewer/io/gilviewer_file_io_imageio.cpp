#include "../config/config.hpp"
#if GILVIEWER_USE_IMAGEIO

#include "gilviewer_file_io_imageio.hpp"
#include "gilviewer_io_factory.hpp"
#include <ign/imageio/ImageInput.h>
#include <ign/imageio/ImageOutput.h>
#include <ign/imageio/Exception.h>

using namespace boost;
using namespace boost::gil;
using namespace boost::filesystem;
using namespace std;


// this should be integrated into ImageIO
namespace ign {
    namespace imageio {

        // eTypePixel -> value_type metafunction
        template<eTypePixel t> struct pixel_type {};
        template<> struct pixel_type<eUnsignedChar> { typedef unsigned char type; };
        template<> struct pixel_type<eSignedShort> { typedef unsigned short type; };
        template<> struct pixel_type<eUnsignedShort> { typedef unsigned short type; };
        template<> struct pixel_type<eUnsignedInt> { typedef unsigned int type; };
        template<> struct pixel_type<eFloat> { typedef float type; };
        template<> struct pixel_type<eBool> { typedef bool type; };
        template<> struct pixel_type<eOther> {}; // compilation error if type is accessed !


        // value_type -> eTypePixel metafunction
        template<typename T> struct TypePixel_       { static const eTypePixel value = eOther; };
        template<> struct TypePixel_<unsigned char > { static const eTypePixel value = eUnsignedChar; };
        template<> struct TypePixel_<  signed short> { static const eTypePixel value = eSignedShort; };
        template<> struct TypePixel_<unsigned short> { static const eTypePixel value = eUnsignedShort; };
        template<> struct TypePixel_<unsigned int  > { static const eTypePixel value = eUnsignedInt; };
        template<> struct TypePixel_<float         > { static const eTypePixel value = eFloat; };
        template<> struct TypePixel_<bool          > { static const eTypePixel value = eBool; };

        // eTypePixel -> sizeof metafunction
        template<eTypePixel t> struct pixel_size
        {
            typedef size_t value_type;
            static const value_type value = sizeof(typename pixel_type<t>::type);
            operator value_type() const { return value; }
        };
        template<> struct pixel_size<eBool>
        {
        typedef size_t value_type;
        static const value_type value = 1;
        operator value_type() const { return value; }
    };

        namespace detail
        {
            template<typename Functor, int bands>
            typename Functor::result_type visitor_applier(Functor& f, const ImageInput& image)
            {
                switch(image.Type())
                {
                case eUnsignedChar  : return f.template apply<bands,eUnsignedChar >(image);
                case eSignedShort   : return f.template apply<bands,eSignedShort  >(image);
                case eUnsignedShort : return f.template apply<bands,eUnsignedShort>(image);
                case eUnsignedInt   : return f.template apply<bands,eUnsignedInt  >(image);
                case eFloat         : return f.template apply<bands,eFloat        >(image);
                case eBool          : return f.template apply<bands,eBool         >(image);
                default /* eOther */: return f.error(image);
            }
            };
        }


        template<typename Functor>
        typename Functor::result_type apply_visitor(Functor& f, const ImageInput& image)
        {
            int b = image.NbBands();
            switch(b)
            {
            case 1 : return detail::visitor_applier<Functor,1>(f,image);
            case 2 : return detail::visitor_applier<Functor,2>(f,image);
            case 3 : return detail::visitor_applier<Functor,3>(f,image);
            case 4 : return detail::visitor_applier<Functor,4>(f,image);
            case 5 : return detail::visitor_applier<Functor,5>(f,image);
            default: return f.error(image);
            }
        }
    }
} // namespace ign::imageio


template <typename Pixel> struct channel_value_type {};
template <typename ChannelValue, typename Layout>
        struct channel_value_type<boost::gil::pixel<ChannelValue,Layout> > { typedef ChannelValue type; };

// value_type -> gil radical metafunction
template<typename T> struct gil_code {};
template<> struct gil_code<unsigned char >  { static const char *value; };
template<> struct gil_code<  signed short>  { static const char *value; };
template<> struct gil_code<unsigned short>  { static const char *value; };
template<> struct gil_code<unsigned int  >  { static const char *value; };
template<> struct gil_code<float         >  { static const char *value; };
template<> struct gil_code<bool          >  { static const char *value; };
const char *gil_code<unsigned char >::value = "8";
const char *gil_code<  signed short>::value = "16s";
const char *gil_code<unsigned short>::value = "16";
const char *gil_code<unsigned int  >::value = "32";
const char *gil_code<float         >::value = "32F";  // not sure... "32f" ??
const char *gil_code<bool          >::value = "8"; // not sure

struct image_copier : public boost::static_visitor<layer::ptrLayerType>
{
    int x0, y0, w, h;
    std::string name;
    image_copier(int x0_, int y0_, int w_, int h_, std::string name_)
        : x0(x0_), y0(y0_), w(w_), h(h_), name(name_) {}

    result_type error(const ign::imageio::ImageInput&)
    {
        GILVIEWER_LOG_ERROR("ImageIO is unable to open the image: " << name);
        return result_type();
    }

    template<int bands, ign::imageio::eTypePixel t>
    result_type apply(const ign::imageio::ImageInput& image)
    {
        typedef boost::gil::devicen_layout_t<bands> layout_type;
        typedef typename ign::imageio::pixel_type<t>::type value_type;
        typedef boost::gil::pixel<value_type,layout_type> pixel_type;

        boost::gil::image<pixel_type, false> img(w,h);
        value_type *buf = boost::gil::interleaved_view_get_raw_data(boost::gil::view(img));
        image.Read(x0,y0,0,w,h,bands,1,buf,t,bands,bands*w,1);
        image_layer::image_ptr image_ptr(new image_layer::image_t);
        try {
            image_ptr->value = img;
        }
        catch(const std::exception &e){
            GILVIEWER_LOG_EXCEPTION("ImageIO is unable to pass this image to GilViewer: " << name << std::endl
                                    << "Fix this by adding the type dev"<<bands<<"n"<<gil_code<value_type>::value
                                    <<"_image_t to the device_image_types of GilViewer/layers/image_types.hpp");
            return result_type();
        }
        return result_type(new image_layer(image_ptr, name, name));
    }

};


shared_ptr<layer> gilviewer_file_io_imageio::load(const string &filename, const ptrdiff_t top_left_x, const ptrdiff_t top_left_y, const ptrdiff_t dim_x, const ptrdiff_t dim_y)
{

    ign::imageio::ImageInput image_input(filename);
    if (!image_input.Valide())
    {
        GILVIEWER_LOG_ERROR("Image is invalid!!!");
        return shared_ptr<layer>();
    }
    int width  = image_input.Size().real();
    int height = image_input.Size().imag();

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

    image_copier ic(x0,y0,w,h,filename);
    layer::ptrLayerType layer = ign::imageio::apply_visitor(ic,image_input);

    if(layer)
    {
        layer->add_orientation(filename);
        layer->infos( build_and_get_infos(filename) );
    }
    return layer;
}

struct write_imageio_view_visitor : public boost::static_visitor<>
{
    std::string m_filename;
    write_imageio_view_visitor(const std::string& s) : m_filename(s) {}
    template <typename ViewType>
            result_type operator()(const ViewType& v) const
    {
        int w = v.width();
        int h = v.height();
        int c = v.num_channels();
        typedef typename ViewType::value_type pixel_type;
        typedef typename channel_value_type<pixel_type>::type value_type;
        ign::imageio::eTypePixel t = ign::imageio::TypePixel_<value_type>::value;

        pixel_type *buf = new pixel_type[w*h];
        boost::gil::copy_pixels(v,boost::gil::interleaved_view(w,h,buf,w*sizeof(pixel_type)));

        std::string file = m_filename+".tif";
        ign::imageio::ImageOutput out(file,w,h,c,t);
        out.Write(0,0,0,w,h,c,(void *)buf,t,c,w*c,1);
        delete [] buf;
    }
    template <typename T> result_type operator()(const boost::gil::any_image_view<T>& v) const
    {
        boost::gil::apply_operation(v,*this);
    }
};

void gilviewer_file_io_imageio::save(shared_ptr<layer> layer, const string &filename)
{
    shared_ptr<image_layer> imagelayer = dynamic_pointer_cast<image_layer>(layer);
    if(!imagelayer)
        throw std::invalid_argument("Bad layer type (not an image layer)!\n");
    try
    {
        write_imageio_view_visitor writer(filename);
        boost::apply_visitor( writer, imagelayer->variant_view()->value );
    }
    catch( ign::imageio::Exception &e )
    {
        GILVIEWER_LOG_EXCEPTION("ImageIO is unable to write the image: " << filename);
    }
}

string gilviewer_file_io_imageio::build_and_get_infos(const std::string &filename)
{
    ign::imageio::ImageInput img(filename);
    if (!img.Valide())
        return "ImageIO is unable to open the file "+filename;

    ostringstream infos_str;
    infos_str << "Dimensions: " << img.Size().real() << "x" << img.Size().imag() << "\n";
    infos_str << "Number of components: " << img.NbBands() << "\n";
    infos_str << "Pixel type: " << ign::imageio::TypeToString(img.Type()) << "\n";
    return infos_str.str();

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
