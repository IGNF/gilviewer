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

#include "GilViewer/gui/vector_layer_settings_control.hpp"
#include "GilViewer/convenient/macros_gilviewer.hpp"
#include "GilViewer/convenient/wxrealpoint.hpp"

#include <wx/dc.h>
#include <wx/pen.h>
#include <wx/brush.h>
#include <wx/log.h>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Arr_extended_dcel.h>
#include <CGAL/IO/Arr_iostream.h>
#include <CGAL/Arr_walk_along_line_point_location.h>
#include <CGAL/squared_distance_2.h>

#include <wx/log.h>
#include <shapefil.h>

#include "../convenient/utils.hpp"

struct Vertex_data {
    Vertex_data() : selected(false) {}
    bool selected;
};

struct Halfedge_data {
    Halfedge_data() : selected(false) {}
    bool selected;

};

struct Face_data {
    Face_data() : selected(false) {}
    bool selected;
};

bool clip( wxPoint& p, wxPoint& q, wxCoord x0, wxCoord y0, wxCoord x1, wxCoord y1 )
{
    double r0[] = { x0, y0 };
    double r1[] = { x1, y1 };
    double d[] = { q.x-p.x, q.y-p.y };
    double p0[] = { p.x, p.y };
    double a[] = { p.x, p.y };
    double b[] = { q.x, q.y };
    double tmin = 0, tmax = 1;
    for(int i=0; i<2; ++i) {
        if(d[i]==0 && (p0[i]<r0[i] || p0[i]>r1[i])) {
            return false;
        } else {
            int j = 1-i, k = (d[i] > 0);
            int l = 1-k;
            double m[] = {r0[i], r1[i]}, t;
            t = (m[l]-p0[i])/d[i];
            if(t>tmin) { tmin=t; a[i]=m[l];  a[j]=p0[j]+t*d[j]; } // a=p+td
            t = (m[k]-p0[i])/d[i];
            if(t<tmax) { tmax=t; b[i]=m[k];  b[j]=p0[j]+t*d[j]; } // b=p+td
            if(tmax <= tmin) return false;
        }
    }
    p.x = wxCoord(a[0]); p.y = wxCoord(a[1]);
    q.x = wxCoord(b[0]); q.y = wxCoord(b[1]);
    return true;
}

struct Arrangement {
    typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
    typedef Kernel::FT FT;
    typedef CGAL::Arr_segment_traits_2<Kernel> Traits_2;
    typedef Traits_2::Point_2                  Point_2;
    typedef Traits_2::Vector_2                  Vector_2;
    typedef Traits_2::Segment_2                  Segment_2;
    typedef Traits_2::X_monotone_curve_2       Curve_2;
    typedef CGAL::Arr_extended_dcel<Traits_2,Vertex_data,Halfedge_data,Face_data>      Dcel;
    typedef CGAL::Arrangement_2<Traits_2,Dcel>      Arrangement_2;
    typedef Arrangement_2::Vertex_const_iterator       Vertex_const_iterator;
    typedef Arrangement_2::Edge_const_iterator       Edge_const_iterator;
    typedef Arrangement_2::Face_const_iterator       Face_const_iterator;
    typedef Arrangement_2::Isolated_vertex_iterator       Isolated_vertex_iterator;

    typedef Arrangement_2::Vertex_const_handle    Vertex_const_handle;
    typedef Arrangement_2::Halfedge_const_handle  Halfedge_const_handle;
    typedef Arrangement_2::Face_const_handle      Face_const_handle;
    typedef Arrangement_2::Vertex_handle    Vertex_handle;
    typedef Arrangement_2::Halfedge_handle  Halfedge_handle;
    typedef Arrangement_2::Face_handle      Face_handle;
    typedef CGAL::Arr_walk_along_line_point_location<Arrangement_2> Point_location; // does not seem to work with Arr_trapezoid_ric_point_location
    typedef Arrangement_2::Inner_ccb_const_iterator Inner_ccb_const_iterator;


    class ObjectCache : public std::map<wxRealPoint,Point_2> {};

    Arrangement() : m_pl(m_arrangement) {}

