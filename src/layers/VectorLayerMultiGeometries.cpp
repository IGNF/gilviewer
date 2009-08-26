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
#include <wx/pen.h>
#include <wx/brush.h>

#include "layers/VectorLayerMultiGeometries.hpp"

VectorLayerMultiGeometries::VectorLayerMultiGeometries() : VectorLayerContent(),
	m_pen( new wxPen(wxColour(255, 0, 0, 255))),
	m_brush(new wxBrush(wxColour(255, 0, 0, 255)))
{
}

VectorLayerMultiGeometries::~VectorLayerMultiGeometries()
{
}

void VectorLayerMultiGeometries::Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent, const float zoomFactor, const float translationX, const float translationY)
{
	dc.SetPen(*m_pen);
	dc.SetBrush(*m_brush);
	for (unsigned int i = 0; i < m_circles.size(); i++)
		dc.DrawCircle((0.5 + m_circles[i].x + translationX) / zoomFactor, (0.5 + m_flagPRJ * m_circles[i].y + translationY) / zoomFactor, m_circles[i].radius / zoomFactor);
	for (unsigned int i = 0; i < m_arcs.size(); i++)
	{
		const simpleArcType &local_tab = m_arcs[i];
		for (unsigned int j = 0; j < local_tab.size() - 1; ++j)
		{
			float posx1 = (0.5 + local_tab[j].first + translationX) / zoomFactor;
			float posy1 = (0.5 + m_flagPRJ * local_tab[j].second + translationY) / zoomFactor;
			float posx2 = (0.5 + local_tab[j + 1].first + translationX) / zoomFactor;
			float posy2 = (0.5 + m_flagPRJ * local_tab[j + 1].second + translationY) / zoomFactor;
			dc.DrawLine(posx1, posy1, posx2, posy2);
		}
	}
	for (unsigned int i = 0; i < m_points.size(); i++)
	{
		double x = (0.5+m_points[i].first + translationX ) / zoomFactor;
		double y = (0.5+m_flagPRJ*m_points[i].second +translationY)/zoomFactor;
		//dc.DrawLine( x,y , x,y );
		dc.DrawPoint( x,y );
	}
	// Splines
	for (unsigned int i=0;i<m_splines.size();++i)
	{
		std::vector< simplePointType > spline = m_splines[i];
		int n = spline.size();
		std::vector<wxPoint> points;
		for (int j=0;j<n;++j)
		{
			points.push_back( wxPoint((0.5+spline[j].first + translationX ) / zoomFactor,(0.5+m_flagPRJ*spline[j].second +translationY)/zoomFactor) );
		}
		dc.DrawSpline(n,&points.front());
	}
	// Ellipses alignees
	for (unsigned int i=0;i<m_ellipses.size();++i)
	{
		dc.DrawEllipse(wxPoint((0.5+m_ellipses[i].x+translationX)/zoomFactor,(0.5+m_flagPRJ*m_ellipses[i].y+translationY)/zoomFactor),wxSize(2*m_ellipses[i].a/zoomFactor,2*m_ellipses[i].b/zoomFactor));
		//dc.DrawEllipticArc(wxPoint((0.5+m_ellipses[i].x+translationX)/zoomFactor,(0.5+m_flagPRJ*m_ellipses[i].y+translationY)/zoomFactor),wxSize(2*m_ellipses[i].a/zoomFactor,2*m_ellipses[i].b/zoomFactor),0.,90.);
		//dc.DrawEllipticArcRot(wxPoint((0.5+m_ellipses[i].x+translationX)/zoomFactor,(0.5+m_flagPRJ*m_ellipses[i].y+translationY)/zoomFactor),wxSize(2*m_ellipses[i].a/zoomFactor,2*m_ellipses[i].b/zoomFactor),0.,90.);
	}
	// Ellipses non alignees
	for (unsigned int i=0;i<m_rotatedellipses.size();++i)
	{
		std::vector<wxPoint> points;
		points.reserve( m_rotatedellipses[i].controlPoints.size() );
		for (unsigned int j=0;j<m_rotatedellipses[i].controlPoints.size();++j)
			points.push_back( wxPoint( (0.5+m_rotatedellipses[i].controlPoints[j].x+translationX)/zoomFactor , (0.5+m_flagPRJ*m_rotatedellipses[i].controlPoints[j].y+translationY)/zoomFactor ) );
		dc.DrawSpline(points.size(),&points.front());
	}
}

void VectorLayerMultiGeometries::AddCircle(double x, double y, double radius)
{
	CircleType ct;
	ct.x = x;
	ct.y = y;
	ct.radius = radius;
	m_circles.push_back(ct);
}

void VectorLayerMultiGeometries::AddLine(double x1, double y1, double x2, double y2)
{
	simpleArcType unArc;
	unArc.push_back(std::make_pair(x1, y1));
	unArc.push_back(std::make_pair(x2, y2));
	m_arcs.push_back(unArc);
}

void VectorLayerMultiGeometries::AddPoint( double x , double y )
{
	simplePointType pt = std::make_pair(x,y);
	m_points.push_back(pt);
}

void VectorLayerMultiGeometries::AddSpline( std::vector<simplePointType> points )
{
	m_splines.push_back( points );
}

void VectorLayerMultiGeometries::AddEllipse(double x_center, double y_center, double a, double b)
{
	EllipseType et;
	et.x = x_center-a;
	et.y = y_center-b;
	et.a = a;
	et.b = b;
	m_ellipses.push_back(et);
}

void VectorLayerMultiGeometries::AddEllipse(double x_center, double y_center, double a, double b, double theta)
{
	RotatedEllipseType et;
	et.x = x_center;
	et.y = y_center;
	et.a = a;
	et.b = b;
	et.theta = theta;
	// Construction de la liste des points
	static const double EToBConst =	0.2761423749154;
	wxSize offset((int)(2*a*EToBConst), (int)(2*b*EToBConst));

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

void VectorLayerMultiGeometries::SetBrushVectorLayerMultiGeometries(const wxBrush &brush)
{
	*m_brush = brush;
}

void VectorLayerMultiGeometries::SetBrushVectorLayerMultiGeometries(unsigned char red, unsigned char green, unsigned char blue, int style, unsigned char alpha)
{
	m_brush->SetColour(wxColor(red, green, blue, alpha));
	m_brush->SetStyle(style);
}

void VectorLayerMultiGeometries::SetPenVectorLayerMultiGeometries(const wxPen &pen)
{
	*m_pen = pen;
}

void VectorLayerMultiGeometries::SetPenVectorLayerMultiGeometries(unsigned char red, unsigned char green, unsigned char blue, int style, int width, unsigned char alpha)
{
	m_pen->SetColour(wxColor(red, green, blue, alpha));
	// Bug wx !!! Inverser width et style !!!
	m_pen->SetStyle(width);
	m_pen->SetWidth(style);
}

void VectorLayerMultiGeometries::Clear()
{
	m_bitmap.clear();
	m_circles.clear();
	m_arcs.clear();
	m_points.clear();
}
