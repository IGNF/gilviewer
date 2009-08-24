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

#ifndef VECTORLAYERMULTIGEOMETRIES_HPP_
#define VECTORLAYERMULTIGEOMETRIES_HPP_

class wxPen;
class wxBrush;

#include <vector>

#include "layers/VectorLayerContent.hpp"

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

	virtual void Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent, const float zoomFactor, const float translationX, const float translationY);
	virtual void Save( const std::string &name ) {};

	void AddCircle( double x , double y , double radius );
	void AddLine( double x1 , double y1 , double x2 , double y2 );
	void AddPoint( double x , double y );
	void AddSpline( std::vector<simplePointType> points );
	void AddEllipse(double x_center, double y_center, double a, double b);
	void AddEllipse(double x_center, double y_center, double a, double b, double theta);

    void SetBrushVectorLayerMultiGeometries( const wxBrush &brush );
    void SetBrushVectorLayerMultiGeometries( unsigned char red , unsigned char green , unsigned char blue , int style = wxSOLID , unsigned char alpha=wxALPHA_OPAQUE );
    void SetPenVectorLayerMultiGeometries( const wxPen &pen );
    void SetPenVectorLayerMultiGeometries( unsigned char red , unsigned char green , unsigned char blue , int style = wxSOLID , int width = 1 , unsigned char alpha=wxALPHA_OPAQUE );

	virtual void Clear();

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
};

#endif /* VECTORLAYERMULTIGEOMETRIES_HPP_ */
