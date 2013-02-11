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

#ifndef GET_GEOMETRY_VISITOR_HPP
#define GET_GEOMETRY_VISITOR_HPP

#include <boost/variant/static_visitor.hpp>

#include <iostream>

#include <gdal/ogrsf_frmts.h>

#include "ogr_vector_layer.hpp"

template<typename Geometry>
class get_geometry_visitor : public boost::static_visitor<>
{
public:
    get_geometry_visitor() {}

    template <typename T>
    void operator()(T* operand)
    {
    }

    void operator()(Geometry* operand)
    {
        m_geometries.push_back(operand);
    }

    std::vector<Geometry*> m_geometries;
};

#endif // GET_GEOMETRY_VISITOR_HPP
