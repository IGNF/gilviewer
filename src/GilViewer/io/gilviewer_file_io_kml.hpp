/***********************************************************************

This file is part of the GilViewer project source files.

GilViewer is an open source 2D viewer (raster and vector) based on Boost
GIL and wxWidgets.


Homepage:

        http://code.google.com/p/gilviewer

Copyright:

        Institut Geographique National (2009)

Authors:

        Olivier Tournaire, Adrien Chauve




    GilViewer is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GilViewer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with GilViewer.  If not, see <http://www.gnu.org/licenses/>.

***********************************************************************/
#ifndef GILVIEWER_FILE_IO_KML_HPP
#define GILVIEWER_FILE_IO_KML_HPP

#include "../config/config.hpp"
#if GILVIEWER_USE_GDAL

#include "gilviewer_file_io.hpp"

class gilviewer_file_io_kml : public gilviewer_file_io
{
public:
    virtual ~gilviewer_file_io_kml() {}

    virtual boost::shared_ptr<layer> load(const std::string &filename, const std::ptrdiff_t top_left_x=0, const std::ptrdiff_t top_left_y=0, const std::ptrdiff_t dim_x=0, const std::ptrdiff_t dim_y=0);
    virtual void save(boost::shared_ptr<layer> layer, const std::string &filename);

    static bool Register();
    friend boost::shared_ptr<gilviewer_file_io_kml> create_gilviewer_file_io_kml();

private:
    gilviewer_file_io_kml() {}
};

#endif // GILVIEWER_FILE_IO_KML_HPP

#endif // GILVIEWER_USE_GDAL
