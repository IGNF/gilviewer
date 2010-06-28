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

#include <wx/dc.h>
#include <wx/pen.h>
#include <wx/brush.h>

#include "../gui/VectorLayerSettingsControl.hpp"

#include "simple_vector_layer.hpp"

#include <fstream>
#include <sstream>
using namespace std;

simple_vector_layer::simple_vector_layer(const std::string& layer_name): vector_layer(),
    m_circles(std::vector<CircleType>() ),
    m_ellipses(std::vector<EllipseType>() ),
    m_rotatedellipses(std::vector<RotatedEllipseType> ()),
    m_arcs(std::vector<ArcType> ()),
    m_points(std::vector<PointType> ()),
    m_splines(std::vector< std::vector<PointType> > ()),
    m_polygons(std::vector< std::vector<PointType> > ())
{
    m_name=layer_name;

    filename( "" );
    default_display_parameters();
    notifyLayerSettingsControl_();
}

//void simple_vector_layer::Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent, double zoomFactor, double translationX, double translationY, double resolution) const
void simple_vector_layer::draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent) const
{
    const double delta = 0.5 * resolution();

    wxPen pen;
    wxBrush brush;

    // 2D
    pen.SetWidth(m_polygon_border_width);
    pen.SetColour(m_polygon_border_color);
    pen.SetStyle(m_polygon_border_style);
    brush.SetColour(m_polygon_inner_color);
    brush.SetStyle(m_polygon_inner_style);
    dc.SetPen(pen);
    dc.SetBrush(brush);
    for (unsigned int i = 0; i < m_circles.size(); i++)
    {
        wxPoint p = simple_vector_layer::from_local(
                zoom_factor(),translation_x(),translation_y(),delta,
                m_circles[i].x,m_circles[i].y);
        wxCoord r = static_cast<wxCoord>(m_circles[i].radius / zoom_factor());
        dc.DrawCircle(p, r);
    }
    // Ellipses alignees
    for (unsigned int i=0;i<m_ellipses.size();++i)
    {
        wxPoint p = simple_vector_layer::from_local(
                zoom_factor(),translation_x(),translation_y(),delta,
                m_ellipses[i].x,m_ellipses[i].y);
        wxSize  s(
                static_cast<wxCoord>(2*m_ellipses[i].a/zoom_factor()),
                static_cast<wxCoord>(2*m_ellipses[i].b/zoom_factor())
                );
        dc.DrawEllipse(p,s);
        //dc.DrawEllipticArc(p,s,0.,90.);
        //dc.DrawEllipticArcRot(p,s,0.,90.);
    }
    // Ellipses non alignees
    for (unsigned int i=0;i<m_rotatedellipses.size();++i)
    {
        std::vector<wxPoint> points;
        points.reserve( m_rotatedellipses[i].controlPoints.size() );
        for (unsigned int j=0;j<m_rotatedellipses[i].controlPoints.size();++j)
            points.push_back( simple_vector_layer::from_local(
                    zoom_factor(),translation_x(),translation_y(),delta,
                    m_rotatedellipses[i].controlPoints[j].x,
                    m_rotatedellipses[i].controlPoints[j].y ));
        dc.DrawSpline(points.size(),&points.front());
    }
    for (unsigned int i=0;i<m_polygons.size();++i)
    {
        std::vector<wxPoint> points;
        points.reserve( m_polygons[i].size() );
        for (unsigned int j=0;j<m_polygons[i].size();++j)
            points.push_back(simple_vector_layer::from_local(
                    zoom_factor(),translation_x(),translation_y(),delta,
                    m_polygons[i][j].x,m_polygons[i][j].y ));
        dc.DrawPolygon( points.size() , &(points.front()) );
    }

    // 1D
    pen.SetColour(m_line_color);
    pen.SetWidth(m_line_width);
    pen.SetStyle(m_line_style);
    dc.SetPen(pen);
    for (unsigned int i = 0; i < m_arcs.size(); i++)
    {
        const ArcType &local_tab = m_arcs[i];
        for (unsigned int j = 0; j < local_tab.arc_points.size() - 1; ++j)
        {
            wxPoint p = simple_vector_layer::from_local(
                    zoom_factor(),translation_x(),translation_y(),delta,
                    local_tab.arc_points[j  ].x,local_tab.arc_points[j  ].y);
            wxPoint q = simple_vector_layer::from_local(
                    zoom_factor(),translation_x(),translation_y(),delta,
                    local_tab.arc_points[j+1].x,local_tab.arc_points[j+1].y);
            dc.DrawLine(p,q);
        }
    }
    // Splines
    for (unsigned int i=0;i<m_splines.size();++i)
    {
        const std::vector<PointType>& spline = m_splines[i];
        int n = spline.size();
        std::vector<wxPoint> points;
        for (int j=0;j<n;++j)
        {
            wxPoint p = simple_vector_layer::from_local(
                    zoom_factor(),translation_x(),translation_y(),delta,
                    spline[j].x,spline[j].y);
            points.push_back(p);
        }
        dc.DrawSpline(n,&points.front());
    }

    // 0D
    pen.SetColour(m_point_color);
    pen.SetWidth(m_point_width);
    dc.SetPen(pen);
    for (unsigned int i = 0; i < m_points.size(); i++)
    {
        wxPoint p = simple_vector_layer::from_local(
                zoom_factor(),translation_x(),translation_y(),delta,
                m_points[i].x,m_points[i].y);
        //dc.DrawLine(p);
        dc.DrawPoint(p);
    }

    // Text
    if(text_visibility())
    {
        pen.SetColour(m_text_color);
        dc.SetPen(pen);
        for (unsigned int i = 0; i < m_texts.size(); i++)
        {
            wxPoint p = simple_vector_layer::from_local(
                    zoom_factor(),translation_x(),translation_y(),delta,
                    m_texts[i].first.x,m_texts[i].first.y);
            //dc.DrawLine(p);
            dc.DrawText(wxString(m_texts[i].second.c_str(),*wxConvCurrent),p);
        }
    }
}

