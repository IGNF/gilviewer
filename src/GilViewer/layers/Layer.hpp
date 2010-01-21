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

#ifndef __LAYER_HPP__
#define __LAYER_HPP__

#include <wx/gdicmn.h>
#include <wx/colour.h>
#include <wx/font.h>

class wxDC;

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>


#include "GilViewer/tools/Orientation2D.h"
#include "GilViewer/tools/ColorLookupTable.h"

#ifdef WIN32
#	include <wx/msw/winundef.h>
#endif


class Layer
{
public:
	typedef boost::shared_ptr< Layer > ptrLayerType;

	static void foo() {}
	static void bar() {}
	Layer(const boost::function<void()> &notifyLayerControl = foo, const boost::function<void()> &notifyLayerSettingsControl = bar) : notifyLayerControl_(notifyLayerControl), notifyLayerSettingsControl_(notifyLayerSettingsControl), m_isVisible(true), m_hasToBeUpdated(true), m_name("Default layer name"), m_filename(""), m_hasOri(false) {}
	virtual ~Layer() {}

	virtual void Update(const int width, const int height)=0;
	virtual void Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent)=0;

	virtual bool HasToBeUpdated() const {return m_hasToBeUpdated;}
	virtual void HasToBeUpdated(const bool update) {m_hasToBeUpdated=update;}

	virtual bool IsVisible() const { return m_isVisible;}
	virtual void IsVisible(const bool visible) { m_isVisible=visible; notifyLayerControl_(); }

	virtual bool IsTransformable() const {return m_isTransformable;}
	virtual void IsTransformable(const bool transf) { m_isTransformable=transf; notifyLayerControl_(); }

	virtual std::string Name() const {return m_name;}
	virtual void Name(const std::string &name) {m_name=name;}
	virtual std::string Filename() const {return m_filename;}
	virtual void Filename(const std::string &filename) {m_filename=filename;}

	virtual std::string GetInfos() const {return m_infos;}
	virtual void Save(const std::string &name) {}

	virtual ptrLayerType crop( int x0 , int y0 , int width , int height , std::string filename = "" ) = 0;

	void SetNotifyLayerControl( const boost::function<void()> &notifier ) { notifyLayerControl_ = notifier; }
	void SetNotifyLayerSettingsControl( const boost::function<void()> &notifier ) { notifyLayerSettingsControl_ = notifier; }

    virtual void ZoomFactor(const double zoomFactor) { m_zoomFactor = zoomFactor; }
    virtual inline double ZoomFactor() const { return m_zoomFactor; }
    virtual void TranslationX(const double dx) { m_translationX = dx; }
    virtual inline double TranslationX() const { return m_translationX; }
    virtual void TranslationY(const double dy) { m_translationY = dy; }
    virtual inline double TranslationY() const { return m_translationY; }

    virtual void Resolution(const double r) { m_resolution = r; }
    virtual inline double Resolution() const { return m_resolution; }

	virtual void HasOri( const bool hasOri ) { m_hasOri = hasOri; }
	virtual bool HasOri() { return m_hasOri; }
	virtual void Orientation(const Orientation2D &orientation)  {}
	virtual const Orientation2D &Orientation() const  { return m_ori; }

	// Methodes specifiques ImageLayer
	virtual void Alpha(const unsigned char alpha) {}
	virtual inline unsigned char Alpha() const { return 0; }
	virtual void IntensityMin(const double intensity) {}
	virtual double IntensityMin() const { return 0.; }
	virtual void IntensityMax(const double intensity) {}
	virtual double IntensityMax() const { return 0.; }
	virtual void Gamma(const double gamma) {}
	virtual double Gamma() const { return 0.; }
	virtual void TransparencyMin(const double t) {}
	virtual double TransparencyMin() const { return 0.; }
	virtual void TransparencyMax(const double t) {}
	virtual double TransparencyMax() const { return 0.; }
	virtual void IsTransparent(const bool t) {}
	virtual bool IsTransparent() const { return false; }
	virtual void SetChannels(const unsigned int red, const unsigned int green, const unsigned int blue) {}
	virtual void GetChannels(unsigned int &red, unsigned int &green, unsigned int &blue) {}
	virtual void SetAlphaChannel(const bool useAlphaChannel, const unsigned int alphaChannel) {}
	virtual void GetAlphaChannel(bool &useAlphaChannel, unsigned int &alphaChannel) {}
	virtual boost::shared_ptr<ColorLookupTable> GetColorLookupTable() { return boost::shared_ptr<ColorLookupTable>(); }


	virtual unsigned int GetNbComponents() const { return 0; }
    virtual void Histogram(std::vector< std::vector<double> > &histo, double &min, double &max) const {}
    virtual std::string GetPixelValue(const int i, const int j) const { return std::string(); }
	// Fin methodes specifiques ImageLayer

	virtual void SetDefaultDisplayParameters() {}

	// Methodes specifiques VectorLayer
    virtual void AddVectorLayerContent( const std::string &shapefileFileName ) {}

    virtual void AddPoint( double x , double y ) {}
    virtual void AddText( double x , double y , const std::string &text , const wxColour &color = *wxBLACK ) {}
    virtual void AddLine( double x1 , double y1 , double x2 , double y2 ) {}
    virtual void AddPolyline( const std::vector<double> &x , const std::vector<double> &y ) {}
    virtual void AddPolygon( const std::vector<double> &x , const std::vector<double> &y ) {}
    virtual void AddCircle( double x , double y , double radius ) {}
    virtual void AddSpline( std::vector<std::pair<double, double> > points ) {}
    virtual void AddEllipse(double x_center, double y_center, double a, double b) {}
    virtual void AddEllipse(double x_center, double y_center, double a, double b, double theta) {}


	virtual void PointsColour( const wxColour &colour , bool update = true ) {}
	virtual wxColour PointsColour() { return wxColour(0,0,0); }
	virtual void PointsWidth( const unsigned int width , bool update = true ) {}
	virtual unsigned int PointsWidth() { return 0; }
	virtual void SetPointsStyle( const wxColour &colour , const unsigned int width , bool update = true ) {}

	virtual void LinesColour( const wxColour &colour , bool update = true ) {}
	virtual wxColour LinesColour() { return wxColour(0,0,0); }
	virtual void LinesWidth( const unsigned int width , bool update = true ) {}
	virtual unsigned int LinesWidth() { return 0; }
	virtual void LinesPenStyle( const int style , bool update = true ) {}
	virtual int LinesPenStyle() { return 0; }
	virtual void SetLinesStyle( const wxColour &colour , const unsigned int width , int style , bool update = true ) {}

	virtual void PolygonsRingsColour( const wxColour &colour , bool update = true) {}
	virtual wxColour PolygonsRingsColour() { return wxColour(0,0,0); }
	virtual void PolygonsInsideColour( const wxColour &colour , bool update = true ) {}
	virtual wxColour PolygonsInsideColour() { return wxColour(0,0,0); }
	virtual void PolygonsRingsStyle( const unsigned int style , bool update = true ) {}
	virtual unsigned int PolygonsRingsStyle() { return 0; }
	virtual void PolygonsInsideStyle( const unsigned int style , bool update = true ) {}
	virtual unsigned int PolygonsInsideStyle() { return 0; }
	virtual void PolygonsRingsWidth( const unsigned int width , bool update = true ) {}
	virtual unsigned int PolygonsRingsWidth() { return 0; }

	virtual void TextsColour( const wxColour &colour , bool update = true ) {}
	virtual void TextsFont( const wxFont &font , bool update = true ) {}
	virtual wxFont TextsFont() { return wxNullFont; }
	virtual void TextsVisibility( bool value , bool update = true ) {}
	virtual bool TextsVisibility() { return true; } // ???!!!???

	virtual void Clear() {}
	// Methodes specifiques VectorLayer

	boost::function<void ()> notifyLayerControl_;
	boost::function<void ()> notifyLayerSettingsControl_;


protected:
	bool m_isVisible;
	bool m_isTransformable;
	bool m_hasToBeUpdated;
	// Le nom du layer
	std::string m_name;
	// Le nom du fichier associe si il existe
	std::string m_filename;

	//gestion de la geometrie
	double m_zoomFactor;
	double m_translationX, m_translationY;
	double m_resolution;

	//orientation (possible que pour les calques images)
	Orientation2D m_ori;
	bool m_hasOri;

	//infos du layer
	std::string m_infos;

	static wxColour m_defaultPointColour;
};


#endif // __LAYER_HPP__
