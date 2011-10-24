/*
    Copyright 2008 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_CR2_SUPPORTED_TYPES_HPP
#define BOOST_GIL_EXTENSION_IO_CR2_SUPPORTED_TYPES_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief 
/// \author Christian Henning \n
///
/// \date   2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/gil/channel.hpp>
#include <boost/gil/color_base.hpp>

#include <boost/gil/extension/io_new/detail/base.hpp>

namespace boost { namespace gil { namespace detail {

// Read support

// TIFF virtually supports everything
struct cr2_read_support : read_support_true
{};


// Write support

struct cr2_write_support : write_support_true
{};

} // namespace detail

template< typename Pixel >
struct is_read_supported< Pixel,
                        cr2_tag
                        >
    : mpl::bool_< detail::cr2_read_support::is_supported > {};

template< typename Pixel >
struct is_write_supported< Pixel
                         , cr2_tag
                         >
    : mpl::bool_< detail::cr2_write_support::is_supported >
{};

} // namespace gil
} // namespace boost


#endif // BOOST_GIL_EXTENSION_IO_CR2_SUPPORTED_TYPES_HPP
