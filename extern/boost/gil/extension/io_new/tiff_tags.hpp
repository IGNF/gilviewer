/*
    Copyright 2007-2008 Christian Henning, Andreas Pokorny, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_TIFF_TAGS_HPP
#define BOOST_GIL_EXTENSION_IO_TIFF_TAGS_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief All supported tiff tags by the gil io extension.
/// \author Christian Henning, Andreas Pokorny, Lubomir Bourdev \n
///
/// \date   2007-2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

extern "C" {
#include "tiff.h"
}

#include "detail/base.hpp"
#include "formats/tiff/log.hpp"

namespace boost { namespace gil {

/// Defines tiff tag.
struct tiff_tag : format_tag {};

/// For a description see: http://www.awaresystems.be/imaging/tiff/tifftags/baseline.html
/// see http://www.remotesensing.org/libtiff/
/// or http://www.libtiff.org/man/

/// TIFF property base class
template< typename T, int Value >
struct tiff_property_base : property_base< T >
{
    /// Tag, needed when reading or writing image properties.
    static const unsigned int tag = Value;
};

/// baseline tags

/// Defines type for new subfile property.
struct tiff_new_subfile_type : tiff_property_base< uint32_t, TIFFTAG_SUBFILETYPE > {};

/// Defines type for subfile property.
struct tiff_subfile_type : tiff_property_base< uint16_t, TIFFTAG_OSUBFILETYPE > {};

/// Defines type for image width property.
struct tiff_image_width : tiff_property_base< uint32_t, TIFFTAG_IMAGEWIDTH > {};

/// Defines type for image height property.
struct tiff_image_height : tiff_property_base< uint32_t, TIFFTAG_IMAGELENGTH > {};

/// Defines type for bits per sample property.
struct tiff_bits_per_sample : tiff_property_base< uint16_t, TIFFTAG_BITSPERSAMPLE > {};

/// Defines type for compression property.
struct tiff_compression : tiff_property_base< uint16_t, TIFFTAG_COMPRESSION > {};

/// Defines type for photometric interpretation property.
struct tiff_photometric_interpretation : tiff_property_base< uint16_t, TIFFTAG_PHOTOMETRIC > {};

/// Defines type for threshold property.
struct tiff_thresholding : tiff_property_base< uint16_t, TIFFTAG_THRESHHOLDING > {};

/// Defines type for cell width property.
struct tiff_cell_width : tiff_property_base< uint16_t, TIFFTAG_CELLWIDTH > {};

/// Defines type for cell length property.
struct tiff_cell_length : tiff_property_base< uint16_t, TIFFTAG_CELLLENGTH > {};

/// Defines type for fill order property.
struct tiff_fill_order : tiff_property_base< std::string, TIFFTAG_FILLORDER > {};

/// Defines type for image description.
struct tiff_image_description : tiff_property_base< std::string, TIFFTAG_IMAGEDESCRIPTION > {};

/// Defines type for make property.
struct tiff_make : tiff_property_base< std::string, TIFFTAG_MAKE > {};

/// Defines type for model property.
struct tiff_model : tiff_property_base< std::string, TIFFTAG_MODEL > {};

/// Defines type for image orientation.
struct tiff_orientation : tiff_property_base< uint16_t, TIFFTAG_ORIENTATION > {};

/// Defines type for samples per pixel property.
struct tiff_samples_per_pixel : tiff_property_base< uint16_t, TIFFTAG_SAMPLESPERPIXEL > {};

/// Defines type for rows per strip property.
struct tiff_rows_per_strip : tiff_property_base< uint32_t, TIFFTAG_ROWSPERSTRIP > {};

/// Defines type for min sample property.
struct tiff_min_sample_value : tiff_property_base< uint16_t, TIFFTAG_MINSAMPLEVALUE > {};

/// Defines type for max sample property.
struct tiff_max_sample_value : tiff_property_base< uint16_t, TIFFTAG_MAXSAMPLEVALUE > {};

/// Defines type for x resolution property.
struct tiff_x_resolution : tiff_property_base< float, TIFFTAG_XRESOLUTION > {};

/// Defines type for y resolution property.
struct tiff_y_resolution : tiff_property_base< float, TIFFTAG_YRESOLUTION > {};

/// Defines type for resolution unit property.
struct tiff_resolution_unit : tiff_property_base< uint16_t, TIFFTAG_RESOLUTIONUNIT > {};

/// Defines type for planar configuration property.
struct tiff_planar_configuration : tiff_property_base< uint16_t, TIFFTAG_PLANARCONFIG > {};

/// Defines type for gray response unit property.
struct tiff_gray_response_unit : tiff_property_base< uint16_t, TIFFTAG_GRAYRESPONSEUNIT > {};

/// Defines type for gray response curve property.
struct tiff_gray_response_curve : tiff_property_base< uint16_t*, TIFFTAG_GRAYRESPONSECURVE > {};

/// Defines type for software vendor property.
struct tiff_software : tiff_property_base< std::string, TIFFTAG_SOFTWARE > {};

/// Defines type for date time property.
struct tiff_date_time : tiff_property_base< std::string, TIFFTAG_DATETIME > {};

/// Defines type for artist information property.
struct tiff_artist : tiff_property_base< std::string, TIFFTAG_ARTIST > {};

/// Defines type for host computer property.
struct tiff_host_computer : tiff_property_base< std::string, TIFFTAG_HOSTCOMPUTER > {};

/// Helper structure for reading a color mapper.
struct tiff_color_map
{
   typedef uint16_t* red_t;
   typedef uint16_t* green_t;
   typedef uint16_t* blue_t;

   static const unsigned int tag = TIFFTAG_COLORMAP;
};

/// Defines type for extra samples property.
struct tiff_extra_samples : tiff_property_base< uint16_t*, TIFFTAG_EXTRASAMPLES > {};

/// Defines type for copyright property.
struct tiff_copyright : tiff_property_base< std::string, TIFFTAG_COPYRIGHT > {};

/// non-baseline tags

/// Defines type for sample format property.
struct tiff_sample_format : tiff_property_base< uint16_t, TIFFTAG_SAMPLEFORMAT > {};

/// Defines type for indexed property.
struct tiff_indexed : tiff_property_base< uint16_t, TIFFTAG_INDEXED > {};

/// Tile related tags

/// Defines type for tile width
struct tiff_tile_width : tiff_property_base< uint32_t, TIFFTAG_TILEWIDTH > {};

/// Defines type for tile length
struct tiff_tile_length : tiff_property_base< uint32_t, TIFFTAG_TILELENGTH > {};

/// Defines type for tile offsets
struct tiff_tile_offsets : tiff_property_base< uint32_t, TIFFTAG_TILEOFFSETS > {};

/// Defines type for tile byte counts
struct tiff_tile_byte_counts : tiff_property_base< uint32_t, TIFFTAG_TILEBYTECOUNTS > {};

/// Read information for tiff images.
///
/// The structure is returned when using read_image_info.
template<>
struct image_read_info< tiff_tag >
{
    /// The number of rows of pixels in the image.
    tiff_image_width::type  _width;
    /// The number of columns in the image, i.e., the number of pixels per row.
    tiff_image_height::type _height;

    /// Compression scheme used on the image data.
    tiff_compression::type _compression;

    /// Number of bits per component.
    tiff_bits_per_sample::type   _bits_per_sample;
    /// The number of components per pixel.
    tiff_samples_per_pixel::type _samples_per_pixel;
    /// Specifies how to interpret each data sample in a pixel.
    tiff_sample_format::type     _sample_format;

    /// How the components of each pixel are stored.
    tiff_planar_configuration::type _planar_configuration;

    /// The color space of the image data.
    tiff_photometric_interpretation::type _photometric_interpretation;

    /// Tile width
    tiff_tile_width::type _tile_width;
    /// Tile length
    tiff_tile_length::type _tile_length;
    /// Tile offsets
    tiff_tile_offsets::type _tile_offsets;
    /// Tile byte counts
    tiff_tile_byte_counts::type _tile_byte_counts;
};

/// Read settings for tiff images.
///
/// The structure can be used for all read_xxx functions, except read_image_info.
template<>
struct image_read_settings< tiff_tag > : public image_read_settings_base
{
    /// Default constructor
    image_read_settings< tiff_tag >()
    : image_read_settings_base()
    {}

    /// Constructor
    /// \param top_left Top left coordinate for reading partial image.
    /// \param dim      Dimensions for reading partial image.
    image_read_settings( const point_t& top_left
                       , const point_t& dim
                       )
    : image_read_settings_base( top_left
                              , dim
                              )
    {}
};

/// Read settings for tiff images.
///
/// The structure can be used for all read_xxx functions, except read_image_info.
template< typename Log >
struct image_write_info< tiff_tag, Log >
{
    /// Default constructor
    image_write_info()
    : _photometric_interpretation( PHOTOMETRIC_MINISBLACK )
    , _compression               ( COMPRESSION_NONE       )
    , _orientation               ( ORIENTATION_TOPLEFT    )
    , _planar_configuration      ( PLANARCONFIG_CONTIG    )
    {}

    /// The color space of the image data.
    tiff_photometric_interpretation::type _photometric_interpretation;
    /// Compression scheme used on the image data.
    tiff_compression::type                _compression;
    /// The orientation of the image with respect to the rows and columns.
    tiff_orientation::type                _orientation;
    /// How the components of each pixel are stored.
    tiff_planar_configuration::type       _planar_configuration;

    /// A log to transcript error and warning messages issued by libtiff.
    Log                                   _log;
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_TIFF_TAGS_HPP
