/*
    Copyright 2008 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_CR2_IO_WRITE_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_CR2_IO_WRITE_HPP_INCLUDED

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

#include <boost/gil/extension/io_new/cr2_tags.hpp>

#include "device.hpp"

namespace boost { namespace gil { namespace detail {

template< typename Device >
class writer< Device
            , cr2_tag
            >
{
public:

    writer( Device& file )
    : _out( file )
    {
    }

    ~writer()
    {
    }

    template<typename View>
    void apply( const View& view )
    {
        write( view );
    }

    template<typename View>
    void apply( const View&                           view
              , const image_write_info< cr2_tag >& /* info */
              )
    {
        // Add code here, once image_write_info< cr2_tag > isn't empty anymore.

        write( view );
    }

private:

    template< typename View >
    void write( const View& view )
    {
    }


    template< typename View
            , typename cr2_View
            >
    void write_image( const View&       view
                    , const std::size_t spn
                    )
    {
    }

private:

    Device& _out;
};


struct cr2_write_is_supported
{
    template< typename View >
    struct apply
        : public is_write_supported< typename get_pixel_type< View >::type
                                   , cr2_tag
                                   >
    {};
};

template< typename Device >
class dynamic_image_writer< Device
                          , cr2_tag
                          >
    : public writer< Device
                   , cr2_tag
                   >
{
    typedef writer< Device
                  , cr2_tag
                  > parent_t;

public:

    dynamic_image_writer( Device& file )
    : parent_t( file )
    {}

    template< typename Views >
    void apply( const any_image_view< Views >& views )
    {
        dynamic_io_fnobj< cr2_write_is_supported
                        , parent_t
                        > op( this );

        apply_operation( views, op );
    }
};

} // detail
} // gil
} // boost

#endif // BOOST_GIL_EXTENSION_IO_CR2_IO_WRITE_HPP_INCLUDED
