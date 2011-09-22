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
#ifndef GILVIEWER_FILE_IO_HPP
#define GILVIEWER_FILE_IO_HPP

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include "GilViewer/convenient/utils.hpp"
#include "GilViewer/plugins/plugin_base.hpp"

#include <string>

class layer;
class gilviewer_io_factory;

class gilviewer_file_io : public plugin_base
{
public:
    virtual ~gilviewer_file_io() {}

    virtual boost::shared_ptr<layer> load(const std::string &filename, const std::ptrdiff_t top_left_x=0, const std::ptrdiff_t top_left_y=0, const std::ptrdiff_t dim_x=0, const std::ptrdiff_t dim_y=0) { return boost::shared_ptr<layer>(); }

    virtual void save(boost::shared_ptr<layer> layer, const std::string &filename)=0;

    virtual std::string build_and_get_infos(const std::string &filename) { return ""; }

    virtual bool Register(gilviewer_io_factory *factory) { return false; }
protected:
    gilviewer_file_io() : m_familly(""), m_description("") {}
    /// This attribute describes the main familly of the reader (e.g. vector, image, ...)
    std::string m_familly;
    /// This attribute gives the individual description of the reader (e.g. TIFF images, SHP files ...)
    std::string m_description;
};

#endif // GILVIEWER_FILE_IO_HPP
