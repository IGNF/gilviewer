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


#ifndef __VECTOR_LAYER_POLYGON_HPP__
#define __VECTOR_LAYER_POLYGON_HPP__

#include "VectorLayerContent.hpp"


class GenericVectorLayerPolygon : public VectorLayerContent
{
public:
	GenericVectorLayerPolygon();
	virtual ~GenericVectorLayerPolygon() {};

	typedef std::pair<double,double> simplePointType;
	typedef std::vector< simplePointType > simplePolygonRingType;
	typedef std::vector< wxPoint > simplewxPolygonRingType;

	virtual void Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent, const float zoomFactor, const float translationX, const float translationY);

	virtual void PolygonsRingsColour( const wxColour &colour ) { m_ringsColour = colour; }
	virtual wxColour PolygonsRingsColour() { return m_ringsColour; }
	virtual void PolygonsInsideColour( const wxColour &colour ) { m_shapesColour = colour; }
	virtual wxColour PolygonsInsideColour() { return m_shapesColour; }
	virtual void PolygonsRingsStyle( const unsigned int style ) { m_penStyle = style; }
	virtual unsigned int PolygonsRingsStyle() { return m_penStyle; }
	virtual void PolygonsInsideStyle( const unsigned int style ) { m_brushStyle = style; }
	virtual unsigned int PolygonsInsideStyle() { return m_brushStyle; }
	virtual void PolygonsRingsWidth( const unsigned int width ) { m_ringsWidth = width; }
	virtual unsigned int PolygonsRingsWidth() { return m_ringsWidth; }

	virtual void Clear();
	virtual void Save( const std::string &name ) {}

protected:
	std::vector< simplePolygonRingType > m_polygons;
	std::vector< simplewxPolygonRingType > m_wxpolygons;

	wxColour m_ringsColour;
	wxColour m_shapesColour;
	unsigned int m_ringsWidth;
	int m_penStyle;
	int m_brushStyle;
};


class VectorLayerPolygon : public GenericVectorLayerPolygon
{
public:
	VectorLayerPolygon() : GenericVectorLayerPolygon() { m_shapeType = SHPT_POLYGON; }
	VectorLayerPolygon(const SHPHandle &handle , const std::string &shapefileFileName);
	~VectorLayerPolygon() {};

    virtual void AddPolygon( const std::vector<double> &x , const std::vector<double> &y );

	virtual void Save( const std::string &name );

private:
};


class VectorLayerPolygonM : public GenericVectorLayerPolygon
{
public:
	VectorLayerPolygonM() : GenericVectorLayerPolygon() { m_shapeType = SHPT_POLYGONM; }
	VectorLayerPolygonM(const SHPHandle &handle , const std::string &shapefileFileName);

	virtual void Save( const std::string &name ) {}

private:
	std::vector< double > m_measures;
};


class VectorLayerPolygonZ : public GenericVectorLayerPolygon
{
public:
	VectorLayerPolygonZ() : GenericVectorLayerPolygon() { m_shapeType = SHPT_POLYGONZ; }
	VectorLayerPolygonZ(const SHPHandle &handle , const std::string &shapefileFileName);
	~VectorLayerPolygonZ() {};

	virtual void Save( const std::string &name ) {}

private:
	std::vector< double > m_Z;
};

#endif // __VECTOR_LAYER_POLYGON_HPP__
