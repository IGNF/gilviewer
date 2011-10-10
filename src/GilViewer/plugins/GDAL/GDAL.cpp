#include "GDAL.hpp"

IMPLEMENT_PLUGIN(GDAL_plugin);

#include "gilviewer_file_io_shp.hpp"
#include "gilviewer_file_io_kml.hpp"
#include "gilviewer_file_io_gdal_jp2.hpp"

bool GDAL_plugin::Register(gilviewer_io_factory *factory)
{
    gilviewer_file_io_shp     ().Register(factory);
    gilviewer_file_io_kml     ().Register(factory);
    gilviewer_file_io_gdal_jp2().Register(factory);
    return true;
}
