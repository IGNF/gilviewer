#include "ImageEXR.hpp"
#include "GilViewer/io/gilviewer_io_factory.hpp"

//#include "test_exrheader.h"

using namespace boost;
using namespace boost::filesystem;
using namespace std;

#include "GilViewer/plugins/plugin_base.hpp"
IMPLEMENT_PLUGIN(gilviewer_file_io_exr)

namespace ign{ namespace imageio
    {
    namespace detail
    {
    template<typename Functor, int bands>
    typename Functor::result_type visitor_applier(Functor& f, Imf::InputFile& image, Imf::PixelType type)
    {

        switch(type)
        {
        case Imf::UINT  : return f.template apply<bands,unsigned int >(image);
        case Imf::HALF  : return f.template apply<bands,float/*half*/>(image);
        case Imf::FLOAT : return f.template apply<bands,float>(image);
        default /* eOther */: return f.error(image);
        }
    }
    }// namespace ign::imageio::detail

    template<typename Functor>
    typename Functor::result_type apply_visitor(Functor& f, Imf::InputFile& image, const int components, Imf::PixelType type)
    {
        switch(components)
        {
        case 1 : return detail::visitor_applier<Functor,1>(f,image, type);
        case 2 : return detail::visitor_applier<Functor,2>(f,image, type);
        case 3 : return detail::visitor_applier<Functor,3>(f,image, type);
        case 4 : return detail::visitor_applier<Functor,4>(f,image, type);
        default: return f.error(image);
        }
    }
    }}// namespace ign::imageio

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
    Imf::PixelType exr_pixel_type;
    image_copier(int x0_, int y0_, int w_, int h_, std::string name_, Imf::PixelType exr_pixtype)
        : x0(x0_), y0(y0_), w(w_), h(h_), name(name_), exr_pixel_type(exr_pixtype){}

    result_type error(const Imf::InputFile&)
    {
        GILVIEWER_LOG_ERROR("ImageIO is unable to open the image: " << name);
        return result_type();
    }

    template<int bands, typename value_type>
    result_type apply(Imf::InputFile& image)
    {
        typedef boost::gil::devicen_layout_t<bands> layout_type;
        typedef boost::gil::pixel<value_type,layout_type> pixel_type;

        boost::gil::image<pixel_type, false> img(w,h);
        value_type *buf = boost::gil::interleaved_view_get_raw_data(boost::gil::view(img));

        Imath::Box2i dw = image.header().dataWindow();
        int width = dw.max.x - dw.min.x + 1;
        //        int height = dw.max.y - dw.min.y + 1;


        if(exr_pixel_type == Imf::HALF)
        {
            exr_pixel_type = Imf::FLOAT;
            GILVIEWER_LOG_MESSAGE("EXR HALF type (16 bit) : converted to FLOAT (32 bit)." );
        }

        Imf::FrameBuffer frameBuffer;
        const Imf::ChannelList& channels = image.header().channels();
        int channeloffset = 0;
        for (Imf::ChannelList::ConstIterator i = channels.begin(); i != channels.end(); ++i)
        {
            if(strcmp(i.name(),"R") == 0) channeloffset=0;
            else if(strcmp(i.name(),"G") == 0) channeloffset=1;
            else if(strcmp(i.name(),"B") == 0) channeloffset=2;
            else if(strcmp(i.name(),"Y") == 0) channeloffset=0;
            else if(strcmp(i.name(),"A") == 0) channeloffset=bands-1;
            else { channeloffset=0; GILVIEWER_LOG_ERROR("Unsupported EXR band (" << i.name() << ") ; affected to channel 0." ); }
            frameBuffer.insert (i.name(), Imf::Slice(exr_pixel_type,(char *) (buf+channeloffset),sizeof (*buf) * bands,sizeof (*buf) * width * bands, 1, 1, 0.0));
        }

        image.setFrameBuffer(frameBuffer);
        image.readPixels (dw.min.y, dw.max.y);

        image_layer::image_ptr image_ptr(new image_layer::image_t);
        try
        {
            image_ptr->value = img;
        }
        catch(const std::exception &e)
        {
            GILVIEWER_LOG_EXCEPTION("EXR is unable to pass this image to GilViewer: " << name << std::endl
                                    << "Fix this by adding the type dev"<<bands<<"n"<< gil_code<value_type>::value
                                    <<"_image_t to the device_image_types of GilViewer/layers/image_types.hpp" << "\n" << e.what());
            return result_type();
        }
        return result_type(new image_layer(image_ptr, name, name));
    }

};


