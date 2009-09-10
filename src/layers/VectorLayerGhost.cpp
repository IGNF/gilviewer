/***********************************************************************

This file is part of the GilViewer project source files.

GilViewer is an open source 2D viewer (raster and vector) based on Boost
GIL and wxWidgets. GilViewer is distributed under the CeCILL-B licence. 
See Licence_CeCILL-B_V1-en.txt or http://www.cecill.info for more details.


Homepage: 

	http://launchpad.net/gilviewer
	
Copyright:
	
	Institut Geographique National (2009)

Authors: 

	Olivier Tournaire, Adrien Chauve

	
	

This software is governed by the CeCILL-B license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL-B
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL-B license and that you accept its terms.
 
***********************************************************************/

#include <wx/dc.h>

#include "layers/VectorLayerGhost.h"

VectorLayerGhost::VectorLayerGhost(bool isCarto) :
	m_pointPosition(0, 0), m_rectangleSelection(std::make_pair(wxPoint(0, 0), wxPoint(0, 0)) ), m_isCarto(isCarto), m_drawPointPosition(false),
	m_drawRectangleSelection(false), m_rectangleSelectionFirstPointSet(false),
	m_drawCircle(false), m_CircleFirstPointSet(false),
	m_drawLine(false), m_lineEndCapture(false), m_lineHasBegun(false)
{
}

wxPoint VectorLayerGhost::Transfo(const wxPoint &pt, const float zoomFactor, const float translationX, const float translationY)
{
	return wxPoint((0.5+pt.x + translationX ) / zoomFactor, (0.5+pt.y +translationY)/zoomFactor);
}

void VectorLayerGhost::Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent, const float zoomFactor, const float translationX, const float translationY)
{
	if (m_drawCircle)
	{
		dc.SetPen(m_penCircle);
		dc.SetBrush(m_brushCircle);
		dc.DrawCircle(Transfo(m_circle.first, zoomFactor, translationX, translationY), m_circle.second/zoomFactor);
	}

	// m_pointPosition
	if (m_drawPointPosition)
	{
		dc.SetPen(m_penPoint);
		dc.DrawLine(Transfo(m_pointPosition, zoomFactor, translationX, translationY) , Transfo(m_pointPosition, zoomFactor, translationX, translationY));
	}

	// m_rectangleSelection
	if (m_drawRectangleSelection)
	{
		dc.SetPen(m_penRectangle);
		// p0 ------- p1
		// |		  |
		// p2 ------- p3
		//
		wxPoint p0(Transfo(m_rectangleSelection.first, zoomFactor, translationX, translationY) );
		wxPoint p3(Transfo(m_rectangleSelection.second, zoomFactor, translationX, translationY) );
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
			dc.DrawLine(Transfo(m_linePoints[i], zoomFactor, translationX, translationY),Transfo(m_linePoints[i+1], zoomFactor, translationX, translationY));
		dc.DrawLine(Transfo(m_linePoints[i], zoomFactor, translationX, translationY),Transfo(m_linePoints.back(), zoomFactor, translationX, translationY));
	}

}