    void load(const std::string& filename)
    {
        std::cout << "loading " << filename << std::endl;
        boost::filesystem::path file(boost::filesystem::system_complete(filename));

        std::string ext = BOOST_FILESYSTEM_STRING(file.extension());
        if(ext==".cgal") load_cgal(filename);
        else if(ext==".shp") load_shp(filename);
        else std::cerr << "unknown file type" << std::endl;
    }

    void load_shp(const std::string& filename)
    {
        std::cout << "loading as an shp " << filename << std::endl;
        SHPHandle handle = SHPOpen( filename.c_str() , "rb+" );
        if(!handle)
        {
            std::ostringstream oss;
            oss << "[" << __FUNCTION__ << ", l. " << __LINE__ << "]: Exception --> Problem reading file " << filename << std::endl;
            throw std::ios_base::failure(oss.str());
        }

        // Infos
        int nbEntities, type;
        double minBound[4], maxBound[4];
        SHPGetInfo(handle, &nbEntities, &type, minBound, maxBound);
        std::cout << nbEntities << " entities" << std::endl;
        std::cout << type << " type" << SHPT_POLYGONZ <<" "<< SHPT_POLYGON<<std::endl;
        if(nbEntities==0) return;
        SHPObject* obj0 = SHPReadObject(handle, 0);
        for (int i = 0; i < nbEntities; ++i)
        {
            SHPObject* obj = SHPReadObject(handle, i);
            // Polygons with holes not handled
            if (obj->panPartStart)
            {
                //  std::cout << i << ","<<obj->nParts<< std::endl;
                for(int k=0; k<obj->nParts; ++k)
                {
                    std::vector<double> x, y;
                    int jbeg = obj->panPartStart[k];
                    int jend = (k+1==obj->nParts)?obj->nVertices:obj->panPartStart[k+1];
                    for (int j = jbeg; j < jend; ++j)
                    {
                        x.push_back(obj->padfX[j]-obj0->padfX[0]);
                        y.push_back(obj->padfY[j]-obj0->padfY[0]);
                        add_point(x.back(), y.back());
                        if(j>jbeg) add_line(x.back(),y.back(),x[j-1-jbeg], y[j-1-jbeg]);
                    }
                    //  add_polygon(x,y);
                }
            }
            else
            {
                std::vector<double> x, y;
                for (int j = 0; j < obj->nVertices; ++j)
                {
                    x.push_back(obj->padfX[j]-obj0->padfX[0]);
                    y.push_back(obj->padfY[j]-obj0->padfY[0]);
                    //    add_point(x.back(), y.back());
                }
            }
            //   add_polygon(x,y);
        }
    }

    void load_cgal(const std::string& filename)
    {
        std::cout << "loading as a cgal file " << filename << std::endl;
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
        Point_2 p = cached_point(x,y);
        CGAL::insert_point(m_arrangement,p,m_pl);
        m_cache.clear();
    }
    void add_line( double x1 , double y1, double x2 , double y2 )
    {
        Point_2 p0 = cached_point(x1,y1);
        Point_2 p1 = cached_point(x2,y2);
        CGAL::insert(m_arrangement,Curve_2(p0,p1));
        m_cache.clear();
    }
    void add_polyline( const std::vector<double> &x , const std::vector<double> &y )
    {
        std::vector<Curve_2> curves;
        Point_2 p0 = cached_point(x.front(),y.front()), p1;
        for(unsigned int i=1; i<x.size(); ++i, p0=p1)
        {
            p1 = cached_point(x[i],y[i]);
            curves.push_back(Curve_2(p0,p1));
        }
        CGAL::insert(m_arrangement,curves.begin(),curves.end());
        m_cache.clear();
    }

    void add_polygon( const std::vector<double> &x , const std::vector<double> &y )
    {
        std::vector<Curve_2> curves;
        Point_2 p0 = cached_point(x.back(),y.back()), p1;
        for(unsigned int i=0; i<x.size(); ++i, p0=p1)
        {
            p1 = cached_point(x[i],y[i]);
            curves.push_back(Curve_2(p0,p1));
        }
        CGAL::insert(m_arrangement,curves.begin(),curves.end());
        m_cache.clear();
    }