shared_ptr<layer> gilviewer_file_io_exr::load(const string &filename, const ptrdiff_t top_left_x, const ptrdiff_t top_left_y, const ptrdiff_t dim_x, const ptrdiff_t dim_y)
{

    int width=0, height=0;
    ReadEXRHeader(filename, width, height);

    if (width == 0 || height == 0)
    {
        GILVIEWER_LOG_ERROR("EXR Image is invalid!!!");
        return shared_ptr<layer>();
    }


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

    Imf::InputFile image_input(filename.c_str());
    image_copier ic(x0,y0,w,h,filename, exr_pixel_type);
    layer::ptrLayerType layer = ign::imageio::apply_visitor(ic, image_input, components, exr_pixel_type);

    if(layer)
    {
        layer->add_orientation(filename);
        layer->infos( build_and_get_infos(filename) );
    }

    return layer;
}


void gilviewer_file_io_exr::ReadEXRHeader(const std::string adresse, int& width, int& height)
//si width et height ne sont pas passés par référence, ils ont des valeurs aberrantes
{
    Imf::InputFile image (adresse.c_str());
    exr_pixel_type = Imf::FLOAT;
    components = 0;
    const Imf::ChannelList& channels = image.header().channels();
    for (Imf::ChannelList::ConstIterator i = channels.begin(); i != channels.end(); ++i)
    {
        exr_pixel_type = i.channel().type;
        exr_channels += i.name(); exr_channels += " ";
        ++components;
    }
    cout << " Channels in image " << adresse << " : " << exr_channels;
    switch(exr_pixel_type)
    {
    case Imf::UINT  : std::cerr << " ; type UINT"; break;
    case Imf::HALF  : std::cerr << " ; type HALF"; break;
    case Imf::FLOAT : std::cerr << " ; type FLOAT"; break;
    default: std::cerr << "Unknown EXR pixel type!" << std::endl;
    }
    std::cerr << endl;

    Imath::Box2i dw = image.header().dataWindow();
    width = dw.max.x - dw.min.x + 1;
    height = dw.max.y - dw.min.y + 1;

}


void gilviewer_file_io_exr::save(shared_ptr<layer> layer, const string &filename)
{
    std::cout<<"TO DO";
    GILVIEWER_LOG_ERROR("Saving EXR image : not implemented.");
}

string gilviewer_file_io_exr::build_and_get_infos(const std::string &filename)
{
    Imf::InputFile file (filename.c_str());

    Imath::Box2i dw = file.header().dataWindow();
    int width = dw.max.x - dw.min.x + 1;
    int height = dw.max.y - dw.min.y + 1;


    ostringstream infos_str;
    infos_str << "Dimensions: " << width << "x" << height << "\n";
    infos_str << "Number of components: " << components << "\n";
    infos_str << "Components: " << exr_channels << "\n";
    infos_str << "Pixel type: ";
    switch(exr_pixel_type)
    {
    case 0 : infos_str << "unsigned int (32 bit)" << "\n"; break;
    case 1 : infos_str << "half (16 bit floating point)" << "\n"; break;
    case 2 : infos_str << "float (32 bit floating point)" << "\n"; break;
    default: infos_str << "Unknown" << "\n"; break;
    }

    return infos_str.str();

}

shared_ptr<gilviewer_file_io_exr> create_gilviewer_file_io_exr()
{
    return shared_ptr<gilviewer_file_io_exr>(new gilviewer_file_io_exr());
}

bool gilviewer_file_io_exr::Register(gilviewer_io_factory *factory)
{

    factory->insert("exr", "Image","EXR", create_gilviewer_file_io_exr);
    return true;
}

