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
#ifndef COMPUTE_CENTER_VISITOR_HPP
#define COMPUTE_CENTER_VISITOR_HPP

#include <boost/variant/static_visitor.hpp>

#include <iostream>

#include <gdal/ogrsf_frmts.h>

#include <wx/dc.h>

class compute_center_visitor : public boost::static_visitor<>
{
public:
    compute_center_visitor(double* center_x, double* center_y, unsigned int nb_geometries):
            m_center_x(center_x), m_center_y(center_y),
            m_nb_geometries(nb_geometries),
            m_ratio(1./(double)nb_geometries)
    {
    }

    template <typename T>
    void operator()(T* operand) const
    {
        //throw std::invalid_argument("Unhandled geometry type!");
        std::cout << "[compute_center_visitor] Unhandled geometry type!" << std::endl;
    }
private:
    double *m_center_x, *m_center_y;
    unsigned int m_nb_geometries;
    double m_ratio;
};

template <>
void compute_center_visitor::operator()(OGRLinearRing* operand) const
{
    // Merge with ()(OGRLineString*)?
    for(int j=0;j<operand->getNumPoints();++j)
    {
        *m_center_x+=m_ratio*operand->getX(j);
        *m_center_y+=m_ratio*operand->getY(j);
    }
}

template <>
void compute_center_visitor::operator()(OGRLineString* operand) const
{
    for(int j=0;j<operand->getNumPoints();++j)
    {
        *m_center_x+=m_ratio*operand->getX(j);
        *m_center_y+=m_ratio*operand->getY(j);
    }
}

template <>
void compute_center_visitor::operator()(OGRMultiLineString* operand) const
{
    for(int i=0;i<operand->getNumGeometries();++i)
        (*this)((OGRLineString*)operand->getGeometryRef(i));
}

template <>
void compute_center_visitor::operator()(OGRPoint* operand) const
{
    *m_center_x+=m_ratio*operand->getX();
    *m_center_y+=m_ratio*operand->getY();
}

template <>
void compute_center_visitor::operator()(OGRMultiPoint* operand) const
{
    for(int i=0;i<operand->getNumGeometries();++i)
        (*this)((OGRPoint*)operand->getGeometryRef(i));
}

template <>
void compute_center_visitor::operator()(OGRPolygon* operand) const
{
    for(int j=0;j<operand->getExteriorRing()->getNumPoints();++j)
    {
        *m_center_x+=m_ratio*operand->getExteriorRing()->getX(j);
        *m_center_y+=m_ratio*operand->getExteriorRing()->getY(j);
    }
    for(int i=0;i<operand->getNumInteriorRings();++i)
    {
        for(int j=0;j<operand->getInteriorRing(i)->getNumPoints();++j)
        {
            *m_center_x+=m_ratio*operand->getInteriorRing(i)->getX(j);
            *m_center_y+=m_ratio*operand->getInteriorRing(i)->getY(j);
        }
    }
}

template <>
void compute_center_visitor::operator()(OGRMultiPolygon* operand) const
{
    for(int i=0;i<operand->getNumGeometries();++i)
        (*this)((OGRMultiPolygon*)operand->getGeometryRef(i));
}

#endif // COMPUTE_CENTER_VISITOR_HPP
