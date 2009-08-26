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

#ifndef __VECTOR_LAYER_POINT_HPP__
#define __VECTOR_LAYER_POINT_HPP__

#include "VectorLayerContent.hpp"

class GenericVectorLayerPoint : public VectorLayerContent
{
public:
    GenericVectorLayerPoint();
    virtual ~GenericVectorLayerPoint() {};

    typedef std::pair<double,double> simplePointType;

    virtual void Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent, const float zoomFactor, const float translationX, const float translationY);

    virtual void AddPoint( double x , double y ) {;}
    virtual void AddPoints( const std::vector<double> &x , const std::vector<double> &y ) {;}

    virtual void PointsColour( const wxColour &colour ) { m_pointsColour = colour; }
	virtual wxColour PointsColour() { return m_pointsColour; }
    virtual void PointsWidth( const unsigned int width ) { m_width = width; }
	virtual unsigned int PointsWidth() { return m_width; }

	virtual void Clear() { m_points.clear(); m_numberOfEntities = 0; }
	virtual void Save( const std::string &name ) {}

protected:
    std::vector< simplePointType > m_points;

    wxColour m_pointsColour;
    unsigned int m_width;
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
