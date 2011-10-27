/*
    Copyright 2008 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_RAW_IO_WRITE_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_RAW_IO_WRITE_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Olivier Tournaire \n
///
/// \date 2011 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <vector>

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>

#include <boost/gil/extension/io_new/raw_tags.hpp>

#include "device.hpp"

namespace boost { namespace gil { namespace detail {

template< typename Device >
class writer< Device
            , raw_tag
            >
{
public:
    writer( Device& file ) : _out( file ) {}
    ~writer() {}

    template<typename View>
    void apply( const View& view )
    {
        io_error( "raw format cannot be written!" );
    }

    template<typename View>
    void apply( const View&                           view
              , const image_write_info< raw_tag >& /* info */
              )
    {
        io_error( "raw format cannot be written!" );
    }

private:
    Device& _out;
};

template< typename Device >
class dynamic_image_writer< Device
                          , raw_tag
                          >
    : public writer< Device
                   , raw_tag
                   >
{
    typedef writer< Device
                  , raw_tag
                  > parent_t;

public:

    dynamic_image_writer( Device& file ) : parent_t( file ) {}

    template< typename Views >
    void apply( const any_image_view< Views >& views )
    {
        io_error( "raw format cannot be written!" );
    }
};

} // detail
} // gil
} // boost

#endif // BOOST_GIL_EXTENSION_IO_RAW_IO_WRITE_HPP_INCLUDED