void simple_vector_layer::add_circle(double x, double y, double radius)
{
    CircleType ct;
    ct.x = x;
    ct.y = y;
    ct.radius = radius;
    m_circles.push_back(ct);
}

void simple_vector_layer::add_line(double x1, double y1, double x2, double y2)
{
    ArcType unArc;
    PointType unPoint;
    unPoint.x = x1; unPoint.y = y1;
    unArc.arc_points.push_back(unPoint);
    unPoint.x = x2; unPoint.y = y2;
    unArc.arc_points.push_back(unPoint);
    m_arcs.push_back(unArc);
}

void simple_vector_layer::add_polyline( const std::vector<double> &x , const std::vector<double> &y )
{
    assert(x.size()==y.size());
    for(unsigned int i=0;i<x.size()-1;++i)
        add_line(x[i  ],y[i  ],
                x[i+1],y[i+1]);
}

void simple_vector_layer::add_point( double x , double y )
{
    PointType pt;
    pt.x=x; pt.y=y;
    m_points.push_back(pt);
}

void simple_vector_layer::add_spline( std::vector<PointType> points )
{
    m_splines.push_back( points );
}
void simple_vector_layer::add_polygon( const std::vector<double> &x , const std::vector<double> &y )
{
    if (x.size() != y.size())
        return;
    m_polygons.resize(m_polygons.size()+1);
    for (unsigned int j=0;j<x.size();++j)
    {
        PointType pt;
        pt.x=x[j]; pt.y=y[j];
        m_polygons.back().push_back(pt);
    }
}


void simple_vector_layer::add_ellipse(double x_center, double y_center, double a, double b)
{
    EllipseType et;
    et.x = x_center-a;
    et.y = y_center-b;
    et.a = a;
    et.b = b;
    m_ellipses.push_back(et);
}

