/***********************************************************************

This file is part of the GilViewer project source files.

GilViewer is an open source 2D viewer (raster and vector) based on Boost
GIL and wxWidgets.


Homepage:

        http://code.google.com/p/gilviewer

Copyright:

        Institut Geographique National (2011)

Authors:

        Mathieu Brédif




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
#include "../config/config.hpp"
#if GILVIEWER_USE_CGAL

#include "cgal_vector_layer.hpp"

#include <boost/filesystem.hpp>
#include <boost/variant/detail/apply_visitor_unary.hpp>

#include <iostream>
#include <sstream>

#include "../gui/vector_layer_settings_control.hpp"
#include "../convenient/macros_gilviewer.hpp"

#include <wx/dc.h>
#include <wx/pen.h>
#include <wx/brush.h>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/IO/Arr_iostream.h>

struct Arrangement {
    typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
    typedef CGAL::Arr_segment_traits_2<Kernel> Traits_2;
    typedef Traits_2::Point_2                  Point_2;
    typedef Traits_2::X_monotone_curve_2       Segment_2;
    typedef CGAL::Arrangement_2<Traits_2>      Arrangement_2;
    typedef Arrangement_2::Vertex_handle       Vertex_handle;
    typedef Arrangement_2::Vertex_const_iterator       Vertex_const_iterator;
    typedef Arrangement_2::Edge_const_iterator       Edge_const_iterator;
    typedef Arrangement_2::Face_const_iterator       Face_const_iterator;

    void load(const std::string& filename)
    {

        std::ifstream f(filename.c_str());
        if (! f.is_open()) // Always test file open
        {
          std::cout << "Error opening input file" << std::endl;
          return;
        }
        f >> m_arrangement;
    }

    void save(const std::string& filename) const
    {

        std::ofstream f(filename.c_str());
        if (! f.is_open()) // Always test file open
        {
          std::cout << "Error opening output file" << std::endl;
          return;
        }
        f << m_arrangement;
    }


    void add_point( double x , double y )
    {
        Point_2 p(x,y);
        CGAL::insert_point(m_arrangement,p);
    }
    void add_line( double x1 , double y1 , double x2 , double y2 )
    {
        Point_2 p1(x1,y1);
        Point_2 p2(x2,y2);
        Segment_2 s(p1,p2);
        CGAL::insert(m_arrangement,s);
     }

    void add_polyline( const std::vector<double> &x , const std::vector<double> &y )
    {
        if(x.size()==1) {
            Point_2 p(x.front(),y.front());
            CGAL::insert_point(m_arrangement,p);
            return;
        }
        // TODO : keep handles to speed this up
        for(unsigned int i=1; i<x.size(); ++i)
        {
            Point_2 p(x[i-1],y[i-1]);
            Point_2 q(x[i  ],y[i  ]);
            Segment_2 s(p,q);
            CGAL::insert(m_arrangement,s);

        }
    }

    void add_polygon( const std::vector<double> &x , const std::vector<double> &y )
    {
        // TODO : keep handles to speed this up
        add_polyline(x,y);
        if(x.size()>2) {
            Point_2 p(x.front(),y.front());
            Point_2 q(x.back(),y.back());
            Segment_2 s(p,q);
            CGAL::insert(m_arrangement,s);
        }
    }


    void draw(wxDC &dc, wxPen point_pen, wxPen line_pen, wxPen poly_pen, wxBrush poly_brush, wxCoord x, wxCoord y, bool transparent, double r, double z, double tx, double ty, int coordinates) const {
        double delta = 0.5*r;

        dc.SetPen(poly_pen);
        dc.SetBrush(poly_brush);
        Face_const_iterator fit;
        for (fit = m_arrangement.faces_begin(); fit != m_arrangement.faces_end(); ++fit)
        {
            std::vector<wxPoint> points;
            if (fit->is_unbounded())  continue;
            Arrangement_2::Ccb_halfedge_const_circulator curr, first;
            curr = first = fit->outer_ccb();
            do {
                wxPoint p=cgal_vector_layer::from_local(z,tx,ty,delta,CGAL::to_double(curr->source()->point().x()),CGAL::to_double(curr->source()->point().y()),coordinates);
                points.push_back(p);
            } while ((++curr) != first);
            std::cout << points.size() << std::endl;
            dc.DrawPolygon(points.size(),&points.front());
            //TODO : interior rings
        }

        dc.SetPen(line_pen);
        Edge_const_iterator eit;
        for (eit = m_arrangement.edges_begin(); eit != m_arrangement.edges_end(); ++eit)
        {
            wxPoint p1=cgal_vector_layer::from_local(z,tx,ty,delta,CGAL::to_double(eit->source()->point().x()),CGAL::to_double(eit->source()->point().y()),coordinates);
            wxPoint p2=cgal_vector_layer::from_local(z,tx,ty,delta,CGAL::to_double(eit->target()->point().x()),CGAL::to_double(eit->target()->point().y()),coordinates);
            dc.DrawLine(p1,p2);
        }

        dc.SetPen(point_pen);
        Vertex_const_iterator vit;
        for (vit = m_arrangement.vertices_begin(); vit != m_arrangement.vertices_end(); ++vit) {
            wxPoint p=cgal_vector_layer::from_local(z,tx,ty,delta,CGAL::to_double(vit->point().x()),CGAL::to_double(vit->point().y()),coordinates);
            dc.DrawLine(p,p);
        }      
    }

    void clear() { m_arrangement.clear(); }

private:
    Arrangement_2 m_arrangement;
};


using namespace std;
using namespace boost::filesystem;

cgal_vector_layer::cgal_vector_layer(const string &layer_name, const string &filename_): vector_layer(),
        m_nb_geometries(0),
        m_coordinates(1)
{
    m_arrangement = new Arrangement;
    m_arrangement->load(filename_);

    m_infos = "CGAL vector layer";
    name(layer_name);
    filename( system_complete(filename_).string() );
    default_display_parameters();
    notifyLayerSettingsControl_();
}

void cgal_vector_layer::save( const std::string& filename ) const {
    m_arrangement->save(filename);
}

cgal_vector_layer::~cgal_vector_layer()
{
    if(m_arrangement) delete m_arrangement;
}


void cgal_vector_layer::draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent) const
{
    wxPen point_pen(m_point_color,m_point_width);
    wxPen line_pen(m_line_color,m_line_width,m_line_style);
    wxPen polygon_pen(m_polygon_border_color,m_polygon_border_width,m_polygon_border_style);
    wxBrush polygon_brush(m_polygon_inner_color,m_polygon_inner_style);
    m_arrangement->draw(dc,point_pen,line_pen,polygon_pen,polygon_brush,x,y,transparent,resolution(),zoom_factor(),translation_x(),translation_y(),m_coordinates);
}

layer_settings_control* cgal_vector_layer::build_layer_settings_control(unsigned int index, layer_control* parent)
{
    return new vector_layer_settings_control(index, parent);
}
/*
void cgal_vector_layer::compute_center(OGRLayer* layer, int nb_layers)
{
    static double ratio=1./(double)nb_layers;
    OGREnvelope env;
    layer->GetExtent(&env);
    m_center_x+=ratio*(env.MaxX+env.MinX)/2.;
    m_center_y+=ratio*(env.MaxY+env.MinY)/2.;
}
*/


