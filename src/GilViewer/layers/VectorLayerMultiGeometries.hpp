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

#ifndef VECTORLAYERMULTIGEOMETRIES_HPP_
#define VECTORLAYERMULTIGEOMETRIES_HPP_

class wxPen;
class wxBrush;

#include <vector>

#include "GilViewer/layers/VectorLayerContent.hpp"

// Dans ce type de calques, on met un peu de tout ...
// Du moins, tout ce qu'un wxDC permet de tracer
// A implementer au fur et a mesure
class VectorLayerMultiGeometries: public VectorLayerContent
{
public:
	typedef struct
	{
		double x;
		double y;
		double radius;
	} CircleType;

	typedef struct
	{
		double x;
		double y;
		double a;
		double b;
	} EllipseType;

	typedef struct : public EllipseType
	{
		double theta;
		std::vector<wxPoint> controlPoints;
	} RotatedEllipseType;

    typedef std::pair<double,double> simplePointType;
    typedef std::vector< simplePointType > simpleArcType;

	VectorLayerMultiGeometries();
	virtual ~VectorLayerMultiGeometries();

	virtual void Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent, const double zoomFactor, const double translationX, const double translationY, const double resolution);
	virtual void Save( const std::string &name ) {};

	void AddCircle( double x , double y , double radius );
	void AddLine( double x1 , double y1 , double x2 , double y2 );
	void AddPoint( double x , double y );
	void AddSpline( std::vector<simplePointType> points );
	void AddEllipse(double x_center, double y_center, double a, double b);
	void AddEllipse(double x_center, double y_center, double a, double b, double theta);
	void AddPolygon( const std::vector<double> &x , const std::vector<double> &y );

    void SetBrushVectorLayerMultiGeometries( const wxBrush &brush );
    void SetBrushVectorLayerMultiGeometries( unsigned char red , unsigned char green , unsigned char blue , int style = wxSOLID , unsigned char alpha=wxALPHA_OPAQUE );
    void SetPenVectorLayerMultiGeometries( const wxPen &pen );
    void SetPenVectorLayerMultiGeometries( unsigned char red , unsigned char green , unsigned char blue , int style = wxSOLID , int width = 1 , unsigned char alpha=wxALPHA_OPAQUE );

	virtual void Clear();

	virtual void PolygonsRingsColour( const wxColour &colour ) { m_pen->SetColour(colour); }
	virtual wxColour PolygonsRingsColour() { return m_pen->GetColour(); }
	virtual void PolygonsInsideColour( const wxColour &colour ) { m_brush->SetColour(colour); }
	virtual wxColour PolygonsInsideColour() { return m_brush->GetColour(); }
	virtual void PolygonsRingsStyle( const unsigned int style ) { m_pen->SetStyle(style); }

	virtual unsigned int PolygonsRingsStyle() { return m_pen->GetStyle(); }
	virtual void PolygonsInsideStyle( const unsigned int style ) { m_brush->SetStyle(style); }
	virtual unsigned int PolygonsInsideStyle() { return m_brush->GetStyle(); }
	virtual void PolygonsRingsWidth( const unsigned int width ) {m_pen->SetWidth(width); }
	virtual unsigned int PolygonsRingsWidth() { return m_pen->GetWidth(); }

private:
	wxPen *m_pen;
	wxBrush *m_brush;
	std::vector<wxBitmap> m_bitmap;
	std::vector<CircleType> m_circles;
	std::vector<EllipseType> m_ellipses;
	std::vector<RotatedEllipseType> m_rotatedellipses;
    std::vector< simpleArcType > m_arcs;
    std::vector< simplePointType > m_points;
    std::vector<std::vector< simplePointType > > m_splines;
	std::vector< std::vector< simplePointType > > m_polygons;
};

#endif /* VECTORLAYERMULTIGEOMETRIES_HPP_ */
