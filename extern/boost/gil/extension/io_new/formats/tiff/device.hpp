/*
    Copyright 2007-2008 Andreas Pokorny, Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_DETAIL_TIFF_IO_DEVICE_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_DETAIL_TIFF_IO_DEVICE_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Andreas Pokorny, Christian Henning \n
///
/// \date   2007-2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

extern "C" {
#include "tiff.h"
#include "tiffio.h"
}

#include "tiffio.hxx"

#include <boost/shared_ptr.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>

namespace boost { namespace gil { namespace detail {

class tiff_device_base
{
public:

    tiff_device_base()
    {}

    tiff_device_base( TIFF* tiff_file )
    : _tiff_file( tiff_file
                , TIFFClose )
    {}


    template <typename Property>
    bool get_property( typename Property::type& value  )
    {
        if( TIFFGetFieldDefaulted( _tiff_file.get()
                                 , Property::tag
                                 , &value ) == 1 )
        {
            return true;
        }

        return false;
    }
   

    template <typename Property>
    inline
    bool set_property( const typename Property::type& value )
    {
       if( TIFFSetField( _tiff_file.get()
                       , Property::tag
                       , value  ) == 1 )
       {
          return true;
       }

       return false;
    }

    bool are_bytes_swapped()
    {
      return ( TIFFIsByteSwapped( _tiff_file.get() )) ? true : false;
    }

    bool is_tiled() const
    {
        return ( TIFFIsTiled( _tiff_file.get() )) ? true : false;
    }

    unsigned int get_default_strip_size()
    {
        return TIFFDefaultStripSize( _tiff_file.get()
                                   , 0 );
    }

    std::size_t get_scanline_size()
    {
      return TIFFScanlineSize( _tiff_file.get() );
    }

    std::size_t get_tile_size()
    {
      return TIFFTileSize( _tiff_file.get() );
    }


    int get_field_defaulted( uint16_t*& red
                           , uint16_t*& green
                           , uint16_t*& blue
                           )
    {
        return TIFFGetFieldDefaulted( _tiff_file.get()
                                    , TIFFTAG_COLORMAP
                                    , &red
                                    , &green
                                    , &blue
                                    );
    }

    template< typename Buffer >
    void read_scanline( Buffer&        buffer
                      , std::ptrdiff_t row
                      , tsample_t      plane
                      )
    {
        io_error_if( TIFFReadScanline( _tiff_file.get()
                                     , reinterpret_cast< tdata_t >( &buffer.front() )
                                     , (uint32) row
                                     , plane           ) == -1
                   , "Read error."
                   );
    }

    template< typename Buffer >
    void read_tile( Buffer&        buffer
                  , std::ptrdiff_t x
                  , std::ptrdiff_t y
                  , std::ptrdiff_t z
                  , tsample_t      plane
                  )
    {
        io_error_if( TIFFReadTile( _tiff_file.get()
                                 , reinterpret_cast< tdata_t >( &buffer.front() )
                                 , (uint32) x
                                 , (uint32) y
                                 , (uint32) z
                                 , plane           ) == -1
                   , "Read tile error."
                   );
    }

    template< typename Buffer >
    inline 
    void write_scaline( Buffer&     buffer
                      , uint32      row
                      , tsample_t   plane
                      )
    {
       io_error_if( TIFFWriteScanline( _tiff_file.get()
                                     , &buffer.front()
                                     , row
                                     , plane 
                                     ) == -1
                   , "Write error"
                   );
    }

protected:

   typedef shared_ptr<TIFF> tiff_file_t;
   tiff_file_t _tiff_file;

};

/*!
 *
 * file_stream_device specialization for tiff images, which are based on TIFF*.
 */
template<>
class file_stream_device< tiff_tag > : public tiff_device_base
{
public:

    struct read_tag {};
    struct write_tag {};

    file_stream_device( std::string const& file_name, read_tag )
    {
        TIFF* tiff;

        io_error_if( ( tiff = TIFFOpen( file_name.c_str(), "r" )) == NULL
                   , "file_stream_device: failed to open file" );

        _tiff_file = tiff_file_t( tiff, TIFFClose );
    }

    file_stream_device( std::string const& file_name, write_tag )
    {
        TIFF* tiff;

        io_error_if( ( tiff = TIFFOpen( file_name.c_str(), "w" )) == NULL
                   , "file_stream_device: failed to open file" );

        _tiff_file = tiff_file_t( tiff, TIFFClose );
    }

    file_stream_device( TIFF* tiff_file )
    : tiff_device_base( tiff_file )
    {}
};

/*!
 *
 * ostream_device specialization for tiff images.
 */
template<>
class ostream_device< tiff_tag > : public tiff_device_base
{
public:
    ostream_device( std::ostream & out )
    : _out( out )
    {
        TIFF* tiff;

        io_error_if( ( tiff = TIFFStreamOpen( ""
                                            , &_out
                                            )
                      ) == NULL
                   , "ostream_device: failed to stream"
                   );

        _tiff_file = tiff_file_t( tiff, TIFFClose );
    }

private:

    std::ostream& _out;
};

/*!
 *
 * ostream_device specialization for tiff images.
 */
template<>
class istream_device< tiff_tag > : public tiff_device_base
{
public:
    istream_device( std::istream & in )
    : _in( in )
    {
        TIFF* tiff;

        io_error_if( ( tiff = TIFFStreamOpen( ""
                                            , &_in
                                            )
                     ) == NULL
                   , "istream_device: failed to stream"
                   );

        _tiff_file = tiff_file_t( tiff, TIFFClose );
    }

private:

    std::istream& _in;
};

/*
template< typename T, typename D >
struct is_adaptable_input_device< tiff_tag, T, D > : mpl::false_{};
*/

template< typename FormatTag >
struct is_adaptable_input_device< FormatTag
                                , TIFF*
                                , void
                                >
    : mpl::true_
{
    typedef file_stream_device< FormatTag > device_type;
};

template< typename FormatTag >
struct is_adaptable_output_device< FormatTag
                                 , TIFF*
                                 , void
                                 >
    : mpl::true_
{
    typedef file_stream_device< FormatTag > device_type;
};

} // namespace detail
} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_DETAIL_TIFF_IO_DEVICE_HPP_INCLUDED
