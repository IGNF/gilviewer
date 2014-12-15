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

#ifndef VECTOR_LAYER_GHOST_HPP
#define VECTOR_LAYER_GHOST_HPP

#include <vector>

#include <wx/pen.h>
#include <wx/brush.h>

#include "layer_transform.hpp"
#include "boost/variant.hpp"

class wxDC;
class wxPoint;

class vector_layer_ghost
{
public:

    // constructor
    vector_layer_ghost();

    // input types
    struct Nothing {};
    typedef wxRealPoint Point;
    typedef std::pair<Point,Point> Rectangle;
    typedef std::pair<Point,double> Circle;
    class Polyline : public std::vector<Point> {};
    class Polygon : public std::vector<Point> {};
    typedef boost::variant<Nothing,Point,Rectangle,Circle,Polyline,Polygon> variant_input;


    // input reseting (same input type)
    void reset();
    // input reseting to a new given input type
    template<typename T> void reset()
    {
        m_input = T();
        m_complete = false;
        m_num_inputs = 0;
    }

    // drawing and geometry modifications
    void draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent);
    bool add_point(const wxRealPoint& p, bool final = false);
    void move_relative(const wxRealPoint& p);
    void move_absolute(const wxRealPoint& p);
    void update_relative(const wxRealPoint& p);
    void update_absolute(const wxRealPoint& p);

    // rendering style
    wxPen m_penPoint, m_penRectangle, m_penCircle, m_penLine;
    wxBrush m_brushRectangle, m_brushCircle;

    // transform
    layer_transform& transform() { return m_layer_transform; }
    const layer_transform& transform() const { return m_layer_transform; }

    // input is ready for consumption
    bool complete() const { return m_complete; }
    inline void complete(bool c) { m_complete = c; }
    inline unsigned int num_inputs() const { return m_num_inputs; }

    // get the input
    const variant_input& get() const { return m_input; }
    template<typename T> inline const T* get() const { return boost::get<T>(&m_input); }

    bool snap( eSNAP snap, double d2[], const wxRealPoint& p, wxRealPoint& psnap );

private:
    layer_transform m_layer_transform;
    variant_input m_input;
    bool m_complete;
    unsigned int m_num_inputs;
};

#endif /*VECTOR_LAYER_GHOST_HPP*/
