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

#include <numeric>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include "vector_layer_ghost.hpp"
#include "../convenient/wxrealpoint.hpp"

// input visitors

struct vlg_drawer : public boost::static_visitor<void>
{
    vlg_drawer(const vector_layer_ghost& layer, wxDC &dc, wxCoord x, wxCoord y, bool transparent)
        : m_layer(layer), m_dc(dc), m_x(x), m_y(y), m_transparent(transparent) {}

    const vector_layer_ghost& m_layer;
    wxDC &m_dc;
    wxCoord m_x, m_y;
    bool m_transparent;

    void operator()(const vector_layer_ghost::Nothing&) const {}
    void operator()(const vector_layer_ghost::Point& p) const
    {
        m_dc.SetPen(m_layer.m_penPoint);
        m_dc.SetBrush(m_layer.m_brushCircle);
        m_dc.DrawLine(m_layer.transform().from_local_int(p) , m_layer.transform().from_local_int(p));
    }
    void operator()(const vector_layer_ghost::Rectangle& r) const
    {
        wxPoint p(m_layer.transform().from_local_int(r.first));
        wxPoint q(m_layer.transform().from_local_int(r.second));
        if(p.x>q.x) std::swap(p.x,q.x);
        if(p.y>q.y) std::swap(p.y,q.y);
        m_dc.SetPen(m_layer.m_penRectangle);
        m_dc.SetBrush(m_layer.m_brushRectangle);
        m_dc.DrawRectangle(wxRect(p,q));
    }
    void operator()(const vector_layer_ghost::Circle& c) const
    {
        m_dc.SetPen(m_layer.m_penCircle);
        m_dc.SetBrush(m_layer.m_brushCircle);
        m_dc.DrawCircle(m_layer.transform().from_local_int(c.first), wxCoord(c.second/m_layer.transform().zoom_factor() ));
    }
    void operator()(const vector_layer_ghost::Polyline& l) const
    {
        if(l.empty()) return;
        m_dc.SetPen(m_layer.m_penLine);
        for (unsigned int i=0;i<l.size()-1;++i)
            m_dc.DrawLine(m_layer.transform().from_local_int(l[i]),m_layer.transform().from_local_int(l[i+1]));
    }
    void operator()(const vector_layer_ghost::Polygon& p) const
    {
        if(p.empty()) return;
        m_dc.SetPen(m_layer.m_penLine);
        for (unsigned int i=0;i<p.size()-1;++i)
            m_dc.DrawLine(m_layer.transform().from_local_int(p[i]),m_layer.transform().from_local_int(p[i+1]));
        m_dc.DrawLine(m_layer.transform().from_local_int(p.back()),m_layer.transform().from_local_int(p.front()));
    }
};


struct vlg_point_adder : public boost::static_visitor<void>
{
    vlg_point_adder(vector_layer_ghost& layer, const wxRealPoint& p, bool final)
        : m_layer(layer), m_p(p), m_final(final) {}

    vector_layer_ghost& m_layer;
    wxRealPoint m_p;
    bool m_final;

    void operator()(vector_layer_ghost::Nothing&) { }
    void operator()(vector_layer_ghost::Point& p) {
        m_layer.complete(true);
        p = m_p;
    }
    void operator()(vector_layer_ghost::Rectangle& r)
    {
        m_layer.complete(m_layer.num_inputs()>1);
        r.second = m_p;
        if(!m_layer.complete()) r.first  = m_p;
    }
    void operator()(vector_layer_ghost::Circle& c)
    {
        m_layer.complete(m_layer.num_inputs()>1);
        if (m_layer.complete()) {
            c.second = std::sqrt(squared_distance(c.first, m_p));
        } else {
            c.first = m_p;
            c.second = 1;
        }
    }
    template<typename Poly> void operator()(Poly& poly)
    {
        m_layer.complete(m_final);
        // at first, insert twice to enable the absolute updating on the second vertex
        if(poly.empty()) poly.push_back(m_p);
        if(!m_final) poly.push_back(m_p);
    }
};

struct vlg_absolute_mover : public boost::static_visitor<void>
{
    vlg_absolute_mover(vector_layer_ghost& layer, const wxRealPoint& p)
        : m_layer(layer), m_p(p) {}

    vector_layer_ghost& m_layer;
    wxRealPoint m_p;

