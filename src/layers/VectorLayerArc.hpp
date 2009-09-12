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

#ifndef __VECTOR_LAYER_ARC_HPP__
#define __VECTOR_LAYER_ARC_HPP__

#include "layers/VectorLayerContent.hpp"

class wxDC;

class GenericVectorLayerArc : public VectorLayerContent
{
public:
    GenericVectorLayerArc();
    virtual ~GenericVectorLayerArc() {};

    typedef std::pair<double,double> simplePointType;
    typedef std::vector< simplePointType > simpleArcType;

    virtual void Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent, const float zoomFactor, const float translationX, const float translationY);

    virtual bool ProcessShapefile( const SHPHandle &shapefileHandle ) { return true; };

    virtual void LinesColour( const wxColour &colour ) { m_arcsColour = colour; }
	virtual wxColour LinesColour() { return m_arcsColour; }
    virtual void LinesWidth( const unsigned int width ) { m_width = width; }
	virtual unsigned int LinesWidth() { return m_width; }
    virtual void LinesPenStyle( const int style ) { m_penStyle = style; }
	virtual int LinesPenStyle() { return m_penStyle; }

	virtual void Clear();
	virtual void Save( const std::string &name ) {}

protected:
    std::vector< simpleArcType > m_arcs;

    wxColour m_arcsColour;
    unsigned int m_width;
    int m_penStyle;
};


class VectorLayerArc : public GenericVectorLayerArc
{
public:
    VectorLayerArc() : GenericVectorLayerArc() { m_shapeType = SHPT_ARC; }
    VectorLayerArc(const SHPHandle &handle , const std::string &shapefileFileName);
    ~VectorLayerArc() {};

    virtual void AddLine( double x1 , double y1 , double x2 , double y2 );
    virtual void AddPolyline( const std::vector<double> &x , const std::vector<double> &y );

	virtual void Save( const std::string &name );
};


class VectorLayerArcM : public GenericVectorLayerArc
{
public:
    VectorLayerArcM() : GenericVectorLayerArc()  { m_shapeType = SHPT_ARCM; }
    VectorLayerArcM(const SHPHandle &handle , const std::string &shapefileFileName);
    ~VectorLayerArcM() {};

	virtual void Save( const std::string &name ) {}

private:
    std::vector< double > m_measures;
};


class VectorLayerArcZ : public GenericVectorLayerArc
{
public:
    VectorLayerArcZ() : GenericVectorLayerArc()  { m_shapeType = SHPT_ARCZ; }
    VectorLayerArcZ(const SHPHandle &handle , const std::string &shapefileFileName);
    ~VectorLayerArcZ() {};

	virtual void Save( const std::string &name ) {}

private:
    std::vector< double > m_Z;
};

#endif // __VECTOR_LAYER_ARC_HPP__
