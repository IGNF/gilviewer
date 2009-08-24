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
