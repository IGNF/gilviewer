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
#include <wx/pen.h>
#include <wx/brush.h>

#include "ogr_vector_layer.hpp"

class draw_geometry_visitor : public boost::static_visitor<>
{
public:
    draw_geometry_visitor(wxDC &dc, wxPen point_pen, wxPen line_pen, wxPen poly_pen, wxBrush poly_brush, wxCoord x, wxCoord y, bool transparent, const layer_transform& trans):
            m_dc(dc),
            m_point_pen(point_pen), m_line_pen(line_pen), m_polygon_pen(poly_pen),
            m_polygon_brush(poly_brush),
            m_x(x), m_y(y),
            m_transparent(transparent),
            m_trans(trans)
    {}

    template <typename T>
    void operator()(T* operand) const
    {
        //throw std::invalid_argument("Unhandled geometry type!");
        std::cout << "[draw_geometry_visitor] Unhandled geometry type!" << std::endl;
    }
private:
    wxDC &m_dc;
    wxPen m_point_pen, m_line_pen, m_polygon_pen;
    wxBrush m_polygon_brush;
    wxCoord m_x, m_y;
    bool m_transparent;
    layer_transform m_trans;
};

template <>
void draw_geometry_visitor::operator()(OGRLinearRing* operand) const
{
    m_dc.SetPen(m_line_pen);
    // Merge with ()(OGRLineString*)?
    for(int j=0;j<operand->getNumPoints()-1;++j)
    {
        wxPoint p1=m_trans.from_local_int(operand->getX(j),operand->getY(j));
        wxPoint p2=m_trans.from_local_int(operand->getX(j+1),operand->getY(j+1));
        m_dc.DrawLine(p1,p2);
    }
}

template <>
void draw_geometry_visitor::operator()(OGRLineString* operand) const
{
    m_dc.SetPen(m_line_pen);
    for(int j=0;j<operand->getNumPoints()-1;++j)
    {
        wxPoint p1=m_trans.from_local_int(operand->getX(j),operand->getY(j));
        wxPoint p2=m_trans.from_local_int(operand->getX(j+1),operand->getY(j+1));
        m_dc.DrawLine(p1,p2);
    }
}

template <>
void draw_geometry_visitor::operator()(OGRMultiLineString* operand) const
{
    m_dc.SetPen(m_line_pen);
    for(int i=0;i<operand->getNumGeometries();++i)
        (*this)((OGRLineString*)operand->getGeometryRef(i));
}

template <>
void draw_geometry_visitor::operator()(OGRPoint* operand) const
{
    m_dc.SetPen(m_point_pen);
    wxPoint p1=m_trans.from_local_int(operand->getX(),operand->getY());
    m_dc.DrawLine(p1,p1);
}

template <>
void draw_geometry_visitor::operator()(OGRMultiPoint* operand) const
{
    m_dc.SetPen(m_point_pen);
    for(int i=0;i<operand->getNumGeometries();++i)
        (*this)((OGRPoint*)operand->getGeometryRef(i));
}

template <>
void draw_geometry_visitor::operator()(OGRPolygon* operand) const
{
    m_dc.SetPen(m_polygon_pen);
    m_dc.SetBrush(m_polygon_brush);
    // TODO: draw polygons
    std::vector<wxPoint> points;
    for(int j=0;j<operand->getExteriorRing()->getNumPoints();++j)
    {
        wxPoint p=m_trans.from_local_int(operand->getExteriorRing()->getX(j),operand->getExteriorRing()->getY(j));
        points.push_back(p);
    }
    m_dc.DrawPolygon(points.size(),&points.front());
    for(int i=0;i<operand->getNumInteriorRings();++i)
    {
        std::vector<wxPoint> points;
        for(int j=0;j<operand->getInteriorRing(i)->getNumPoints();++j)
        {
            wxPoint p=m_trans.from_local_int(operand->getInteriorRing(i)->getX(j),operand->getInteriorRing(i)->getY(j));
            points.push_back(p);
        }
        m_dc.DrawPolygon(points.size(),&points.front());
    }
}

template <>
void draw_geometry_visitor::operator()(OGRMultiPolygon* operand) const
{
    m_dc.SetPen(m_polygon_pen);
    m_dc.SetBrush(m_polygon_brush);
    for(int i=0;i<operand->getNumGeometries();++i)
        (*this)((OGRMultiPolygon*)operand->getGeometryRef(i));
}

#endif // DRAW_GEOMETRY_VISITOR_HPP