string cgal_vector_layer::available_formats_wildcard() const
{
    ostringstream wildcard;
    wildcard << "All supported vector files (*.shp;*.kml)|*.shp;*.SHP;*.kml;*.KML|";
    wildcard << "SHP (*.shp)|*.shp;*.SHP|";
    wildcard << "KML (*.kml)|*.kml;*.KML";
    return wildcard.str();
}
/*
const std::vector<std::pair<geometry_types,OGRFeature*> >& cgal_vector_layer::geometries_features() const {return m_geometries_features;}

std::vector<std::pair<geometry_types,OGRFeature*> >& cgal_vector_layer::geometries_features() {return m_geometries_features;}
*/

wxPoint cgal_vector_layer::from_local(double zoomFactor, double translationX, double translationY, double delta, double x, double y, int coordinates = 1 /*IMAGE_COORDINATES*/)
{
    return wxPoint( static_cast<wxCoord>((delta+            x+translationX)/zoomFactor),
                    static_cast<wxCoord>((delta+coordinates*y+translationY)/zoomFactor) );
}
void cgal_vector_layer::add_point( double x , double y )
{
}
void cgal_vector_layer::add_line( double x1 , double y1 , double x2 , double y2 )
{
 }

void cgal_vector_layer::add_polyline( const std::vector<double> &x , const std::vector<double> &y )
{
}

void cgal_vector_layer::add_polygon( const std::vector<double> &x , const std::vector<double> &y )
{
 }

/*

void cgal_vector_layer::add_text( double x , double y , const std::string &text , const wxColour &color)
{
    internal_point_type pt;
    pt.x=x; pt.y=y;
    m_texts.push_back( make_pair<internal_point_type,string>(pt,text) );
}

void cgal_vector_layer::add_circle( double x , double y , double radius )
{
    OGRFeature *poFeature = new OGRFeature(new OGRFeatureDefn);
    OGRLineString* l= new OGRLineString();
    for(unsigned int i=0;i<=360;++i)
        l->addPoint(x+radius*cos((double)i*3.1415/180.),y+radius*sin((double)i*3.1415/180.));
    poFeature->SetGeometry(l);
    m_geometries_features.push_back(std::make_pair<OGRLineString*,OGRFeature*>(l,poFeature));
    m_nb_geometries+=l->getNumPoints();
}

void cgal_vector_layer::add_spline( std::vector<std::pair<double, double> > points )
{
    cout << "Not implemented!!! (" << __FUNCTION__ << ")" << endl;
}

void cgal_vector_layer::add_ellipse(double x_center, double y_center, double a, double b)
{
    cout << "Not implemented!!! (" << __FUNCTION__ << ")" << endl;
}

void cgal_vector_layer::add_ellipse(double x_center, double y_center, double a, double b, double theta)
{
    cout << "Not implemented!!! (" << __FUNCTION__ << ")" << endl;
}
*/

void cgal_vector_layer::clear()
{
    if (m_arrangement) m_arrangement->clear();
}

// TODO: notify, settings control, shared_ptr, IMAGE or GEOGRAPHIC coordinates ...

#endif // GILVIEWER_USE_CGAL
