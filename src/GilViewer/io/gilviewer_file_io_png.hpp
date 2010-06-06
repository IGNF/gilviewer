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
#ifndef GILVIEWER_FILE_IO_PNG_HPP
#define GILVIEWER_FILE_IO_PNG_HPP

#include "gilviewer_file_io.hpp"

class gilviewer_file_io_png : public gilviewer_file_io
{
public:
    virtual ~gilviewer_file_io_png() {}

    virtual boost::shared_ptr<Layer> load(const std::string &filename);
    virtual void save(boost::shared_ptr<Layer> layer, const std::string &filename);

    static bool Register();
    friend boost::shared_ptr<gilviewer_file_io_png> create_gilviewer_file_io_png();

private:
    gilviewer_file_io_png() {}
};

#endif // GILVIEWER_FILE_IO_PNG_HPP
