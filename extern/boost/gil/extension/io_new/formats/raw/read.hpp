/*
    Copyright 2008 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_RAW_IO_READ_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_RAW_IO_READ_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Olivier Tournaire \n
///
/// \date 2011 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <vector>
#include <boost/gil/extension/io_new/raw_tags.hpp>

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/bit_operations.hpp>
#include <boost/gil/extension/io_new/detail/conversion_policies.hpp>
#include <boost/gil/extension/io_new/detail/row_buffer_helper.hpp>
#include <boost/gil/extension/io_new/detail/reader_base.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>
#include <boost/gil/extension/io_new/detail/typedefs.hpp>

#include "is_allowed.hpp"
#include "device.hpp"

// TODO: remove
#include <iostream>

namespace boost { namespace gil { namespace detail {

#define BUILD_INTERLEAVED_VIEW(color_layout, bits_per_pixel) \
{ \
    color_layout##bits_per_pixel##_view_t build = boost::gil::interleaved_view(processed_image->width, \
                                                                             processed_image->height, \
                                                                             (color_layout##bits_per_pixel##_pixel_t*)processed_image->data, \
                                                                             processed_image->colors*processed_image->width*processed_image->bits/8); \
    this->_cc_policy.read( build.begin(), build.end(), dst_view.begin() ); \
} \


template< typename Device
        , typename ConversionPolicy
        >
class reader< Device
            , raw_tag
            , ConversionPolicy
            >
    : public reader_base< raw_tag
                        , ConversionPolicy
                        >
{
private:

    typedef typename ConversionPolicy::color_converter_type cc_t;

public:

    reader( Device&                               device
          , const image_read_settings< raw_tag >& settings
          )
    : reader_base< raw_tag
                 , ConversionPolicy >( settings )
    , _io_dev( device )
    {}

    reader( Device&                               device
          , const cc_t&                           cc
          , const image_read_settings< raw_tag >& settings
          )
    : _io_dev( device )
    , reader_base< raw_tag
                 , ConversionPolicy
                 >( cc
                  , settings
                  )
    {}

    image_read_info< raw_tag > get_info()
    {
        _io_dev.get_mem_image_format(&_info._width
                                    ,&_info._height
                                    ,&_info._samples_per_pixel
                                    ,&_info._bits_per_pixel);

        // iparams
        _info._camera_manufacturer = _io_dev.get_camera_manufacturer();
        _info._camera_model        = _io_dev.get_camera_model();
        _info._raw_images_count    = _io_dev.get_raw_count();
        _info._dng_version         = _io_dev.get_dng_version();
        _info._number_colors       = _io_dev.get_colors();
        //_io_dev.get_filters();
        _info._colors_description  = _io_dev.get_cdesc();

        // image_sizes
        _info._raw_width      = _io_dev.get_raw_width();
        _info._raw_height     = _io_dev.get_raw_height();
        _info._visible_width  = _io_dev.get_image_width();
        _info._visible_height = _io_dev.get_image_height();
        _info._top_margin     = _io_dev.get_top_margin();
        _info._left_margin    = _io_dev.get_left_margin();
        _info._output_width   = _io_dev.get_iwidth();
        _info._output_height  = _io_dev.get_iheight();
        _info._pixel_aspect   = _io_dev.get_pixel_aspect();
        _info._flip           = _io_dev.get_flip();

        // imgother
        _info._iso_speed         = _io_dev.get_iso_speed();
        _info._shutter           = _io_dev.get_shutter();
        _info._aperture          = _io_dev.get_aperture();
        _info._focal_length      = _io_dev.get_focal_len();
        _info._timestamp         = _io_dev.get_timestamp();
        _info._shot_order        = _io_dev.get_shot_order();
        //_io_dev.get_gpsdata();
        _info._image_description = _io_dev.get_desc();
        _info._artist            = _io_dev.get_artist();

        _info._libraw_version = _io_dev.get_version();

        _info._valid = true;

        return _info;
    }

    template< typename View >
    void apply( const View& dst_view )
    {
        if( !_info._valid )
        {
            get_info();
        }

        typedef typename is_same< ConversionPolicy
                                , read_and_no_convert
                                >::type is_read_and_convert_t;

        io_error_if( !is_allowed< View >( this->_info
                                        , is_read_and_convert_t()
                                        )
                   , "Image types aren't compatible." );

        // TODO: better error handling based on return code
        int return_code = _io_dev.unpack();
        io_error_if( return_code != LIBRAW_SUCCESS, "Unable to unpack image" );
        _info._unpack_function_name = _io_dev.get_unpack_function_name();

        return_code = _io_dev.dcraw_process();
        io_error_if( return_code != LIBRAW_SUCCESS, "Unable to emulate dcraw behavior to process image" );

        libraw_processed_image_t* processed_image = _io_dev.dcraw_make_mem_image(&return_code);
        io_error_if( return_code != LIBRAW_SUCCESS, "Unable to dcraw_make_mem_image" );

        if(processed_image->colors!=1 && processed_image->colors!=3)
            io_error( "Image is neither gray nor RGB" );

        if(processed_image->bits!=8 && processed_image->bits!=16)
            io_error( "Image is neither 8bit nor 16bit" );

        // TODO
        // Here, we should use a metafunction to reduce code size and avoid a (compile time) macro
        if(processed_image->bits==8)
        {
            if(processed_image->colors==1){ BUILD_INTERLEAVED_VIEW(gray, 8); }
            else                          { BUILD_INTERLEAVED_VIEW(rgb,  8); }
        }
        else if(processed_image->bits==16)
        {
            if(processed_image->colors==1){ BUILD_INTERLEAVED_VIEW(gray, 16); }
            else                          { BUILD_INTERLEAVED_VIEW(rgb,  16); }
        }
    }

protected:
    Device& _io_dev;
    image_read_info< raw_tag > _info;
};

/////////////////////////////////// dynamic image


struct raw_type_format_checker
{
    raw_type_format_checker( const image_read_info< raw_tag >& info )
    : _info( info )
    {}

    template< typename Image >
    bool apply()
    {
        typedef typename Image::view_t view_t;

        return is_allowed< view_t >( _info
                                   , mpl::true_()
                                   );
    }

private:

    const image_read_info< raw_tag >& _info;
};

struct raw_read_is_supported
{
    template< typename View >
    struct apply : public is_read_supported< typename get_pixel_type< View >::type
                                           , raw_tag
                                           >
    {};
};

template< typename Device
        >
class dynamic_image_reader< Device
                          , raw_tag
                          >
    : public reader< Device
                   , raw_tag
                   , detail::read_and_no_convert
                   >
{
    typedef reader< Device
                  , raw_tag
                  , detail::read_and_no_convert
                  > parent_t;

public:

    dynamic_image_reader( Device&                               device
                        , const image_read_settings< raw_tag >& settings
                        )
    : parent_t( device
              , settings
              )
    {}

    template< typename Images >
    void apply( any_image< Images >& images )
    {
        raw_type_format_checker format_checker( this->_info );

        if( !construct_matched( images
                               , format_checker
                               ))
        {
            std::ostringstream error_message;
            error_message << "No matching image type between those of the given any_image and that of the file.\n";
            error_message << "Image type must be {gray||rgb}{8||16} unsigned for RAW image files.";
            io_error( error_message.str() );
        }
        else
        {
            if( !this->_info._valid )
            {
                this->get_info();
            }
            this->init_image( images
                       , this->_info
                       );

            dynamic_io_fnobj< raw_read_is_supported
                    , parent_t
                    > op( this );

            apply_operation( view( images )
                            , op
                            );
        }
    }
};

} // detail
} // gil
} // boost

#endif // BOOST_GIL_EXTENSION_IO_RAW_IO_READ_HPP_INCLUDED