    void operator()(vector_layer_ghost::Nothing& ) const { }
    void operator()(vector_layer_ghost::Point& p ) const { p = m_p; }
    void operator()(vector_layer_ghost::Circle& c) const { c.first = m_p; }
    void operator()(vector_layer_ghost::Rectangle& r) const
    {
        wxRealPoint v = r.second - r.first;
        r.first = m_p - 0.5*v;
        r.second = r.first + v;
    }
    template<typename Poly> void operator()(Poly& poly) const
    {
        wxRealPoint bary(std::accumulate(poly.begin(), poly.end(), wxRealPoint(0, 0)));
        bary.x /= poly.size();
        bary.y /= poly.size();
        std::for_each(poly.begin(), poly.end(), boost::lambda::_1 += -bary + m_p);
    }
};

struct vlg_absolute_updater : public boost::static_visitor<void>
{
    vlg_absolute_updater(vector_layer_ghost& layer, const wxRealPoint& p)
        : m_layer(layer), m_p(p) {}

    vector_layer_ghost& m_layer;
    wxRealPoint m_p;

    void operator()(vector_layer_ghost::Nothing& ) const { }
    void operator()(vector_layer_ghost::Point& p ) const { }
    void operator()(vector_layer_ghost::Circle& c) const {
        if(m_layer.num_inputs()==1) c.second = std::sqrt(squared_distance(c.first, m_p));
    }
    void operator()(vector_layer_ghost::Rectangle& r) const
    {
        if(m_layer.num_inputs()==1) r.second = m_p;
    }
    template<typename Poly> void operator()(Poly& poly) const
    {
        if(!m_layer.complete() && !poly.empty()) poly.back() = m_p;
    }
};

struct vlg_relative_updater : public boost::static_visitor<void>
{
    vlg_relative_updater(vector_layer_ghost& layer, const wxRealPoint& p)
        : m_layer(layer), m_p(p) {}

    vector_layer_ghost& m_layer;
    wxRealPoint m_p;

    void operator()(vector_layer_ghost::Rectangle& r) const
    {
        if(!m_layer.complete()) r.second += m_p;
    }
    template<typename T> void operator()(T&) const {}
};

struct vlg_relative_mover : public boost::static_visitor<void>
{
    vlg_relative_mover(vector_layer_ghost& layer, const wxRealPoint& p)
        : m_layer(layer), m_p(p) {}

    vector_layer_ghost& m_layer;
    wxRealPoint m_p;

    void operator()(vector_layer_ghost::Nothing& ) const { }
    void operator()(vector_layer_ghost::Point& p ) const { p += m_p; }
    void operator()(vector_layer_ghost::Circle& c) const { c.first += m_p; }
    void operator()(vector_layer_ghost::Rectangle& r) const
    {
        r.first += m_p;
        r.second += m_p;
    }
    template<typename Poly> void operator()(Poly& poly) const
    {
        std::for_each(poly.begin(), poly.end(), boost::lambda::_1 += m_p);
    }
};

// vector_layer_ghost methods

vector_layer_ghost::vector_layer_ghost() :
        m_input(Nothing()),
        m_complete(true),
        m_num_inputs(0)
{
    m_penCircle = wxPen(*wxBLUE, 2);
    m_brushCircle = wxBrush(*wxBLUE, wxTRANSPARENT);
    m_penLine = wxPen(*wxBLUE, 1);
    m_penRectangle = wxPen(*wxRED, 2, wxDOT);
    m_brushRectangle = wxBrush(*wxRED, wxTRANSPARENT);
    m_penPoint = wxPen(*wxGREEN, 3);
}

void vector_layer_ghost::draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent)
{
    boost::apply_visitor(vlg_drawer(*this,dc,x,y,transparent),m_input);
}
bool vector_layer_ghost::add_point(const wxRealPoint& p, bool final)
{
    ++m_num_inputs;
    vlg_point_adder v(*this,transform().to_local(p),final);
    boost::apply_visitor(v,m_input);
    return complete();
}
void vector_layer_ghost::move_absolute(const wxRealPoint& p)
{
    boost::apply_visitor(vlg_absolute_mover(*this,transform().to_local(p)),m_input);
}
void vector_layer_ghost::update_absolute(const wxRealPoint& p)
{
    boost::apply_visitor(vlg_absolute_updater(*this,transform().to_local(p)),m_input);
}
void vector_layer_ghost::move_relative(const wxRealPoint& p)
{
    boost::apply_visitor(vlg_relative_mover(*this,p),m_input);
}
void vector_layer_ghost::update_relative(const wxRealPoint& p)
{
    boost::apply_visitor(vlg_relative_updater(*this,p),m_input);
}

// clearing

struct vlg_reseter : public boost::static_visitor<void> {
    template<typename T> void operator()(T& t) { t=T(); }
};

void vector_layer_ghost::reset() {
    vlg_reseter c;
    boost::apply_visitor(c,m_input);
    m_complete = false;
    m_num_inputs = 0;
}
