#include "gilviewer_file_io_tiff.hpp"
#include "gilviewer_io_factory.hpp"

using namespace boost;
using namespace boost::gil;
using namespace boost::filesystem;
using namespace std;

string gilviewer_file_io_tiff::get_infos(const std::string &filename)
{
    if(!_info_read)
        _info = read_image_info(filename, tiff_tag());
    string compression_str, sample_format_str, planar_config_str, photometric_interpretation_str;

    switch(_info._compression)
    {
    case COMPRESSION_NONE:          compression_str = "COMPRESSION_NONE (dump mode)"; break;
    case COMPRESSION_CCITTRLE:      compression_str = "COMPRESSION_CCITTRLE (CCITT modified Huffman RLE)"; break;
    case COMPRESSION_CCITTFAX3:     compression_str = "COMPRESSION_CCITTFAX3 (CCITT Group 3 fax encoding)"; break;
	//case COMPRESSION_CCITT_T4:      compression_str = "COMPRESSION_CCITT_T4 (CCITT T.4 (TIFF 6 name))"; break;
    case COMPRESSION_CCITTFAX4:     compression_str = "COMPRESSION_CCITTFAX4 (CCITT Group 4 fax encoding)"; break;
	//case COMPRESSION_CCITT_T6:      compression_str = "COMPRESSION_CCITT_T6 (CCITT T.6 (TIFF 6 name))"; break;
    case COMPRESSION_LZW:           compression_str = "COMPRESSION_LZW (Lempel-Ziv  & Welch)"; break;
    case COMPRESSION_OJPEG:         compression_str = "COMPRESSION_OJPEG (!6.0 JPEG)"; break;
    case COMPRESSION_JPEG:          compression_str = "COMPRESSION_JPEG (%JPEG DCT compression)"; break;
    case COMPRESSION_NEXT:          compression_str = "COMPRESSION_NEXT (NeXT 2-bit RLE)"; break;
    case COMPRESSION_CCITTRLEW:     compression_str = "COMPRESSION_CCITTRLEW (#1 w/ word alignment)"; break;
    case COMPRESSION_PACKBITS:      compression_str = "COMPRESSION_PACKBITS (Macintosh RLE)"; break;
    case COMPRESSION_THUNDERSCAN:   compression_str = "COMPRESSION_THUNDERSCAN (ThunderScan RLE)"; break;
    case COMPRESSION_IT8CTPAD:      compression_str = "COMPRESSION_IT8CTPAD (IT8 CT w/padding)"; break;
    case COMPRESSION_IT8LW:         compression_str = "COMPRESSION_IT8LW (IT8 Linework RLE)"; break;
    case COMPRESSION_IT8MP:         compression_str = "COMPRESSION_IT8MP (IT8 Monochrome picture)"; break;
    case COMPRESSION_IT8BL:         compression_str = "COMPRESSION_IT8BL (IT8 Binary line art)"; break;
    case COMPRESSION_PIXARFILM:     compression_str = "COMPRESSION_PIXARFILM (Pixar companded 10bit LZW)"; break;
    case COMPRESSION_PIXARLOG:      compression_str = "COMPRESSION_PIXARLOG (Pixar companded 11bit ZIP)"; break;
    case COMPRESSION_DEFLATE:       compression_str = "COMPRESSION_DEFLATE (Deflate compression)"; break;
    case COMPRESSION_ADOBE_DEFLATE: compression_str = "COMPRESSION_ADOBE_DEFLATE (Deflate compression, as recognized by Adobe)"; break;
    case COMPRESSION_DCS:           compression_str = "Kodak DCS encoding"; break;
    case COMPRESSION_JBIG:          compression_str = "ISO JBIG"; break;
    case COMPRESSION_SGILOG:        compression_str = "SGI Log Luminance RLE"; break;
    case COMPRESSION_SGILOG24:      compression_str = "SGI Log 24-bit packed"; break;
    case COMPRESSION_JP2000:        compression_str = "Leadtools JPEG2000"; break;
    default:                        compression_str = "Unknow..."; break;
    }

    switch(_info._sample_format)
    {
    case SAMPLEFORMAT_UINT:          sample_format_str = "SAMPLEFORMAT_UINT"; break;
    case SAMPLEFORMAT_INT:           sample_format_str = "SAMPLEFORMAT_INT"; break;
    case SAMPLEFORMAT_IEEEFP:        sample_format_str = "SAMPLEFORMAT_IEEEFP"; break;
    case SAMPLEFORMAT_VOID:          sample_format_str = "SAMPLEFORMAT_VOID"; break;
    case SAMPLEFORMAT_COMPLEXINT:    sample_format_str = "SAMPLEFORMAT_COMPLEXINT"; break;
    case SAMPLEFORMAT_COMPLEXIEEEFP: sample_format_str = "SAMPLEFORMAT_COMPLEXIEEEFP"; break;
    default:                         sample_format_str = "Unknow..."; break;
    }

    switch(_info._planar_configuration)
    {
    case PLANARCONFIG_CONTIG:   planar_config_str = "SAMPLEFORMAT_UINT"; break;
    case PLANARCONFIG_SEPARATE: planar_config_str = "SAMPLEFORMAT_INT"; break;
    default:                    planar_config_str = "Unknow..."; break;
    }

    switch(_info._photometric_interpretation)
    {
    case PHOTOMETRIC_MINISWHITE: photometric_interpretation_str = "PHOTOMETRIC_MINISWHITE"; break;
    case PHOTOMETRIC_MINISBLACK: photometric_interpretation_str = "PHOTOMETRIC_MINISBLACK"; break;
    case PHOTOMETRIC_RGB:        photometric_interpretation_str = "PHOTOMETRIC_RGB"; break;
    case PHOTOMETRIC_PALETTE:    photometric_interpretation_str = "PHOTOMETRIC_PALETTE (color map indexed)"; break;
    case PHOTOMETRIC_MASK:       photometric_interpretation_str = "PHOTOMETRIC_MASK ($holdout mask)"; break;
    case PHOTOMETRIC_SEPARATED:  photometric_interpretation_str = "PHOTOMETRIC_SEPARATED (!color separations)"; break;
    case PHOTOMETRIC_YCBCR:      photometric_interpretation_str = "PHOTOMETRIC_YCBCR (!CCIR 601)"; break;
    case PHOTOMETRIC_CIELAB:     photometric_interpretation_str = "PHOTOMETRIC_CIELAB (!1976 CIE L*a*b*)"; break;
    case PHOTOMETRIC_ICCLAB:     photometric_interpretation_str = "PHOTOMETRIC_ICCLAB (ICC L*a*b* [Adobe TIFF Technote 4])"; break;
    case PHOTOMETRIC_ITULAB:     photometric_interpretation_str = "PHOTOMETRIC_ITULAB (ITU L*a*b*)"; break;
    case PHOTOMETRIC_LOGL:       photometric_interpretation_str = "COMPRESSION_NEXT (NeXT 2-bit RLE)"; break;
    case PHOTOMETRIC_LOGLUV:     photometric_interpretation_str = "PHOTOMETRIC_LOGL (CIE Log2(L))"; break;
    default:                     photometric_interpretation_str = "Unknow..."; break;
    }

    ostringstream infos_str;
    infos_str << "Dimensions: " << _info._width << "x" << _info._height << "\n";
    infos_str << "Number of components: " << _info._samples_per_pixel << "\n";
    infos_str << "Bits per pixel: " << _info._bits_per_sample << "\n";
    infos_str << "Compression: " << compression_str << "\n";
    infos_str << "Sample format: " << sample_format_str << "\n";
    infos_str << "Planar configuration: " << planar_config_str << "\n";
    infos_str << "Photometric interpretation: " << photometric_interpretation_str << "\n";

    if(_info._is_tiled)
    {
        infos_str << "Image is tiled\n";
        infos_str << "Tile dimensions: " << _info._tile_width << "x" << _info._tile_length << "\n";
    }

    return infos_str.str();
}

boost::shared_ptr<gilviewer_file_io_tiff> create_gilviewer_file_io_tiff()
{
    return boost::shared_ptr<gilviewer_file_io_tiff>(new gilviewer_file_io_tiff());
}

bool gilviewer_file_io_tiff::Register(gilviewer_io_factory *factory)
{
    factory->insert("tif",  "Image","TIFF", create_gilviewer_file_io_tiff);
    factory->insert("tiff", "Image","TIFF", create_gilviewer_file_io_tiff);
    return true;
}
