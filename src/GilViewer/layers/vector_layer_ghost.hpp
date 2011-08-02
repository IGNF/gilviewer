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

#ifndef VECTORLAYERGENERIC_H_
#define VECTORLAYERGENERIC_H_

#include <utility>
#include <vector>

#include <wx/pen.h>
#include <wx/brush.h>
#include <wx/gdicmn.h>

#include "layer_transform.hpp"

class wxDC;
class wxPoint;

class vector_layer_ghost
{
public:
    vector_layer_ghost( bool isCarto = false );
    wxRect rectangle() const;
    wxRect local_rectangle(const layer_transform& trans) const;

    void draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent);

    wxRealPoint m_pointPosition;
    std::pair<wxRealPoint,wxRealPoint>  m_rectangleSelection;
    std::vector<wxRealPoint> m_linePoints;

    typedef std::pair< wxRealPoint , double > CircleType;
    CircleType m_circle;

    bool m_isCarto;
    bool m_drawPointPosition;
    bool m_drawRectangleSelection;
    bool m_rectangleSelectionFirstPointSet;
    bool m_drawCircle;
    bool m_CircleFirstPointSet; //=center
    bool m_drawLine;
    bool m_lineEndCapture; // existance du premier point de la polyligne
    bool m_lineHasBegun;

    wxPen m_penPoint, m_penRectangle, m_penCircle, m_penLine;
    wxBrush m_brushRectangle, m_brushCircle;

    layer_transform& transform() { return m_layer_transform; }
    const layer_transform& transform() const { return m_layer_transform; }

private:
    // transformation du layer
    layer_transform m_layer_transform;
};

#endif /*VECTORLAYERGENERIC_H_*/
