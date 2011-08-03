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

#ifndef __SIMPLE_VECTOR_LAYER_HPP__
#define __SIMPLE_VECTOR_LAYER_HPP__

#include "vector_layer.hpp"

#include <boost/serialization/vector.hpp>

// TODO: move in basic_geometries.hpp
typedef struct __circle
{
    double x, y, radius;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(x)
           & BOOST_SERIALIZATION_NVP(y)
           & BOOST_SERIALIZATION_NVP(radius);
    }
} CircleType;

typedef struct __ellipse
{
    double x, y, a, b;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(x)
           & BOOST_SERIALIZATION_NVP(y)
           & BOOST_SERIALIZATION_NVP(a)
           & BOOST_SERIALIZATION_NVP(b);
    }
} EllipseType;

typedef struct __rotated_ellipse : public EllipseType
{
    double theta;
    std::vector<wxPoint> controlPoints;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(EllipseType)
           & BOOST_SERIALIZATION_NVP(theta)
           & BOOST_SERIALIZATION_NVP(controlPoints);
    }
} RotatedEllipseType;

typedef struct __point
{
    double x, y;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(x)
           & BOOST_SERIALIZATION_NVP(y);
    }
} PointType;

typedef struct __arc
{
    std::vector<PointType> arc_points;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(arc_points);
    }
} ArcType;

namespace boost { namespace serialization {

template<class Archive>
void serialize(Archive & ar, wxPoint & p, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(p.x)
       & BOOST_SERIALIZATION_NVP(p.y);
}

} // namespace serialization
} // namespace boost

class simple_vector_layer: public vector_layer
{
public:
    simple_vector_layer(const std::string& layer_name="default layer name");
    virtual ~simple_vector_layer() {}

    virtual void draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent) const;
    virtual void update(int, int) {}

    virtual layer_settings_control* build_layer_settings_control(unsigned int index, layer_control* parent);
    virtual std::string infos();

    virtual bool snap( eSNAP snap, double d2[], const wxRealPoint& p, wxRealPoint& psnap );

    virtual std::string available_formats_wildcard() const;

    void add_circle( double x , double y , double radius );
    void add_line( double x1 , double y1 , double x2 , double y2 );
    void add_polyline( const std::vector<double> &x , const std::vector<double> &y );
    void add_point( double x , double y );
    void add_spline( std::vector<PointType> points );
    void add_ellipse(double x_center, double y_center, double a, double b);
    void add_ellipse(double x_center, double y_center, double a, double b, double theta);
    void add_polygon( const std::vector<double> &x , const std::vector<double> &y );
    void add_text( double x , double y , const std::string &text , const wxColour &color = *wxRED );

    virtual void clear();

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        // note, version is always the latest when saving
        ar & BOOST_SERIALIZATION_NVP(m_circles)
           & BOOST_SERIALIZATION_NVP(m_ellipses)
           & BOOST_SERIALIZATION_NVP(m_rotatedellipses)
           & BOOST_SERIALIZATION_NVP(m_arcs)
           & BOOST_SERIALIZATION_NVP(m_points)
           & BOOST_SERIALIZATION_NVP(m_splines)
           & BOOST_SERIALIZATION_NVP(m_polygons);
    }

private:
    std::vector<CircleType> m_circles;
    std::vector<EllipseType> m_ellipses;
    std::vector<RotatedEllipseType> m_rotatedellipses;
    std::vector<ArcType> m_arcs;
    std::vector<PointType> m_points;
    std::vector< std::vector<PointType> > m_splines;
    std::vector< std::vector<PointType> > m_polygons;
    std::vector< std::pair< PointType, std::string > > m_texts;
};

#endif /* __SIMPLE_VECTOR_LAYER_HPP__ */
