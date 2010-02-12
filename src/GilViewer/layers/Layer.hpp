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
#include "GilViewer/gui/LayerSettingsControl.hpp"

#ifdef WIN32
#	include <wx/msw/winundef.h>
#endif

class LayerControl;

class Layer
{
public:
    typedef boost::shared_ptr< Layer > ptrLayerType;

    Layer(const boost::function<void()> &notifyLayerControl = foo, const boost::function<void()> &notifyLayerSettingsControl = bar):
            notifyLayerControl_(notifyLayerControl),
            notifyLayerSettingsControl_(notifyLayerSettingsControl),
            m_isVisible(true),
            m_hasToBeUpdated(true),
            m_name("Default layer name"),
            m_filename(""),
            m_hasOri(false) {}
    static void foo() {}
    static void bar() {}

    /// Set the callback to notify the LayerControl from changes
    void SetNotifyLayerControl( const boost::function<void()> &notifier ) { notifyLayerControl_ = notifier; }
    /// Set the callback to notify the LayerSettingsControl from changes
    void SetNotifyLayerSettingsControl( const boost::function<void()> &notifier ) { notifyLayerSettingsControl_ = notifier; }

    virtual void Update(int width, int height)=0;
    virtual void Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent) const =0;

    virtual bool HasToBeUpdated() const {return m_hasToBeUpdated;}
    virtual void HasToBeUpdated(bool update) {m_hasToBeUpdated=update;}

    virtual bool IsVisible() const { return m_isVisible;}
    virtual void IsVisible(bool visible) { m_isVisible=visible; notifyLayerControl_(); }

    virtual bool IsTransformable() const {return m_isTransformable;}
    virtual void IsTransformable(bool transf) { m_isTransformable=transf; notifyLayerControl_(); }

    virtual bool is_saveable() const {return false;}

    virtual std::string Name() const {return m_name;}
    virtual void Name(const std::string &name) {m_name=name;}
    virtual std::string Filename() const {return m_filename;}
    virtual void Filename(const std::string &filename) {m_filename=filename;}

    std::string GetInfos() const {return m_infos;}
    virtual std::string get_layer_type_as_string() const {return "unknow type";}
    virtual void Save(const std::string &name) {}

    virtual ptrLayerType crop(int& x0, int& y0, int& x1, int& y1) const { return ptrLayerType(); }

    virtual LayerSettingsControl* build_layer_settings_control(unsigned int index, LayerControl* parent) {return new LayerSettingsControl(parent);}

    virtual void ZoomFactor(double zoomFactor) { m_zoomFactor = zoomFactor; }
    virtual inline double ZoomFactor() const { return m_zoomFactor; }
    virtual void TranslationX(double dx) { m_translationX = dx; }
    virtual inline double TranslationX() const { return m_translationX; }
    virtual void TranslationY(double dy) { m_translationY = dy; }
    virtual inline double TranslationY() const { return m_translationY; }

    // local<->global transforms. Default: pixel-centered
    wxPoint FromLocal(const wxPoint &p, double delta=0.5) const
    {
	return wxPoint(
		wxCoord((p.x +m_translationX+delta)/m_zoomFactor),
		wxCoord((p.y +m_translationY+delta)/m_zoomFactor));
    }

    wxPoint ToLocal(const wxPoint &p, double delta=0.5) const
    {
	return wxPoint(
		wxCoord(m_zoomFactor*p.x -m_translationX+0.5-delta),
		wxCoord(m_zoomFactor*p.y -m_translationY+0.5-delta));
    }


    virtual void Resolution(double r) { m_resolution = r; }
    virtual inline double Resolution() const { return m_resolution; }

    virtual void HasOri(bool hasOri) { m_hasOri = hasOri; }
    virtual bool HasOri() const { return m_hasOri; }
    virtual void Orientation(const Orientation2D &orientation)  {}
    virtual const Orientation2D &Orientation() const  { return m_ori; }

    // Methodes specifiques ImageLayer
    virtual void Alpha(unsigned char alpha) {}
    virtual inline unsigned char Alpha() const { return 0; }
    virtual void IntensityMin(double intensity) {}
    virtual double IntensityMin() const { return 0.; }
    virtual void IntensityMax(double intensity) {}
    virtual double IntensityMax() const { return 0.; }
    virtual void Gamma(double gamma) {}
    virtual double Gamma() const { return 0.; }
    virtual void TransparencyMin(double t) {}
    virtual double TransparencyMin() const { return 0.; }
    virtual void TransparencyMax(double t) {}
    virtual double TransparencyMax() const { return 0.; }
    virtual void IsTransparent(bool t) {}
    virtual bool IsTransparent() const { return false; }
    virtual void SetChannels(unsigned int red, unsigned int green, unsigned int blue) {}
    virtual void GetChannels(unsigned int &red, unsigned int &green, unsigned int &blue) const {}
    virtual void SetAlphaChannel(bool useAlphaChannel, unsigned int alphaChannel) {}
    virtual void GetAlphaChannel(bool &useAlphaChannel, unsigned int &alphaChannel) const {}
    virtual boost::shared_ptr<ColorLookupTable> GetColorLookupTable() { return boost::shared_ptr<ColorLookupTable>(); }

    virtual unsigned int GetNbComponents() const { return 0; }
    virtual void Histogram(std::vector< std::vector<double> > &histo, double &min, double &max) const {}
    virtual std::string GetPixelValue(int i, int j) const { return std::string(); }
    // Fin methodes specifiques ImageLayer

    virtual std::vector<std::string> get_available_formats_extensions() const { return std::vector<std::string>(); }
    virtual std::string get_available_formats_wildcard() const { return std::string(); }

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


    virtual void set_inner_color( const wxColour &colour , bool update = true ) {}
    virtual void set_border_color( const wxColour &colour , bool update = true ) {}
    virtual void set_width( unsigned int width , bool update = true ) {}
    virtual void set_style( const wxColour &inner_color, const wxColour &border_color, unsigned int inner_style, unsigned int border_style, unsigned int width , bool update = true ) {}
    virtual wxColour get_inner_color() const {return wxNullColour;}
    virtual wxColour get_border_color() const {return wxNullColour;}
    virtual unsigned int get_width() const {return 3;}
    virtual unsigned int get_inner_style() const {return wxSOLID;}
    virtual unsigned int get_border_style() const {return wxSOLID;}

    virtual void TextsColour( const wxColour &colour , bool update = true ) {}
    virtual void TextsFont( const wxFont &font , bool update = true ) {}
    virtual const wxFont& TextsFont() const { return wxNullFont; }
    virtual void TextsVisibility( bool value , bool update = true ) {}
    virtual bool TextsVisibility() const { return true; } // ???!!!???

    virtual void Clear() {}
    // Methodes specifiques VectorLayer

    /// Callback to notify the LayerControl from changes
    boost::function<void ()> notifyLayerControl_;
    /// Callback to notify the LayerSettingsControl from changes
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