    void draw(wxDC &dc, wxPen point_pen[2], wxPen line_pen[2], wxPen poly_pen[2], wxBrush poly_brush[2], wxCoord x, wxCoord y, bool transparent, const layer_transform& trans) const {
        Face_const_iterator fit;

        wxCoord x0,y0,w,h;
        dc.GetDeviceOrigin(&x0,&y0);
        dc.GetSize(&w,&h);

        for (fit = m_arrangement.faces_begin(); fit != m_arrangement.faces_end(); ++fit)
        {
            if (fit->is_unbounded()) continue;
            dc.SetPen(poly_pen[fit->data().selected]);
            dc.SetBrush(poly_brush[fit->data().selected]);

            unsigned int n = 1+fit->number_of_inner_ccbs();
            std::vector<int> count;
            std::vector<wxPoint> points;

            int prev_size = 0;
            { // outer ccb
                Arrangement_2::Ccb_halfedge_const_circulator curr, first;
                curr = first = fit->outer_ccb();
                wxPoint p=trans.from_local_int(CGAL::to_double(curr->source()->point().x()),CGAL::to_double(curr->source()->point().y()));
                points.push_back(p);
                do {
                    p=trans.from_local_int(CGAL::to_double(curr->target()->point().x()),CGAL::to_double(curr->target()->point().y()));
                    points.push_back(p);
                } while ((++curr) != first);
                count.push_back(points.size()-prev_size);
                prev_size = points.size();

            }

            for(Inner_ccb_const_iterator it = fit->inner_ccbs_begin(); it != fit->inner_ccbs_end(); ++it)
            {
                Arrangement_2::Ccb_halfedge_const_circulator curr, first;
                curr = first = *it;
                wxPoint p=trans.from_local_int(CGAL::to_double(curr->source()->point().x()),CGAL::to_double(curr->source()->point().y()));
                points.push_back(p);
                do {
                    p=trans.from_local_int(CGAL::to_double(curr->target()->point().x()),CGAL::to_double(curr->target()->point().y()));
                    points.push_back(p);
                } while ((++curr) != first);
                count.push_back(points.size()-prev_size);
                prev_size = points.size();
            }
            dc.DrawPolyPolygon(n,&(count.front()),&(points.front()),0,0,wxWINDING_RULE);
        }

        Edge_const_iterator eit;
        for (eit = m_arrangement.edges_begin(); eit != m_arrangement.edges_end(); ++eit)
        {
            dc.SetPen(line_pen[eit->data().selected]);
            wxPoint p1=trans.from_local_int(CGAL::to_double(eit->source()->point().x()),CGAL::to_double(eit->source()->point().y()));
            wxPoint p2=trans.from_local_int(CGAL::to_double(eit->target()->point().x()),CGAL::to_double(eit->target()->point().y()));
            if( clip(p1,p2,x0,y0,x0+w,y0+h) )
                dc.DrawLine(p1,p2);
        }

        Vertex_const_iterator vit;
        for (vit = m_arrangement.vertices_begin(); vit != m_arrangement.vertices_end(); ++vit) {
            dc.SetPen(point_pen[vit->data().selected]);
            wxPoint p=trans.from_local_int(CGAL::to_double(vit->point().x()),CGAL::to_double(vit->point().y()));
            dc.DrawLine(p,p);
        }
    }

    void select(const layer_transform& trans, double d2[], const wxRealPoint& p)
    {
        wxLogMessage(_("selecting"));
        wxRealPoint psnap = p;
        CGAL::Object obj = snap(trans,d2,p,psnap);
        Vertex_const_handle   v;
        Halfedge_const_handle h;
        Face_const_handle     f;

        if      (CGAL::assign (f, obj)) {
            m_arrangement.non_const_handle(f)->data().selected = !m_arrangement.non_const_handle(f)->data().selected;
        }
        else if (CGAL::assign (h, obj)) {
            bool selected =  !m_arrangement.non_const_handle(h)->data().selected;
            m_arrangement.non_const_handle(h        )->data().selected = selected;
            m_arrangement.non_const_handle(h->twin())->data().selected = selected;
        }
        else if (CGAL::assign (v, obj)) {
            m_arrangement.non_const_handle(v)->data().selected = !m_arrangement.non_const_handle(v)->data().selected;
        }
        m_cache.clear();
    }

