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

#include <boost/variant/static_visitor.hpp>
#include <boost/variant/apply_visitor.hpp>

struct write_view_tiff_visitor : public static_visitor<>
{
    write_view_tiff_visitor(const string& filename) : m_filename(filename) {}

    template <typename ViewType>
    result_type operator()(const ViewType& v) const { write_view( m_filename , v, tiff_tag() ); }

private:
    string m_filename;
};

shared_ptr<layer> gilviewer_file_io_tiff::load(const string &filename)
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

    //image_read_info< tiff_tag > info = read_image_info(filename.string(), tiff_tag());

    image_layer::image_ptr image(new image_layer::image_t);

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
        return layer::ptrLayerType();
    }

	layer::ptrLayerType layer(new image_layer(image, path.stem(), path.string()));

    layer->add_orientation(filename);

    image_read_info< tiff_tag > info = read_image_info(filename, tiff_tag());
    ostringstream infos_str;
    infos_str << "Dimensions: " << info._width << "x" << info._height << "\n";
    infos_str << "Number of components: " << info._samples_per_pixel << "\n";
    infos_str << "Bits per pixel: " << info._bits_per_sample << "\n";
    string compression_str;
    if(info._compression==COMPRESSION_NONE)
        compression_str="COMPRESSION_NONE (dump mode)";
    else if(info._compression==COMPRESSION_CCITTRLE)
        compression_str="COMPRESSION_CCITTRLE (CCITT modified Huffman RLE)";
    else if(info._compression==COMPRESSION_CCITTFAX3)
        compression_str="COMPRESSION_CCITTFAX3 (CCITT Group 3 fax encoding)";
    else if(info._compression==COMPRESSION_CCITT_T4)
        compression_str="COMPRESSION_CCITT_T4 (CCITT T.4 (TIFF 6 name))";
    else if(info._compression==COMPRESSION_CCITTFAX4)
        compression_str="COMPRESSION_CCITTFAX4 (CCITT Group 4 fax encoding)";
    else if(info._compression==COMPRESSION_CCITT_T6)
        compression_str="COMPRESSION_CCITT_T6 (CCITT T.6 (TIFF 6 name))";
    else if(info._compression==COMPRESSION_LZW)
        compression_str="COMPRESSION_LZW (Lempel-Ziv  & Welch)";
    else if(info._compression==COMPRESSION_OJPEG)
        compression_str="COMPRESSION_OJPEG (!6.0 JPEG)";
    else if(info._compression==COMPRESSION_JPEG)
        compression_str="COMPRESSION_JPEG (%JPEG DCT compression)";
    else if(info._compression==COMPRESSION_NEXT)
        compression_str="COMPRESSION_NEXT (NeXT 2-bit RLE)";
    else if(info._compression==COMPRESSION_CCITTRLEW)
        compression_str="COMPRESSION_CCITTRLEW (#1 w/ word alignment)";
    else if(info._compression==COMPRESSION_PACKBITS)
        compression_str="COMPRESSION_PACKBITS (Macintosh RLE)";
    else if(info._compression==COMPRESSION_THUNDERSCAN)
        compression_str="COMPRESSION_THUNDERSCAN (ThunderScan RLE)";
    else if(info._compression==COMPRESSION_IT8CTPAD)
        compression_str="COMPRESSION_IT8CTPAD (IT8 CT w/padding)";
    else if(info._compression==COMPRESSION_IT8LW)
        compression_str="COMPRESSION_IT8LW (IT8 Linework RLE)";
    else if(info._compression==COMPRESSION_IT8MP)
        compression_str="COMPRESSION_IT8MP (IT8 Monochrome picture)";
    else if(info._compression==COMPRESSION_IT8BL)
        compression_str="COMPRESSION_IT8BL (IT8 Binary line art)";
    else if(info._compression==COMPRESSION_PIXARFILM)
        compression_str="COMPRESSION_PIXARFILM (Pixar companded 10bit LZW)";
    else if(info._compression==COMPRESSION_PIXARLOG)
        compression_str="COMPRESSION_PIXARLOG (Pixar companded 11bit ZIP)";
    else if(info._compression==COMPRESSION_DEFLATE)
        compression_str="COMPRESSION_DEFLATE (Deflate compression)";
    else if(info._compression==COMPRESSION_ADOBE_DEFLATE)
        compression_str="COMPRESSION_ADOBE_DEFLATE (Deflate compression, as recognized by Adobe)";
    infos_str << "Compression: " << compression_str << "\n";
    string sample_format_str;
    if(info._sample_format==SAMPLEFORMAT_UINT)
        sample_format_str="SAMPLEFORMAT_UINT";
    else if(info._sample_format==SAMPLEFORMAT_INT)
        sample_format_str="SAMPLEFORMAT_INT";
    else if(info._sample_format==SAMPLEFORMAT_IEEEFP)
        sample_format_str="SAMPLEFORMAT_IEEEFP";
    else if(info._sample_format==SAMPLEFORMAT_VOID)
        sample_format_str="SAMPLEFORMAT_VOID";
    else if(info._sample_format==SAMPLEFORMAT_COMPLEXINT)
        sample_format_str="SAMPLEFORMAT_COMPLEXINT";
    else if(info._sample_format==SAMPLEFORMAT_COMPLEXIEEEFP)
        sample_format_str="SAMPLEFORMAT_COMPLEXIEEEFP";
    infos_str << "Sample format: " << sample_format_str << "\n";
    string planar_config_str;
    if(info._planar_configuration==PLANARCONFIG_CONTIG)
        planar_config_str = "PLANARCONFIG_CONTIG";
    else if(info._planar_configuration==PLANARCONFIG_SEPARATE)
        planar_config_str = "PLANARCONFIG_SEPARATE";
    infos_str << "Planar configuration: " << planar_config_str << "\n";
    string photometric_interpretation_str;
    if(info._photometric_interpretation==PHOTOMETRIC_MINISWHITE)
        photometric_interpretation_str = "PHOTOMETRIC_MINISWHITE";
    else if(info._photometric_interpretation==PHOTOMETRIC_MINISBLACK)
        photometric_interpretation_str = "PHOTOMETRIC_MINISBLACK";
    else if(info._photometric_interpretation==PHOTOMETRIC_RGB)
        photometric_interpretation_str = "PHOTOMETRIC_RGB";
    else if(info._photometric_interpretation==PHOTOMETRIC_PALETTE)
        photometric_interpretation_str = "PHOTOMETRIC_PALETTE (color map indexed)";
    else if(info._photometric_interpretation==PHOTOMETRIC_MASK)
        photometric_interpretation_str = "PHOTOMETRIC_MASK ($holdout mask)";
    else if(info._photometric_interpretation==PHOTOMETRIC_SEPARATED)
        photometric_interpretation_str = "PHOTOMETRIC_SEPARATED (!color separations)";
    else if(info._photometric_interpretation==PHOTOMETRIC_YCBCR)
        photometric_interpretation_str = "PHOTOMETRIC_YCBCR (!CCIR 601)";
    else if(info._photometric_interpretation==PHOTOMETRIC_CIELAB)
        photometric_interpretation_str = "PHOTOMETRIC_CIELAB (!1976 CIE L*a*b*)";
    else if(info._photometric_interpretation==PHOTOMETRIC_ICCLAB)
        photometric_interpretation_str = "PHOTOMETRIC_ICCLAB (ICC L*a*b* [Adobe TIFF Technote 4])";
    else if(info._photometric_interpretation==PHOTOMETRIC_ITULAB)
        photometric_interpretation_str = "PHOTOMETRIC_ITULAB (ITU L*a*b*)";
    else if(info._photometric_interpretation==PHOTOMETRIC_LOGL)
        photometric_interpretation_str = "PHOTOMETRIC_LOGL (CIE Log2(L))";
    else if(info._photometric_interpretation==PHOTOMETRIC_LOGLUV)
        photometric_interpretation_str = "PHOTOMETRIC_LOGLUV (CIE Log2(L) (u',v'))";
    infos_str << "Photometric interpretation: " << photometric_interpretation_str << "\n";
    if(info._is_tiled)
    {
        infos_str << "Image is tiled\n";
        infos_str << "Tile dimensions: " << info._tile_width << "x" << info._tile_length << "\n";
        infos_str << "Tile offsets: " << info._tile_offsets << "\n";
        infos_str << "Tile byte counts: " << info._tile_byte_counts << "\n";
    }
    layer->infos(infos_str.str());

    return layer;
}

