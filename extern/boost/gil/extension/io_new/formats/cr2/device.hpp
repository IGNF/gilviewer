/*
    Copyright 2007-2008 Andreas Pokorny, Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_DETAIL_CR2_IO_DEVICE_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_DETAIL_CR2_IO_DEVICE_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Olivier Tournaire \n
///
/// \date   2011 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <libraw/libraw.h>

#include <boost/shared_ptr.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>

namespace boost { namespace gil { namespace detail {

class cr2_device_base
{
public:

    cr2_device_base() {}
    cr2_device_base( const LibRaw& processor ) : m_libraw_processor(processor) {}

    // iparams getters
    std::string get_camera_manufacturer() { return std::string(m_libraw_processor.imgdata.idata.make);  }
    std::string get_camera_model()        { return std::string(m_libraw_processor.imgdata.idata.model); }
    unsigned get_raw_count()              { return m_libraw_processor.imgdata.idata.raw_count; }
    unsigned get_dng_version()            { return m_libraw_processor.imgdata.idata.dng_version; }
    int get_colors()                      { return m_libraw_processor.imgdata.idata.colors; }
    unsigned get_filters()                { return m_libraw_processor.imgdata.idata.filters; }
    std::string get_cdesc()               { return std::string(m_libraw_processor.imgdata.idata.cdesc); }

    // image_sizes getters
    unsigned short get_raw_width()    { return m_libraw_processor.imgdata.sizes.raw_width;  }
    unsigned short get_raw_height()   { return m_libraw_processor.imgdata.sizes.raw_height; }
    unsigned short get_image_width()  { return m_libraw_processor.imgdata.sizes.width;  }
    unsigned short get_image_height() { return m_libraw_processor.imgdata.sizes.height; }
    unsigned short get_top_margin()   { return m_libraw_processor.imgdata.sizes.top_margin;  }
    unsigned short get_left_margin()  { return m_libraw_processor.imgdata.sizes.left_margin; }
    unsigned short get_iwidth()       { return m_libraw_processor.imgdata.sizes.iwidth;  }
    unsigned short get_iheight()      { return m_libraw_processor.imgdata.sizes.iheight; }
    double get_pixel_aspect()         { return m_libraw_processor.imgdata.sizes.pixel_aspect;  }
    int get_flip()                    { return m_libraw_processor.imgdata.sizes.flip; }

    // colordata getters
    // TODO

    // imgother getters
    float get_iso_speed()     { return m_libraw_processor.imgdata.other.iso_speed; }
    float get_shutter()       { return m_libraw_processor.imgdata.other.shutter; }
    float get_aperture()      { return m_libraw_processor.imgdata.other.aperture; }
    float get_focal_len()     { return m_libraw_processor.imgdata.other.focal_len; }
    time_t get_timestamp()    { return m_libraw_processor.imgdata.other.timestamp; }
    unsigned get_shot_order() { return m_libraw_processor.imgdata.other.shot_order; }
    unsigned* get_gpsdata()   { return m_libraw_processor.imgdata.other.gpsdata; }
    std::string get_desc()    { return std::string(m_libraw_processor.imgdata.other.desc); }
    std::string get_artist()  { return std::string(m_libraw_processor.imgdata.other.artist); }

    void get_mem_image_format(int *widthp, int *heightp, int *colorsp, int *bpp) { m_libraw_processor.get_mem_image_format(widthp, heightp, colorsp, bpp); }

    int unpack()                                                         { return m_libraw_processor.unpack(); }
    int dcraw_process()                                                  { return m_libraw_processor.dcraw_process(); }
    libraw_processed_image_t* dcraw_make_mem_image(int* error_code=NULL) { return m_libraw_processor.dcraw_make_mem_image(error_code); }

protected:
    LibRaw m_libraw_processor;
};

/*!
 *
 * file_stream_device specialization for tiff images, which are based on TIFF*.
 */
template<>
class file_stream_device< cr2_tag > : public cr2_device_base
{
public:

    struct read_tag  {};
    struct write_tag {};

    file_stream_device( std::string const& file_name, read_tag )
    {
        io_error_if( m_libraw_processor.open_file(file_name.c_str()) != LIBRAW_SUCCESS , "file_stream_device: failed to open file" );
    }

    file_stream_device( std::string const& file_name, write_tag )
    {
        /// TODO
        /*
        TIFF* tiff;

        io_error_if( ( tiff = TIFFOpen( file_name.c_str(), "w" )) == NULL
                   , "file_stream_device: failed to open file" );

        _tiff_file = tiff_file_t( tiff, TIFFClose );
        */
    }

    file_stream_device( const LibRaw& processor ) : cr2_device_base( processor ) {}
};

/*!
 *
 * ostream_device specialization for tiff images.
 */
template<>
class ostream_device< cr2_tag > : public cr2_device_base
{
public:
    ostream_device( std::ostream & out ) : _out( out )
    {
        /// TODO
        /*
        TIFF* tiff;

        io_error_if( ( tiff = TIFFStreamOpen( ""
                                            , &_out
                                            )
                      ) == NULL
                   , "ostream_device: failed to stream"
                   );

        _tiff_file = tiff_file_t( tiff, TIFFClose );
        */
    }

private:
    std::ostream& _out;
};

/*!
 *
 * ostream_device specialization for tiff images.
 */
template<>
class istream_device< cr2_tag > : public cr2_device_base
{
public:
    istream_device( std::istream & in ) : _in( in )
    {
        /// TODO
        /*
        TIFF* tiff;

        io_error_if( ( tiff = TIFFStreamOpen( ""
                                            , &_in
                                            )
                     ) == NULL
                   , "istream_device: failed to stream"
                   );

        _tiff_file = tiff_file_t( tiff, TIFFClose );
        */
    }

private:
    std::istream& _in;
};

/*
template< typename T, typename D >
struct is_adaptable_input_device< tiff_tag, T, D > : mpl::false_{};
*/

/// TODO???
/*
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
*/

} // namespace detail
} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_DETAIL_CR2_IO_DEVICE_HPP_INCLUDED