    CGAL::Object snap(const layer_transform& trans, double d2[], const wxRealPoint& p, wxRealPoint& psnap )
    {
        FT zoom = trans.zoom_factor();
        FT invzoom2 = 1.0/(zoom*zoom);
        wxRealPoint plocal = trans.to_local(p);
        Point_2 q(plocal.x,plocal.y);
        Point_2 qsnap(q);

        CGAL::Object obj = m_pl.locate (q);

        Vertex_const_handle   v;
        Halfedge_const_handle h;
        Face_const_handle     f;

        if (CGAL::assign (f, obj)) {
            Arrangement_2::Ccb_halfedge_const_circulator curr, first;
            if (!f->is_unbounded())
            {
                curr = first = f->outer_ccb();
                do {
                    Segment_2 s(curr->source()->point(),curr->target()->point());
                    FT d = CGAL::squared_distance(s,q)*invzoom2;
                    if(d<d2[SNAP_LINE]) {
                        d2[SNAP_LINE]=CGAL::to_double(d);
                        h = curr;
                    }
                } while ((++curr) != first);

            }
            for(Inner_ccb_const_iterator it = f->inner_ccbs_begin(); it != f->inner_ccbs_end(); ++it)
            {
                curr = first = *it;
                do {
                    Segment_2 s(curr->source()->point(),curr->target()->point());
                    FT d = CGAL::squared_distance(s,q)*invzoom2;
                    if(d<d2[SNAP_LINE]) {
                        d2[SNAP_LINE]=CGAL::to_double(d);
                        h = curr;
                    }
                } while ((++curr) != first);

            }
            // trickery : ArrangementDcelFace misses a const iterator
            Arrangement_2& arr = (Arrangement_2&) m_arrangement;
            Face_handle g = arr.non_const_handle(f);

            for(Isolated_vertex_iterator it = g->isolated_vertices_begin(); it != g->isolated_vertices_end(); ++it)
            {
                FT d = CGAL::squared_distance(it->point(),q)*invzoom2;
                if(d<d2[SNAP_POINT]) {
                    d2[SNAP_POINT]=CGAL::to_double(d);
                    v = it;
                }

            }
        }

        if(CGAL::assign (h, obj)) d2[SNAP_LINE]=0;

        if(h!=Halfedge_const_handle())
        {
            for(unsigned int i=0; i<SNAP_LINE;++i) d2[i]=0;
            FT d = CGAL::squared_distance(h->source()->point(),q)*invzoom2;
            if(d<d2[SNAP_POINT]) {
                d2[SNAP_POINT]=CGAL::to_double(d);
                v = h->source();
            }
            d = CGAL::squared_distance(h->target()->point(),q)*invzoom2;
            if(d<d2[SNAP_POINT]) {
                d2[SNAP_POINT]=CGAL::to_double(d);
                v = h->target();
            }
            if(v==Vertex_const_handle())
            {
                obj = CGAL::make_object(h);

                Segment_2 s(h->source()->point(),h->target()->point());
                // implementation de qsnap = project(q,s);
                Vector_2 u = q-s.source();
                Vector_2 v = s.target()-s.source();
                qsnap = s.source()+v*((u*v)/v.squared_length());
            }
        }

        if(CGAL::assign (v, obj))  d2[SNAP_POINT]=0;

        if(v!=Vertex_const_handle())
        {
            for(unsigned int i=0; i<SNAP_LINE; ++i) d2[i]=0;
            obj = CGAL::make_object(v);
            qsnap = v->point();
        }

        psnap = trans.from_local(CGAL::to_double(qsnap.x()),CGAL::to_double(qsnap.y()));
        wxRealPoint psnaplocal = trans.to_local(psnap);
        m_cache[psnaplocal] = qsnap;
        return obj;
    }


    void clear() { m_arrangement.clear(); }