void gilviewer_file_io_tiff::save(shared_ptr<layer> layer, const string &filename)
{
    shared_ptr<image_layer> imagelayer = dynamic_pointer_cast<image_layer>(layer);
    if(!imagelayer)
        throw invalid_argument("Bad layer type!\n");

    write_view_tiff_visitor writer(filename);
    try
    {
        apply_visitor( writer, imagelayer->variant_view()->value );
    }
    catch( const std::exception &e )
    {
        ostringstream oss;
        oss << "JPEG write error: "<<filename<< "!\n" << "File: " <<__FILE__ << "\nLine: " << __LINE__ << "\nFunction: " << __FUNCTION__ << endl;
        error_logger::log_wx_log_message(oss.str());
    }
}

boost::shared_ptr<gilviewer_file_io_tiff> create_gilviewer_file_io_tiff()
{
    return shared_ptr<gilviewer_file_io_tiff>(new gilviewer_file_io_tiff());
}

bool gilviewer_file_io_tiff::Register()
{
    gilviewer_io_factory::instance()->Register("tif", create_gilviewer_file_io_tiff);
    gilviewer_io_factory::instance()->Register("tiff", create_gilviewer_file_io_tiff);
    pair<string,string> familly_description = make_pair<string,string>("Image files","TIFF images");
    pair< string, pair<string,string> > to_insert = make_pair< string, pair<string,string> >( "tif", familly_description );
    gilviewer_io_factory::instance()->metadata().insert( to_insert );
    to_insert = make_pair< string, pair<string,string> >( "tiff", familly_description );
    gilviewer_io_factory::instance()->metadata().insert( to_insert );
    return true;
}

bool register_tiff_ok = gilviewer_file_io_tiff::Register();
