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

/// Defines type for bits per pixels property.
struct cr2_bits_per_pixel : property_base< uint16_t > {};

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

    /// The bitmap width in pixels ( signed integer ).
    cr2_image_width::type  _width;

    /// The bitmap height in pixels ( signed integer ).
    cr2_image_height::type _height;

    cr2_bits_per_pixel::type _bits_per_pixel;

    /// Used internaly to identify is the header has been read.
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