            private:
    Point_2 cached_point(double x, double y) const {
        ObjectCache::const_iterator it = m_cache.find(wxRealPoint(x,y));
        if (it != m_cache.end()) return it->second;
        return Point_2(x,y);
    }

    Arrangement_2 m_arrangement;
    Point_location m_pl;
    CGAL::Object m_snap;
    mutable ObjectCache m_cache;
};


using namespace std;
using namespace boost::filesystem;


void cgal_vector_layer::load(const std::string& filename_)
{
    m_arrangement->load(filename_);
    filename( system_complete(filename_).string() );
}

cgal_vector_layer::cgal_vector_layer(const string &layer_name)
{
    name(layer_name);
    m_arrangement = new Arrangement;
    m_infos = "CGAL vector layer";
    default_display_parameters();
    notifyLayerSettingsControl_();
    m_selection_color = wxColour(255, 255, 0); //*wxYELLOW;
}

cgal_vector_layer::cgal_vector_layer(const string &layer_name, const string &filename_)
{
    name(layer_name);
    m_arrangement = new Arrangement;
    load(filename_);
    m_infos = "CGAL vector layer";
    default_display_parameters();
    notifyLayerSettingsControl_();
    m_selection_color = wxColour(255, 255, 0); //*wxYELLOW;
}

void cgal_vector_layer::save( const std::string& filename ) const {
    m_arrangement->save(filename);
}

cgal_vector_layer::~cgal_vector_layer()
{
    if(m_arrangement) delete m_arrangement;
}

bool cgal_vector_layer::snap( eSNAP snap, double d2[], const wxRealPoint& p, wxRealPoint& psnap )
{
    Arrangement::Face_const_handle f;
    return !(CGAL::assign(f,m_arrangement->snap(transform(),d2,p,psnap)));
}

void cgal_vector_layer::draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent) const
{
    wxPen point_pen[] = { wxPen(m_point_color,m_point_width), wxPen(m_selection_color,m_point_width) };
    wxPen line_pen[] = { wxPen(m_line_color,m_line_width,m_line_style), wxPen(m_selection_color,m_line_width,m_line_style) };
    wxPen polygon_pen[] = { wxPen(m_polygon_border_color,m_polygon_border_width,m_polygon_border_style), wxPen(m_selection_color,m_polygon_border_width,m_polygon_border_style) };
    wxBrush polygon_brush[] = { wxBrush(m_polygon_inner_color,m_polygon_inner_style), wxBrush(m_selection_color,m_polygon_inner_style) };
    m_arrangement->draw(dc,point_pen,line_pen,polygon_pen,polygon_brush,x,y,transparent,transform());
}

layer_settings_control* cgal_vector_layer::build_layer_settings_control(unsigned int index, layer_control* parent)
{
    return new vector_layer_settings_control(index, parent);
}

string cgal_vector_layer::available_formats_wildcard() const
{
    ostringstream wildcard;
    wildcard << "All supported vector files (*.shp;*.kml)|*.shp;*.SHP;*.kml;*.KML;*.cgal;*.CGAL|";
    wildcard << "SHP (*.shp)|*.shp;*.SHP|";
    wildcard << "KML (*.kml)|*.kml;*.KML";
    wildcard << "CGAL (*.cgal)|*.cgal;*.CGAL";
    return wildcard.str();
}

void cgal_vector_layer::add_point( double x , double y )
{
    m_arrangement->add_point(x,y);
}
void cgal_vector_layer::add_line( double x1 , double y1 , double x2 , double y2 )
{
    m_arrangement->add_line(x1,y1,x2,y2);
}

void cgal_vector_layer::add_polyline( const std::vector<double> &x , const std::vector<double> &y )
{
    m_arrangement->add_polyline(x,y);
}

void cgal_vector_layer::add_polygon( const std::vector<double> &x , const std::vector<double> &y )
{
    m_arrangement->add_polygon(x,y);
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



void cgal_vector_layer::select(const wxRealPoint& p)
{
    double d2[SNAP_MAX_ID];
    d2[SNAP_POINT] = 100;
    d2[SNAP_LINE] = 100;
    m_arrangement->select(transform(),d2,p);
}

#endif // GILVIEWER_USE_CGAL
