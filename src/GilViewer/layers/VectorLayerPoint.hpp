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

#ifndef __VECTOR_LAYER_POINT_HPP__
#define __VECTOR_LAYER_POINT_HPP__

#include "VectorLayerContent.hpp"

class GenericVectorLayerPoint : public VectorLayerContent
{
public:
    GenericVectorLayerPoint();
    virtual ~GenericVectorLayerPoint() {};

    typedef std::pair<double,double> simplePointType;

    virtual void Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent, const double zoomFactor, const double translationX, const double translationY, const double resolution) const;

    virtual void AddPoint( double x , double y ) {;}
    virtual void AddPoints( const std::vector<double> &x , const std::vector<double> &y ) {;}

	virtual void Clear() { m_points.clear(); m_numberOfEntities = 0; }
	virtual void Save( const std::string &name ) {}

protected:
    std::vector< simplePointType > m_points;
};

class VectorLayerPoint : public GenericVectorLayerPoint
{
public:
	VectorLayerPoint() : GenericVectorLayerPoint() { m_shapeType = SHPT_POINT; }
    VectorLayerPoint(const SHPHandle &handle , const std::string &shapefileFileName);
    ~VectorLayerPoint() {};

    virtual void AddPoint( double x , double y );
    virtual void AddPoints( const std::vector<double> &x , const std::vector<double> &y );

	virtual void Save( const std::string &name );

private:
};

class VectorLayerPointM : public GenericVectorLayerPoint
{
public:
	VectorLayerPointM() : GenericVectorLayerPoint() { m_shapeType = SHPT_POINTM; }
    VectorLayerPointM(const SHPHandle &handle , const std::string &shapefileFileName);
    ~VectorLayerPointM() {};

    virtual void Clear() { m_points.clear(); m_measures.clear(); }

	virtual void Save( const std::string &name ) {}

private:
    std::vector< double > m_measures;
};

class VectorLayerPointZ : public GenericVectorLayerPoint
{
public:
	VectorLayerPointZ() : GenericVectorLayerPoint() { m_shapeType = SHPT_POINTZ; }
    VectorLayerPointZ(const SHPHandle &handle , const std::string &shapefileFileName);
    ~VectorLayerPointZ() {};

    virtual void Clear() { m_points.clear(); m_Z.clear(); }

	virtual void Save( const std::string &name ) {}

private:
    std::vector< double > m_Z;
};

#endif // __VECTOR_LAYER_POINT_HPP__
