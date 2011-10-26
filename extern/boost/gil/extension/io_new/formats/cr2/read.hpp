/*
    Copyright 2008 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_CR2_IO_READ_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_CR2_IO_READ_HPP_INCLUDED

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
#include <boost/gil/extension/io_new/cr2_tags.hpp>

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


template< typename Device
        , typename ConversionPolicy
        >
class reader< Device
            , cr2_tag
            , ConversionPolicy
            >
    : public reader_base< cr2_tag
                        , ConversionPolicy
                        >
{
private:

    typedef typename ConversionPolicy::color_converter_type cc_t;

public:

    reader( Device&                               device
          , const image_read_settings< cr2_tag >& settings
          )
    : reader_base< cr2_tag
                 , ConversionPolicy >( settings )
    , _io_dev( device )
    {}

    reader( Device&                               device
          , const cc_t&                           cc
          , const image_read_settings< cr2_tag >& settings
          )
    : _io_dev( device )
    , reader_base< cr2_tag
                 , ConversionPolicy
                 >( cc
                  , settings
                  )
    {}

    image_read_info< cr2_tag > get_info()
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
        std::cout << "data unpacked ..." << std::endl;

        return_code = _io_dev.dcraw_process();
        io_error_if( return_code != LIBRAW_SUCCESS, "Unable to emulate behavior dcraw to process image" );
        std::cout << "dcraw_processed ..." << std::endl;

        libraw_processed_image_t* processed_image = _io_dev.dcraw_make_mem_image(&return_code);
        io_error_if( return_code != LIBRAW_SUCCESS, "Unable to dcraw_make_mem_image" );
        std::cout << "dcraw_make_mem_imageed ..." << std::endl;

        if(processed_image->colors!=1 && processed_image->colors!=3)
            io_error( "Image is neither gray nor RGB" );

        if(processed_image->bits==8)
        {
            rgb8_view_t build = boost::gil::interleaved_view(processed_image->width,
                                                             processed_image->height,
                                                             (rgb8_pixel_t*)processed_image->data,
                                                             processed_image->colors*processed_image->width);

            this->_cc_policy.read( build.begin(), build.end(), dst_view.begin() );
        }
        else if(processed_image->bits==16)
        {
            rgb16_view_t build = boost::gil::interleaved_view(processed_image->width,
                                                              processed_image->height,
                                                              (rgb16_pixel_t*)processed_image->data,
                                                              2*processed_image->colors*processed_image->width);

            this->_cc_policy.read( build.begin(), build.end(), dst_view.begin() );
        }
        else
            io_error( "Image is neither 8bit nor 16bit" );
    }

protected:
    Device& _io_dev;
    image_read_info< cr2_tag > _info;
};

/////////////////////////////////// dynamic image


struct cr2_type_format_checker
{
    cr2_type_format_checker( const image_read_info< cr2_tag >& info )
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

    const image_read_info< cr2_tag >& _info;
};

struct cr2_read_is_supported
{
    template< typename View >
    struct apply : public is_read_supported< typename get_pixel_type< View >::type
                                           , cr2_tag
                                           >
    {};
};

template< typename Device
        >
class dynamic_image_reader< Device
                          , cr2_tag
                          >
    : public reader< Device
                   , cr2_tag
                   , detail::read_and_no_convert
                   >
{
    typedef reader< Device
                  , cr2_tag
                  , detail::read_and_no_convert
                  > parent_t;

public:

    dynamic_image_reader( Device&                               device
                        , const image_read_settings< cr2_tag >& settings
                        )
    : parent_t( device
              , settings
              )
    {}

    template< typename Images >
    void apply( any_image< Images >& images )
    {
        cr2_type_format_checker format_checker( this->_info );

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
            init_image( images
                       , this->_info
                       );

            dynamic_io_fnobj< cr2_read_is_supported
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

#endif // BOOST_GIL_EXTENSION_IO_CR2_IO_READ_HPP_INCLUDED
