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
#ifndef GILVIEWER_FILE_IO_TIFF_HPP
#define GILVIEWER_FILE_IO_TIFF_HPP

#include <boost/gil/extension/io_new/tiff_all.hpp>
#include "gilviewer_file_io_image.hpp"

class gilviewer_file_io_tiff : public gilviewer_file_io_image<boost::gil::tiff_tag>
{
public:
    virtual ~gilviewer_file_io_tiff() {}

    virtual std::string get_infos(const std::string &filename);

    virtual bool Register(gilviewer_io_factory *factory);
};

#endif // GILVIEWER_FILE_IO_TIFF_HPP
