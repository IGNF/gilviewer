/*
    Copyright 2008 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_PNM_IO_READ_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_PNM_IO_READ_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning \n
///
/// \date 2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <boost/bind.hpp>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/pnm_tags.hpp>

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/conversion_policies.hpp>
#include <boost/gil/extension/io_new/detail/row_buffer_helper.hpp>
#include <boost/gil/extension/io_new/detail/bit_operations.hpp>
#include <boost/gil/extension/io_new/detail/reader_base.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>
#include <boost/gil/extension/io_new/detail/typedefs.hpp>

#include "is_allowed.hpp"

namespace boost { namespace gil { namespace detail {

template< typename View, typename T >
struct calc_pitch {};

template< typename View >
struct calc_pitch< View, mpl::false_ >
{
    static uint32_t do_it( uint32_t width ) { return width * num_channels< View >::value; }
};

template< typename View >
struct calc_pitch< View, mpl::true_ >
{
    static uint32_t do_it( uint32_t width ) {  return ( width + 7 ) >> 3; }
};


template< typename Device
        , typename ConversionPolicy
        >
class reader< Device
            , pnm_tag
            , ConversionPolicy
            >
    : public reader_base< pnm_tag
                        , ConversionPolicy
                        >
{
private:

    typedef typename ConversionPolicy::color_converter_type cc_t;

public:
    reader( Device&                                device
          , const image_read_settings< pnm_tag >& settings
          )
    : reader_base< pnm_tag
                 , ConversionPolicy >( settings )
    , _io_dev( device )
    {}

    reader( Device&                               device
          , const cc_t&                           cc
          , const image_read_settings< pnm_tag >& settings
          )
    : reader_base< pnm_tag
                 , ConversionPolicy >( cc
                                     , settings
                                     )
    , _io_dev( device )
    {}

    image_read_info<pnm_tag> get_info()
    {
        image_read_info<pnm_tag> ret;

        // read signature
        io_error_if( read_char() != 'P', "Invalid PNM signature" );

        ret._type = read_char() - '0';

		io_error_if( ret._type < pnm_image_type::mono_asc_t::value || ret._type > pnm_image_type::color_bin_t::value
		           , "Invalid PNM file (supports P1 to P6)"
		           );

        ret._width  = read_int();
        ret._height = read_int();

        if( ret._type == pnm_image_type::mono_asc_t::value || ret._type == pnm_image_type::mono_bin_t::value )
        {
            ret._max_value = 1;
        }
        else
        {
            ret._max_value = read_int();

		    io_error_if( ret._max_value > 255
		               , "Unsupported PNM format (supports maximum value 255)"
		               );
        }

        return ret;
    }

    template<typename View>
    void apply( const View& view )
    {

        typedef typename is_same< ConversionPolicy
                                , read_and_no_convert
                                >::type is_read_and_convert_t;

        io_error_if( !is_allowed< View >( this->_info
                                        , is_read_and_convert_t()
                                        )
                   , "Image types aren't compatible."
                   );

        switch( this->_info._type )
		{
            // reading mono text is reading grayscale but with only two values
			case pnm_image_type::mono_asc_t::value:  { read_text_data< gray8_view_t >( view ); break; }
			case pnm_image_type::gray_asc_t::value:  { read_text_data< gray8_view_t >( view ); break; }
			case pnm_image_type::color_asc_t::value: { read_text_data< rgb8_view_t  >( view ); break; }

			case pnm_image_type::mono_bin_t::value:  { read_bin_data< gray1_image_t::view_t >( view ); break; }
			case pnm_image_type::gray_bin_t::value:  { read_bin_data< gray8_view_t          >( view ); break; }
			case pnm_image_type::color_bin_t::value: { read_bin_data< rgb8_view_t           >( view ); break; }
		}
    }

private:

    template< typename View_Src
            , typename View_Dst
            >
    void read_text_data( const View_Dst& dst )
    {
        typedef typename View_Dst::y_coord_t y_t;

        uint32_t pitch = this->_info._width * num_channels< View_Src >::value;

        byte_vector_t row( pitch );

        //Skip scanlines if necessary.
        for( int y = 0; y <  this->_settings._top_left.y; ++y )
        {
            read_text_row< View_Src >( dst, row, pitch, y, false );
        }

        for( y_t y = 0; y < dst.height(); ++y )
        {
            read_text_row< View_Src >( dst, row, pitch, y, true );
        }
    }

    template< typename View_Src
            , typename View_Dst
            >
    void read_text_row( const View_Dst&              dst
                      , byte_vector_t&               row
                      , uint32_t                     pitch
                      , typename View_Dst::y_coord_t y
                      , bool                         process
                      )
    {
        static char buf[16];

        View_Src src = interleaved_view( this->_info._width
                                       , 1
                                       , (typename View_Src::value_type*) &row.front()
                                       , pitch
                                       );

        for( uint32_t x = 0; x < pitch; ++x )
        {
            for( uint32_t k = 0; ; )
            {
				int ch = _io_dev.getc_unchecked();

				if( isdigit( ch ))
				{
                    buf[ k++ ] = static_cast< char >( ch );
				}
				else if( k )
				{
					buf[ k ] = 0;
					break;
				}
				else if( ch == EOF || !isspace( ch ))
				{
					return;
				}
            }

            if( process )
            {
                int value = atoi( buf );

                if( this->_info._max_value == 1 )
                {
                    typedef typename channel_type< typename get_pixel_type< View_Dst >::type >::type channel_t;

                    // for pnm format 0 is white
                    row[x] = ( value != 0 )
                             ? typename channel_traits< channel_t >::value_type( 0 )
                             : channel_traits< channel_t >::max_value();
                }
                else
                {
                    row[x] = static_cast< byte_t >( value );
                }
            }
        }

        if( process )
        {
            // We are reading a gray1_image like a gray8_image but the two pixel_t
            // aren't compatible. Though, read_and_no_convert::read(...) wont work.
            copy_data< View_Dst
                     , View_Src >( dst
                                 , src
                                 , y
                                 , typename is_same< View_Dst
                                                   , gray1_image_t::view_t
                                                   >::type()
                                 );
        }
    }

    template< typename View_Dst
            , typename View_Src
            >
    void copy_data( const View_Dst&              dst
                  , const View_Src&              src
                  , typename View_Dst::y_coord_t y
                  , mpl::true_ // is gray1_view
                  )
    {
        if(  this->_info._max_value == 1 )
        {
            typename View_Dst::x_iterator it = dst.row_begin( y );

            for( typename View_Dst::x_coord_t x = 0
               ; x < dst.width()
               ; ++x
               )
            {
                it[x] = src[x];
            }
        }
        else
        {
            copy_data( dst
                     , src
                     , y
                     , mpl::false_()
                     );
        }
    }

    template< typename View_Dst
            , typename View_Src
            >
    void copy_data( const View_Dst&              view
                  , const View_Src&              src
                  , typename View_Dst::y_coord_t y
                  , mpl::false_ // is gray1_view
                  )
    {
        typename View_Src::x_iterator beg = src.row_begin( 0 ) + this->_settings._top_left.x;
        typename View_Src::x_iterator end = beg + this->_settings._dim.x;

        this->_cc_policy.read( beg
                             , end
                             , view.row_begin( y )
                             );
    }


    template< typename View_Src
            , typename View_Dst
            >
    void read_bin_data( const View_Dst& view )
    {
        typedef typename View_Dst::y_coord_t y_t;
        typedef typename is_bit_aligned<
                    typename View_Src::value_type >::type is_bit_aligned_t;

        uint32_t pitch = calc_pitch< View_Src, is_bit_aligned_t >::do_it( this->_info._width );

        typedef row_buffer_helper_view< View_Src > rh_t;
        rh_t rh( pitch, true );

        typename rh_t::iterator_t beg = rh.begin() + this->_settings._top_left.x;
        typename rh_t::iterator_t end = beg + this->_settings._dim.x;

        // For bit_aligned images we need to negate all bytes in the row_buffer
        // to make sure that 0 is black and 255 is white.
        negate_bits< typename rh_t::buffer_t
                   , is_bit_aligned_t
                   > neg;

        swap_half_bytes< typename rh_t::buffer_t
                       , is_bit_aligned_t
                       > swhb;

        //Skip scanlines if necessary.
        for( y_t y = 0; y < this->_settings._top_left.y; ++y )
        {
            _io_dev.read( reinterpret_cast< byte_t* >( rh.data() )
                        , pitch
                        );
        }

        for( y_t y = 0; y < view.height(); ++y )
        {
            _io_dev.read( reinterpret_cast< byte_t* >( rh.data() )
                        , pitch
                        );

            neg( rh.buffer() );
            swhb( rh.buffer() );

            this->_cc_policy.read( beg
                                 , end
                                 , view.row_begin( y )
                                 );
        }
    }

private:

    // Read a character and skip a comment if necessary.
    char read_char()
    {
        char ch;

        if(( ch = _io_dev.getc() ) == '#' )
        {
            // skip comment to EOL
            do
            {
                ch = _io_dev.getc();
            }
            while (ch != '\n' && ch != '\r');
        }

        return ch;
    }

	unsigned int read_int()
	{
		char ch;

        // skip whitespaces, tabs, and new lines
		do
		{
			ch = read_char();
		}
		while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r');

		if( ch < '0' || ch > '9' )
		{
			io_error( "Unexpected characters reading decimal digits" );
		}

		unsigned val = 0;

        do
        {
			unsigned dig = ch - '0';

			if( val > INT_MAX / 10 - dig )
			{
				io_error( "Integer too large" );
			}

			val = val * 10 + dig;

			ch = read_char();
		}
		while( '0' <= ch && ch <= '9' );

		return val;
	}

private:

    Device& _io_dev;
};

struct pnm_type_format_checker
{
    pnm_type_format_checker( pnm_image_type::type type )
    : _type( type )
    {}

    template< typename Image >
    bool apply()
    {
        typedef is_read_supported< typename get_pixel_type< typename Image::view_t >::type
                                 , pnm_tag
                                 > is_supported_t;

        return is_supported_t::_asc_type == _type
            || is_supported_t::_bin_type == _type;
    }

private:

    pnm_image_type::type _type;
};

struct pnm_read_is_supported
{
    template< typename View >
    struct apply : public is_read_supported< typename get_pixel_type< View >::type
                                           , pnm_tag
                                           >
    {};
};

template< typename Device
        >
class dynamic_image_reader< Device
                          , pnm_tag
                          >
    : public reader< Device
                   , pnm_tag
                   , detail::read_and_no_convert
                   >
{
    typedef reader< Device
                  , pnm_tag
                  , detail::read_and_no_convert
                  > parent_t;

public:

    dynamic_image_reader( Device&                               device
                        , const image_read_settings< pnm_tag >& settings
                        )
    : parent_t( device
              , settings
              )
    {}

    template< typename Images >
    void apply( any_image< Images >& images )
    {
        pnm_type_format_checker format_checker( this->_info._type );

        if( !construct_matched( images
                              , format_checker
                              ))
        {
            io_error( "No matching image type between those of the given any_image and that of the file" );
        }
        else
        {
            init_image( images
                      , this->_info
                      );

            dynamic_io_fnobj< pnm_read_is_supported
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

#endif // BOOST_GIL_EXTENSION_IO_PNM_IO_READ_HPP_INCLUDED