void simple_vector_layer::add_ellipse(double dx_center, double dy_center, double da, double db, double theta)
{
    RotatedEllipseType et;
    et.x = dx_center;
    et.y = dy_center;
    et.a = da;
    et.b = db;
    et.theta = theta;
    // Construction de la liste des points
    static const double EToBConst =	0.2761423749154;
    wxSize offset((int)(2*da*EToBConst), (int)(2*db*EToBConst));

    wxCoord x_center = (wxCoord) dx_center;
    wxCoord y_center = (wxCoord) dy_center;
    wxCoord a        = (wxCoord) da;
    wxCoord b        = (wxCoord) db;
    et.controlPoints.push_back(wxPoint(x_center-a,y_center));
    et.controlPoints.push_back(wxPoint(x_center-a,y_center-offset.GetY()));
    et.controlPoints.push_back(wxPoint(x_center-offset.GetX(),y_center-b));
    et.controlPoints.push_back(wxPoint(x_center,y_center-b));
    et.controlPoints.push_back(wxPoint(x_center+offset.GetX(),y_center-b));
    et.controlPoints.push_back(wxPoint(x_center+a,y_center-offset.GetY()));
    et.controlPoints.push_back(wxPoint(x_center+a,y_center));
    et.controlPoints.push_back(wxPoint(x_center+a,y_center+offset.GetY()));
    et.controlPoints.push_back(wxPoint(x_center+offset.GetX(),y_center+b));
    et.controlPoints.push_back(wxPoint(x_center,y_center+b));
    et.controlPoints.push_back(wxPoint(x_center-offset.GetX(),y_center+b));
    et.controlPoints.push_back(wxPoint(x_center-a,y_center+offset.GetY()));
    et.controlPoints.push_back(wxPoint(x_center-a,y_center));

    m_rotatedellipses.push_back(et);
}

void simple_vector_layer::add_text( double x , double y , const std::string &text , const wxColour &color )
{
    PointType pt;
    pt.x=x; pt.y=y;
    m_texts.push_back( make_pair<PointType,string>(pt,text) );
}

void simple_vector_layer::clear()
{
    m_circles.clear();
    m_ellipses.clear();
    m_rotatedellipses.clear();
    m_arcs.clear();
    m_points.clear();
    m_splines.clear();
    m_polygons.clear();
    m_texts.clear();
    // deallocate memory
    vector<CircleType>().        swap(m_circles);
    vector<EllipseType>().       swap(m_ellipses);
    vector<RotatedEllipseType>().swap(m_rotatedellipses);
    vector<ArcType>().           swap(m_arcs);
    vector<PointType>().         swap(m_points);
    vector<vector<PointType> >().swap(m_splines);
    vector<vector<PointType> >().swap(m_polygons);
    vector< pair<PointType,string> >().swap(m_texts);
}

wxPoint simple_vector_layer::from_local(double zoomFactor, double translationX, double translationY, double delta, double x, double y, int coordinates) const {
        return wxPoint(
                static_cast<wxCoord>((delta+            x+translationX)/zoomFactor),
                static_cast<wxCoord>((delta+coordinates*y+translationY)/zoomFactor)
        );
    }

string simple_vector_layer::available_formats_wildcard() const
{
    ostringstream wildcard;
    wildcard << "All available serialization (*.txt;*.xml;*.bin)|*.txt;*.TXT;*.xml;*.XML;*.bin;*.BIN|";
    wildcard << "Serialization text (*.txt)|*.txt;*.TXT|";
    wildcard << "Serialization xml (*.xml)|*.xml;*.XML|";
    wildcard << "Serialization binary (*.bin)|*.bin;*.BIN";
    return wildcard.str();
}

layer_settings_control* simple_vector_layer::build_layer_settings_control(unsigned int index, layer_control* parent)
{
    return new vector_layer_settings_control(index, parent);
}

std::string simple_vector_layer::infos()
{
    ostringstream oss;
    oss << m_circles.size() << " circles\n";
    oss << m_ellipses.size() << " ellipses\n";
    oss << m_rotatedellipses.size() << " rotated ellipses\n";
    oss << m_arcs.size() << " arcs\n";
    oss << m_points.size() << " points\n";
    oss << m_splines.size() << " splines\n";
    oss << m_polygons.size() << " polygons\n";
    m_infos = oss.str();
    return m_infos;
}
