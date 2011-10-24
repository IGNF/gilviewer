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
/// \author Christian Henning \n
///
/// \date 2008 \n
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
        // iparams
        std::cout << _io_dev.get_camera_manufacturer() << std::endl;
        std::cout << _io_dev.get_camera_model() << std::endl;
        std::cout << _io_dev.get_raw_count() << std::endl;
        std::cout << _io_dev.get_dng_version() << std::endl;
        std::cout << _io_dev.get_colors() << std::endl;
        std::cout << _io_dev.get_filters() << std::endl;
        std::cout << _io_dev.get_cdesc() << std::endl;
        std::cout << std::endl;

        // image_sizes
        std::cout << _io_dev.get_raw_width() << std::endl;
        std::cout << _io_dev.get_raw_height() << std::endl;
        std::cout << _io_dev.get_image_width() << std::endl;
        std::cout << _io_dev.get_image_height() << std::endl;
        std::cout << _io_dev.get_top_margin() << std::endl;
        std::cout << _io_dev.get_left_margin() << std::endl;
        std::cout << _io_dev.get_iwidth() << std::endl;
        std::cout << _io_dev.get_iheight() << std::endl;
        std::cout << _io_dev.get_pixel_aspect() << std::endl;
        std::cout << _io_dev.get_flip() << std::endl;
        std::cout << std::endl;

        // imgother
        std::cout << _io_dev.get_iso_speed() << std::endl;
        std::cout << _io_dev.get_shutter() << std::endl;
        std::cout << _io_dev.get_aperture() << std::endl;
        std::cout << _io_dev.get_focal_len() << std::endl;
        std::cout << _io_dev.get_timestamp() << std::endl;
        std::cout << _io_dev.get_shot_order() << std::endl;
        std::cout << _io_dev.get_gpsdata() << std::endl;
        std::cout << _io_dev.get_desc() << std::endl;
        std::cout << _io_dev.get_artist() << std::endl;

        _info._valid = true;

        return _info;
    }

    template< typename View >
    void apply( const View& dst_view )
    {
        // TODO
        if( !_info._valid )
        {
            get_info();
        }
    }

protected:
    Device& _io_dev;
    image_read_info< cr2_tag > _info;
};

/////////////////////////////////// dynamic image

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
    }
};

} // detail
} // gil
} // boost

#endif // BOOST_GIL_EXTENSION_IO_CR2_IO_READ_HPP_INCLUDED
