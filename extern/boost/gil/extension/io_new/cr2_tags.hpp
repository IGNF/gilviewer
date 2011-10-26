#ifndef BOOST_GIL_EXTENSION_IO_CR2_TAGS_HPP
#define BOOST_GIL_EXTENSION_IO_CR2_TAGS_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief All supported cr2 tags by the gil io extension.
/// \author Olivier Tournaire \n
///
/// \date   2011 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include "detail/base.hpp"

namespace boost { namespace gil {

/// Defines tiff tag.
struct cr2_tag : format_tag {};

/// Defines type for image width property.
struct cr2_image_width : property_base< int32_t > {};

/// Defines type for image height property.
struct cr2_image_height : property_base< int32_t > {};

/// Defines type for samples per pixel property.
struct cr2_samples_per_pixel : property_base< int32_t > {};

/// Defines type for bits per pixel property.
struct cr2_bits_per_pixel : property_base< int32_t > {};

/// Defines type for camera manufacturer.
struct cr2_camera_manufacturer : property_base< std::string > {};

/// Defines type for camera model.
struct cr2_camera_model : property_base< std::string > {};

/// Defines type for raw images count.
struct cr2_raw_images_count : property_base< unsigned > {};

/// Defines type for dng version.
struct cr2_dng_version : property_base< unsigned > {};

/// Defines type for number of colors.
struct cr2_number_colors : property_base< int32_t > {};

/// Defines type for colors description.
struct cr2_colors_description : property_base< std::string > {};

/// Defines type for raw height.
struct cr2_raw_height : property_base< uint16_t > {};

/// Defines type for raw width.
struct cr2_raw_width : property_base< uint16_t > {};

/// Defines type for visible height.
struct cr2_visible_height : property_base< uint16_t > {};

/// Defines type for visible width.
struct cr2_visible_width : property_base< uint16_t > {};

/// Defines type for top margin.
struct cr2_top_margin : property_base< uint16_t > {};

/// Defines type for left margin.
struct cr2_left_margin : property_base< uint16_t > {};

/// Defines type for output height.
struct cr2_output_height : property_base< uint16_t > {};

/// Defines type for output width.
struct cr2_output_width : property_base< uint16_t > {};

/// Defines type for pixel aspect.
struct cr2_pixel_aspect : property_base< double_t > {};

/// Defines type for image orientation.
struct cr2_flip : property_base< uint32_t > {};

/// Defines type for iso speed.
struct cr2_iso_speed : property_base< float > {};

/// Defines type for shutter.
struct cr2_shutter : property_base< float > {};

/// Defines type for aperture.
struct cr2_aperture : property_base< float > {};

/// Defines type for focal length.
struct cr2_focal_length : property_base< float > {};

/// Defines type for timestamp.
struct cr2_timestamp : property_base< time_t > {};

/// Defines type for shot order.
struct cr2_shot_order : property_base< uint16_t > {};

/// Defines type for image description.
struct cr2_image_description : property_base< std::string > {};

/// Defines type for artist.
struct cr2_artist : property_base< std::string > {};

/// Read information for cr2 images.
///
/// The structure is returned when using read_image_info.
template<>
struct image_read_info< cr2_tag >
{
    /// Default contructor.
    image_read_info< cr2_tag >()
    : _valid( false )
    {}

    // Here, width and height of the image are the ones of the output height (ie after having been processed by dcraw emulator)
    cr2_image_width::type       _width;
    cr2_image_height::type      _height;
    cr2_samples_per_pixel::type _samples_per_pixel;
    cr2_bits_per_pixel::type    _bits_per_pixel;

    cr2_camera_manufacturer::type _camera_manufacturer;
    cr2_camera_model::type        _camera_model;

    cr2_raw_images_count::type   _raw_images_count;
    cr2_dng_version::type        _dng_version;
    cr2_number_colors::type      _number_colors;
    cr2_colors_description::type _colors_description;

    cr2_raw_width::type      _raw_width;
    cr2_raw_height::type     _raw_height;
    cr2_visible_width::type  _visible_width;
    cr2_visible_height::type _visible_height;
    cr2_top_margin::type     _top_margin;
    cr2_left_margin::type    _left_margin;
    cr2_output_width::type   _output_width;
    cr2_output_height::type  _output_height;
    cr2_pixel_aspect::type   _pixel_aspect;
    cr2_flip::type           _flip;

    cr2_iso_speed::type         _iso_speed;
    cr2_shutter::type           _shutter;
    cr2_aperture::type          _aperture;
    cr2_focal_length::type      _focal_length;
    cr2_timestamp::type         _timestamp;
    cr2_shot_order::type        _shot_order;
    cr2_image_description::type _image_description;
    cr2_artist::type            _artist;

    /// Used internaly to identify if the header has been read.
    bool _valid;
};

/// Read settings for cr2 images.
///
/// The structure can be used for all read_xxx functions, except read_image_info.
template<>
struct image_read_settings< cr2_tag > : public image_read_settings_base
{
    /// Default constructor
    image_read_settings()
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

/// Write information for cr2 images.
///
/// The structure can be used for write_view() function.
template<>
struct image_write_info< cr2_tag >
{
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_CR2_TAGS_HPP
