/*
    Copyright 2007-2008 Andreas Pokorny, Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_DETAIL_CONVERSION_POLICIES_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_DETAIL_CONVERSION_POLICIES_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Andreas Pokorny, Christian Henning \n
///
/// \date   2007-2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <iterator>
#include <boost/gil/image_view_factory.hpp>

namespace boost{namespace gil{ namespace detail {

struct read_and_no_convert 
{
public:
    typedef void* color_converter_type;

    template< typename InIterator
            , typename OutIterator
            >
    void read( const InIterator& /* begin */
             , const InIterator& /* end   */
             , OutIterator       /* out   */
             , typename disable_if< typename pixels_are_compatible< typename std::iterator_traits<InIterator>::value_type
                                                                  , typename std::iterator_traits<OutIterator>::value_type
                                                                  >::type 
                                  >::type* /* ptr */ = 0
             )
    {
        io_error( "Data cannot be copied because the pixels are incompatible." );
    }

    template< typename InIterator
            , typename OutIterator
            >
    void read( const InIterator& begin
             , const InIterator& end
             , OutIterator       out
             , typename enable_if< typename pixels_are_compatible< typename std::iterator_traits<InIterator>::value_type
                                                                 , typename std::iterator_traits<OutIterator>::value_type
                                                                 >::type 
                                 >::type* /* ptr */ = 0
             )
    {
        std::copy( begin
                 , end
                 , out
                 );
    }

};

template<typename CC>
struct read_and_convert
{
public:
    typedef CC color_converter_type;
    CC _cc;

    read_and_convert( color_converter_type const& cc )
        : _cc(cc) {}

    template< typename InIterator
            , typename OutIterator
            >
    void read( const InIterator& begin
             , const InIterator& end
             , OutIterator       out
             )
    {
        typedef color_convert_deref_fn< typename std::iterator_traits<InIterator>::reference
                                      , typename std::iterator_traits<OutIterator>::value_type //reference?
                                      , CC
                                      > deref_t;

        std::transform( begin
                      , end
                      , out
                      , deref_t( _cc )
                      );
    }
};

} // namespace detail
} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_DETAIL_CONVERSION_POLICIES_HPP_INCLUDED
