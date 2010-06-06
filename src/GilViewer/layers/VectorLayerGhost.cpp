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

#include "VectorLayerGhost.h"

wxPoint vector_layer_ghost::FromLocal(const wxPoint &p, double delta) const
{
	return wxPoint(
		wxCoord((p.x +m_translationX+delta)/m_zoomFactor),
		wxCoord((p.y +m_translationY+delta)/m_zoomFactor));
}

wxPoint vector_layer_ghost::ToLocal(const wxPoint &p, double delta) const
{
	return wxPoint(
		wxCoord(m_zoomFactor*p.x -m_translationX+0.5-delta),
		wxCoord(m_zoomFactor*p.y -m_translationY+0.5-delta));
}

vector_layer_ghost::vector_layer_ghost(bool isCarto) :
	m_pointPosition(0, 0), m_rectangleSelection(std::make_pair(wxPoint(0, 0), wxPoint(0, 0)) ), m_isCarto(isCarto), m_drawPointPosition(false),
	m_drawRectangleSelection(false), m_rectangleSelectionFirstPointSet(false),
	m_drawCircle(false), m_CircleFirstPointSet(false),
	m_drawLine(false), m_lineEndCapture(false), m_lineHasBegun(false),
	m_zoomFactor(1.), m_translationX(0.), m_translationY(0.)
{
}

wxRect vector_layer_ghost::GetRectangle() const {
	wxPoint p(m_rectangleSelection.first );
	wxPoint q(m_rectangleSelection.second);
	if(p.x>q.x) std::swap(p.x,q.x);
	if(p.y>q.y) std::swap(p.y,q.y);
	return wxRect(FromLocal(p,0),FromLocal(q,1));
}

void vector_layer_ghost::Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent)
{
	if (m_drawCircle)
	{
		dc.SetPen(m_penCircle);
		dc.SetBrush(m_brushCircle);
		dc.DrawCircle(FromLocal(m_circle.first), wxCoord(m_circle.second/m_zoomFactor) );
	}

	// m_pointPosition
	if (m_drawPointPosition)
	{
		dc.SetPen(m_penPoint);
		dc.SetBrush(m_brushCircle);
		dc.DrawLine(FromLocal(m_pointPosition) , FromLocal(m_pointPosition));
	}

	// m_rectangleSelection
	if (m_drawRectangleSelection)
	{
		dc.SetPen(m_penRectangle);
		dc.SetBrush(m_brushRectangle);
		dc.DrawRectangle(GetRectangle());
	}

	if ( m_drawLine )
	{
		dc.SetPen(m_penLine);
		unsigned int  i;
		for (i=0;i<m_linePoints.size()-1;++i)
			dc.DrawLine(FromLocal(m_linePoints[i]),FromLocal(m_linePoints[i+1]));
		dc.DrawLine(FromLocal(m_linePoints[i]),FromLocal(m_linePoints.back()));
	}

}
