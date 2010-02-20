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
#ifndef DRAW_GEOMETRY_VISITOR_HPP
#define DRAW_GEOMETRY_VISITOR_HPP

#include <boost/variant/static_visitor.hpp>

#include <iostream>

#include <gdal/ogrsf_frmts.h>

#include <wx/dc.h>

#include "ogr_vector_layer.hpp"

class draw_geometry_visitor : public boost::static_visitor<>
{
public:
    draw_geometry_visitor(wxDC &dc, wxCoord x, wxCoord y, bool transparent, double r, double z, double tx, double ty, int coordinates):
            m_dc(dc),
            m_x(x), m_y(y),
            m_transparent(transparent),
            m_r(r), m_z(z), m_tx(tx), m_ty(ty),
            m_delta(0.5*r),
            m_coordinates(coordinates)
    {}

    template <typename T>
    void operator()(T* operand) const
    {
        //throw std::invalid_argument("Unhandled geometry type!");
        std::cout << "[draw_geometry_visitor] Unhandled geometry type!" << std::endl;
    }
private:
    wxDC &m_dc;
    wxCoord m_x, m_y;
    bool m_transparent;
    double m_r, m_z, m_tx, m_ty, m_delta;
    int m_coordinates;
};

// TODO: use (implement) FromLocal

template <>
void draw_geometry_visitor::operator()(OGRLinearRing* operand) const
{
    // Merge with ()(OGRLineString*)?
    for(int j=0;j<operand->getNumPoints()-1;++j)
    {
        wxPoint p1=ogr_vector_layer::FromLocal(m_z,m_tx,m_ty,m_delta,operand->getX(j),operand->getY(j),m_coordinates);
        wxPoint p2=ogr_vector_layer::FromLocal(m_z,m_tx,m_ty,m_delta,operand->getX(j+1),operand->getY(j+1),m_coordinates);
        m_dc.DrawLine(p1,p2);
    }
}

template <>
void draw_geometry_visitor::operator()(OGRLineString* operand) const
{
    for(int j=0;j<operand->getNumPoints()-1;++j)
    {
        wxPoint p1=ogr_vector_layer::FromLocal(m_z,m_tx,m_ty,m_delta,operand->getX(j),operand->getY(j),m_coordinates);
        wxPoint p2=ogr_vector_layer::FromLocal(m_z,m_tx,m_ty,m_delta,operand->getX(j+1),operand->getY(j+1),m_coordinates);
        m_dc.DrawLine(p1,p2);
    }
}

template <>
void draw_geometry_visitor::operator()(OGRMultiLineString* operand) const
{
    for(int i=0;i<operand->getNumGeometries();++i)
        (*this)((OGRLineString*)operand->getGeometryRef(i));
}

template <>
void draw_geometry_visitor::operator()(OGRPoint* operand) const
{
    wxPoint p1=ogr_vector_layer::FromLocal(m_z,m_tx,m_ty,m_delta,operand->getX(),operand->getY(),m_coordinates);
    m_dc.DrawLine(p1,p1);
}

template <>
void draw_geometry_visitor::operator()(OGRMultiPoint* operand) const
{
    for(int i=0;i<operand->getNumGeometries();++i)
        (*this)((OGRPoint*)operand->getGeometryRef(i));
}

template <>
void draw_geometry_visitor::operator()(OGRPolygon* operand) const
{
    // TODO: draw polygons
    std::vector<wxPoint> points;
    for(int j=0;j<operand->getExteriorRing()->getNumPoints();++j)
    {
        wxPoint p=ogr_vector_layer::FromLocal(m_z,m_tx,m_ty,m_delta,operand->getExteriorRing()->getX(j),operand->getExteriorRing()->getY(j),m_coordinates);
        points.push_back(p);
    }
    m_dc.DrawPolygon(points.size(),&points.front());
    for(int i=0;i<operand->getNumInteriorRings();++i)
    {
        std::vector<wxPoint> points;
        for(int j=0;j<operand->getInteriorRing(i)->getNumPoints();++j)
        {
            wxPoint p=ogr_vector_layer::FromLocal(m_z,m_tx,m_ty,m_delta,operand->getInteriorRing(i)->getX(j),operand->getInteriorRing(i)->getY(j),m_coordinates);
            points.push_back(p);
        }
        m_dc.DrawPolygon(points.size(),&points.front());
    }
}

template <>
void draw_geometry_visitor::operator()(OGRMultiPolygon* operand) const
{
    for(int i=0;i<operand->getNumGeometries();++i)
        (*this)((OGRMultiPolygon*)operand->getGeometryRef(i));
}

#endif // DRAW_GEOMETRY_VISITOR_HPP
