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
#include <wx/colour.h>

class wxDC;

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <sstream>

#include "../tools/Orientation2D.h"
#include "../tools/ColorLookupTable.h"
#include "../gui/LayerSettingsControl.hpp"
#include "../tools/error_logger.hpp"

#ifdef WIN32
#	include <wx/msw/winundef.h>
#endif

class LayerControl;

class Layer
{
public:
    typedef boost::shared_ptr< Layer > ptrLayerType;

    Layer(const boost::function<void()> &notifyLayerControl = notify_none, const boost::function<void()> &notifyLayerSettingsControl = notify_none):
            notifyLayerControl_(notifyLayerControl),
            notifyLayerSettingsControl_(notifyLayerSettingsControl),
            m_isVisible(true),
            m_isTransformable(true),
            m_hasToBeUpdated(true),
            m_name("Default layer name"),
            m_filename(""),
            m_zoomFactor(1.),
            m_translationX(0.), m_translationY(0.),
            m_hasOri(false),
            m_infos(""),
            m_point_width(3), m_line_width(3),
            m_line_style(wxSOLID),
            m_polygon_border_width(3),
            m_polygon_border_style(wxSOLID), m_polygon_inner_style(wxSOLID),
            m_point_color(*wxRED), m_line_color(*wxBLUE), m_polygon_border_color(*wxLIGHT_GREY), m_polygon_inner_color(*wxGREEN), m_text_color(*wxRED) {}
    static void notify_none() {}

    virtual ~Layer() {}
    
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

    virtual std::string GetInfos() {return m_infos;}
    virtual std::string get_layer_type_as_string() const {return "unknow type";}
    virtual void Save(const std::string &name) const {}

    virtual ptrLayerType crop(int& x0, int& y0, int& x1, int& y1) const { return ptrLayerType(); }

    virtual LayerSettingsControl* build_layer_settings_control(unsigned int index, LayerControl* parent) {return new LayerSettingsControl(parent);}

    virtual void ZoomFactor(double zoomFactor) { m_zoomFactor = zoomFactor; }
    virtual inline double ZoomFactor() const { return m_zoomFactor; }
    virtual void TranslationX(double dx) { m_translationX = dx; }
    virtual inline double TranslationX() const { return m_translationX; }
    virtual void TranslationY(double dy) { m_translationY = dy; }
    virtual inline double TranslationY() const { return m_translationY; }

    inline virtual double get_center_x() {return 0.;}
    inline virtual double get_center_y() {return 0.;}

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
    void add_orientation( const std::string &image_filename )
    {
        try
        {
            Orientation2D ori;
            ori.ReadOriFromImageFile(image_filename);
            this->Orientation(ori);
            this->HasOri(true);
        }
        catch (const std::exception &e)
        {
            std::ostringstream oss;
            oss << "No orientation for image " << image_filename << "\n" << e.what();
            error_logger::log_wx_log_message(oss.str());
            this->HasOri(false);
        }
    }

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

    void set_point_color(const wxColor& c, bool update=true)          {m_point_color=c;          if(update) notifyLayerSettingsControl_();}
    void set_point_width(unsigned int w, bool update=true)            {m_point_width=w;          if(update) notifyLayerSettingsControl_();}
    void set_line_color(const wxColor& c, bool update=true)           {m_line_color=c;           if(update) notifyLayerSettingsControl_();}
    void set_line_width(unsigned int w, bool update=true)             {m_line_width=w;           if(update) notifyLayerSettingsControl_();}
    void set_line_style(unsigned int s, bool update=true)             {m_line_style=s;           if(update) notifyLayerSettingsControl_();}
    void set_polygon_border_color(const wxColor& c, bool update=true) {m_polygon_border_color=c; if(update) notifyLayerSettingsControl_();}
    void set_polygon_inner_color(const wxColor& c, bool update=true)  {m_polygon_inner_color=c;  if(update) notifyLayerSettingsControl_();}
    void set_polygon_border_width(unsigned int w, bool update=true)   {m_polygon_border_width=w; if(update) notifyLayerSettingsControl_();}
    void set_polygon_border_style(unsigned int s, bool update=true)   {m_polygon_border_style=s; if(update) notifyLayerSettingsControl_();}
    void set_polygon_inner_style(unsigned int s, bool update=true)    {m_polygon_inner_style=s;  if(update) notifyLayerSettingsControl_();}
    void set_text_color(const wxColor& c, bool update=true)           {m_text_color=c;           if(update) notifyLayerSettingsControl_();}
    wxColor get_point_color() const                                   {return m_point_color;}
    unsigned int get_point_width() const                              {return m_point_width;}
    wxColor get_line_color() const                                    {return m_line_color;}
    unsigned int get_line_width() const                               {return m_line_width;}
    unsigned int get_line_style() const                               {return m_line_style;}
    wxColor get_polygon_border_color() const                          {return m_polygon_border_color;}
    wxColor get_polygon_inner_color() const                           {return m_polygon_inner_color;}
    unsigned int get_polygon_border_width() const                     {return m_polygon_border_width;}
    unsigned int get_polygon_border_style() const                     {return m_polygon_border_style;}
    unsigned int get_polygon_inner_style() const                      {return m_polygon_inner_style;}
    wxColor get_text_color() const                                    {return m_text_color;}

    virtual void text_visibility( bool value , bool update = true ) {}
    virtual bool text_visibility() const { return true; } // ???!!!???

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

    unsigned int m_point_width, m_line_width, m_line_style, m_polygon_border_width, m_polygon_border_style, m_polygon_inner_style;
    wxColor m_point_color, m_line_color, m_polygon_border_color, m_polygon_inner_color, m_text_color;
};


#endif // __LAYER_HPP__
