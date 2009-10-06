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
#include <wx/pen.h>
#include <wx/brush.h>

#include "GilViewer/layers/VectorLayerMultiGeometries.hpp"

VectorLayerMultiGeometries::VectorLayerMultiGeometries() : VectorLayerContent(),
	m_pen( new wxPen(wxColour(255, 0, 0, 255))),
	m_brush(new wxBrush(wxColour(255, 0, 0, 255)))
{
}

VectorLayerMultiGeometries::~VectorLayerMultiGeometries()
{
}

void VectorLayerMultiGeometries::Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent, const double zoomFactor, const double translationX, const double translationY, const double resolution)
{
	const double delta = 0.5 * resolution;
	dc.SetPen(*m_pen);
	dc.SetBrush(*m_brush);
	for (unsigned int i = 0; i < m_circles.size(); i++)
		dc.DrawCircle((delta + m_circles[i].x + translationX) / zoomFactor, (delta + m_flagPRJ * m_circles[i].y + translationY) / zoomFactor, m_circles[i].radius / zoomFactor);
	for (unsigned int i = 0; i < m_arcs.size(); i++)
	{
		const simpleArcType &local_tab = m_arcs[i];
		for (unsigned int j = 0; j < local_tab.size() - 1; ++j)
		{
			float posx1 = (delta + local_tab[j].first + translationX) / zoomFactor;
			double posy1 = (delta + m_flagPRJ * local_tab[j].second + translationY) / zoomFactor;
			double posx2 = (delta + local_tab[j + 1].first + translationX) / zoomFactor;
			double posy2 = (delta + m_flagPRJ * local_tab[j + 1].second + translationY) / zoomFactor;
			dc.DrawLine(posx1, posy1, posx2, posy2);
		}
	}
	for (unsigned int i = 0; i < m_points.size(); i++)
	{
		double x = (delta+m_points[i].first + translationX ) / zoomFactor;
		double y = (delta+m_flagPRJ*m_points[i].second +translationY)/zoomFactor;
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
			points.push_back( wxPoint((delta+spline[j].first + translationX ) / zoomFactor,(delta+m_flagPRJ*spline[j].second +translationY)/zoomFactor) );
		}
		dc.DrawSpline(n,&points.front());
	}
	// Ellipses alignees
	for (unsigned int i=0;i<m_ellipses.size();++i)
	{
		dc.DrawEllipse(wxPoint((delta+m_ellipses[i].x+translationX)/zoomFactor,(delta+m_flagPRJ*m_ellipses[i].y+translationY)/zoomFactor),wxSize(2*m_ellipses[i].a/zoomFactor,2*m_ellipses[i].b/zoomFactor));
		//dc.DrawEllipticArc(wxPoint((delta+m_ellipses[i].x+translationX)/zoomFactor,(delta+m_flagPRJ*m_ellipses[i].y+translationY)/zoomFactor),wxSize(2*m_ellipses[i].a/zoomFactor,2*m_ellipses[i].b/zoomFactor),0.,90.);
		//dc.DrawEllipticArcRot(wxPoint((delta+m_ellipses[i].x+translationX)/zoomFactor,(delta+m_flagPRJ*m_ellipses[i].y+translationY)/zoomFactor),wxSize(2*m_ellipses[i].a/zoomFactor,2*m_ellipses[i].b/zoomFactor),0.,90.);
	}
	// Ellipses non alignees
	for (unsigned int i=0;i<m_rotatedellipses.size();++i)
	{
		std::vector<wxPoint> points;
		points.reserve( m_rotatedellipses[i].controlPoints.size() );
		for (unsigned int j=0;j<m_rotatedellipses[i].controlPoints.size();++j)
			points.push_back( wxPoint( (delta+m_rotatedellipses[i].controlPoints[j].x+translationX)/zoomFactor , (delta+m_flagPRJ*m_rotatedellipses[i].controlPoints[j].y+translationY)/zoomFactor ) );
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
