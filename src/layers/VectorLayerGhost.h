/***********************************************************************

This file is part of the GilViewer project source files.

GilViewer is an open source 2D viewer (raster and vector) based on Boost
GIL and wxWidgets.


Homepage: 

	http://launchpad.net/gilviewer
	
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

class wxDC;
class wxPoint;

class VectorLayerGhost
{
	public:
		VectorLayerGhost( bool isCarto = false );
		virtual ~VectorLayerGhost() {}

		virtual void Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent, const float zoomFactor, const float translationX, const float translationY);

		wxPoint m_pointPosition;
		std::pair< wxPoint , wxPoint > m_rectangleSelection;
		std::vector<wxPoint> m_linePoints;

		typedef std::pair< wxPoint , double > CircleType;
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

		inline wxPoint Transfo(const wxPoint &pt, const float zoomFactor, const float translationX, const float translationY);
};

#endif /*VECTORLAYERGENERIC_H_*/
