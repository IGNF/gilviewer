#include "gilviewer_io_factory.hpp"

#include "gilviewer_file_io_jpg.hpp"
#include "gilviewer_file_io_png.hpp"
#include "gilviewer_file_io_tiff.hpp"

#include "gilviewer_file_io_pk1.hpp"

#include "../config/config.hpp"
#if GILVIEWER_USE_GDAL
#   include "gilviewer_file_io_shp.hpp"
#   include "gilviewer_file_io_kml.hpp"
    #if GILVIEWER_USE_GDALJP2
    #   include "gilviewer_file_io_gdal_jp2.hpp"
    #endif // GILVIEWER_USE_GDALJP2
#endif // GILVIEWER_USE_GDAL
#if GILVIEWER_USE_CGAL
#   include "gilviewer_file_io_cgal.hpp"
#endif // GILVIEWER_USE_CGAL



#include "gilviewer_file_io_serialization_txt.hpp"
#include "gilviewer_file_io_serialization_xml.hpp"
#include "gilviewer_file_io_serialization_binary.hpp"

void register_all_image_file_formats()
{
    gilviewer_file_io_jpg::Register();
    gilviewer_file_io_png::Register();
    gilviewer_file_io_tiff::Register();
#if GILVIEWER_USE_GDAL
    #if GILVIEWER_USE_GDALJP2
       gilviewer_file_io_gdal_jp2::Register();
    #endif // GILVIEWER_USE_GDALJP2
#endif // GILVIEWER_USE_GDAL

#if GILVIEWER_USE_STEREOV2
    gilviewer_file_io_stereoV2_jp2::Register();
#endif // GILVIEWER_USE_STEREOV2
}

void register_all_vector_file_formats()
{
#if GILVIEWER_USE_GDAL
    gilviewer_file_io_shp::Register();
    gilviewer_file_io_kml::Register();
#endif // GILVIEWER_USE_GDAL
#if GILVIEWER_USE_CGAL
    gilviewer_file_io_cgal::Register();
#endif // GILVIEWER_USE_CGAL
    gilviewer_file_io_serialization_txt::Register();
    gilviewer_file_io_serialization_xml::Register();
    gilviewer_file_io_serialization_binary::Register();
    gilviewer_file_io_pk1::Register();
}

void register_all_file_formats()
{
    register_all_image_file_formats();
    register_all_vector_file_formats();
}
