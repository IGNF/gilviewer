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

#include "GilViewer/layers/VectorLayerGhost.h"

VectorLayerGhost::VectorLayerGhost(bool isCarto) :
	m_pointPosition(0, 0), m_rectangleSelection(std::make_pair(wxPoint(0, 0), wxPoint(0, 0)) ), m_isCarto(isCarto), m_drawPointPosition(false),
	m_drawRectangleSelection(false), m_rectangleSelectionFirstPointSet(false),
	m_drawCircle(false), m_CircleFirstPointSet(false),
	m_drawLine(false), m_lineEndCapture(false), m_lineHasBegun(false)
{
}

wxPoint VectorLayerGhost::Transfo(const wxPoint &pt, const double zoomFactor, const double translationX, const double translationY, const double resolution)
{
	const double delta = 0.5;// * resolution;
	return wxPoint((delta+pt.x + translationX ) / zoomFactor, (delta+pt.y +translationY)/zoomFactor);
}

void VectorLayerGhost::Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent, const double zoomFactor, const double translationX, const double translationY, const double resolution)
{
	if (m_drawCircle)
	{
		dc.SetPen(m_penCircle);
		dc.SetBrush(m_brushCircle);
		dc.DrawCircle(Transfo(m_circle.first, zoomFactor, translationX, translationY, resolution), m_circle.second/zoomFactor);
	}

	// m_pointPosition
	if (m_drawPointPosition)
	{
		dc.SetPen(m_penPoint);
		dc.DrawLine(Transfo(m_pointPosition, zoomFactor, translationX, translationY, resolution) , Transfo(m_pointPosition, zoomFactor, translationX, translationY, resolution));
	}

	// m_rectangleSelection
	if (m_drawRectangleSelection)
	{
		dc.SetPen(m_penRectangle);
		// p0 ------- p1
		// |		  |
		// p2 ------- p3

		//
		wxPoint p0(Transfo(m_rectangleSelection.first, zoomFactor, translationX -0.5, translationY -0.5, resolution) );
		wxPoint p3(Transfo(m_rectangleSelection.second, zoomFactor, translationX +0.5, translationY +0.5, resolution) );
		wxPoint p1(p0.x, p3.y);
		wxPoint p2(p3.x, p0.y);

		dc.DrawLine(p0, p1);
		dc.DrawLine(p1, p3);
		dc.DrawLine(p3, p2);
		dc.DrawLine(p2, p0);
	}

	if ( m_drawLine )
	{
		dc.SetPen(m_penLine);
		unsigned int  i;
		for (i=0;i<m_linePoints.size()-1;++i)
			dc.DrawLine(Transfo(m_linePoints[i], zoomFactor, translationX, translationY, resolution),Transfo(m_linePoints[i+1], zoomFactor, translationX, translationY, resolution));
		dc.DrawLine(Transfo(m_linePoints[i], zoomFactor, translationX, translationY, resolution),Transfo(m_linePoints.back(), zoomFactor, translationX, translationY, resolution));
	}

}
