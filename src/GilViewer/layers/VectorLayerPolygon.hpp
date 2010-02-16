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

        virtual void Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent, double zoomFactor, double translationX, double translationY, double resolution) const;

	virtual void Clear();
	virtual void Save( const std::string &name ) {}

protected:
        std::vector< simplePolygonRingType > m_polygons;
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
