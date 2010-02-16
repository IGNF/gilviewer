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
#ifndef GILVIEWER_TIFF_FILE_IO_HPP
#define GILVIEWER_TIFF_FILE_IO_HPP

#include "gilviewer_file_io.hpp"
#include <boost/shared_ptr.hpp>

class gilviewer_tiff_file_io : public gilviewer_file_io
{
public:
    virtual ~gilviewer_tiff_file_io() {}

    virtual void load();
    virtual void save();

    static bool Register();
    friend boost::shared_ptr<gilviewer_tiff_file_io> create_gilviewer_tiff_file_io();

private:
    gilviewer_tiff_file_io() {}
};

#endif // GILVIEWER_TIFF_FILE_IO_HPP
