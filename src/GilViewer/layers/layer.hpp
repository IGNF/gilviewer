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

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <vector>

#include <wx/gdicmn.h>
#include <wx/colour.h>
#include <wx/font.h>
#include <wx/colour.h>

#include "layer_transform.hpp"

class wxDC;
#ifdef WIN32
#	include <wx/msw/winundef.h>
#endif

class orientation_2d;
class color_lookup_table;
class layer_settings_control;
class layer_control;

class layer
{
public:
    typedef boost::shared_ptr< layer > ptrLayerType;
    typedef std::vector<std::vector<double> > histogram_type;

    layer(const boost::function<void()> &notifyLayerControl = notify_none, const boost::function<void()> &notifyLayerSettingsControl = notify_none);
    static void notify_none() {}

    virtual ~layer() {}

    /// Set the callback to notify the LayerControl from changes
    void notify_layer_control( const boost::function<void()> &notifier ) { notifyLayerControl_ = notifier; }
    /// Set the callback to notify the LayerSettingsControl from changes
    void notify_layer_settings_control( const boost::function<void()> &notifier ) { notifyLayerSettingsControl_ = notifier; }

    virtual void update(int width, int height)=0;
    virtual void draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent) const =0;

    virtual bool needs_update() const {return m_hasToBeUpdated;}
    virtual void needs_update(bool update) {m_hasToBeUpdated=update;}

    virtual bool visible() const { return m_isVisible;}
    virtual void visible(bool visible) { m_isVisible=visible; notifyLayerControl_(); }

    virtual bool transformable() const {return m_isTransformable;}
    virtual void transformable(bool transf) { m_isTransformable=transf; notifyLayerControl_(); }

    virtual bool saveable() const {return false;}

    virtual std::string name() const {return m_name;}
    virtual void name(const std::string &name) {m_name=name;}
    virtual std::string filename() const {return m_filename;}
    virtual void filename(const std::string &filename) {m_filename=filename;}

    virtual void infos(const std::string &infos) {m_infos=infos;}
    virtual std::string infos() {return m_infos;}
    virtual std::string layer_type_as_string() const {return "unknown type";}
    virtual void save(const std::string &name) const {}

    virtual ptrLayerType crop(int& x0, int& y0, int& x1, int& y1) const { return ptrLayerType(); }

    virtual layer_settings_control* build_layer_settings_control(unsigned int index, layer_control* parent);

    inline virtual double center_x() {return 0.;}
    inline virtual double center_y() {return 0.;}



    virtual void has_ori(bool hasOri) { m_hasOri = hasOri; }
    virtual bool has_ori() const { return m_hasOri; }
    void add_orientation( const std::string &image_filename );
    virtual void orientation(const boost::shared_ptr<orientation_2d> &orientation)  {}
    virtual const boost::shared_ptr<orientation_2d> &orientation() const;

    // Methodes specifiques ImageLayer
    virtual void alpha(unsigned char alpha) {}
    virtual inline unsigned char alpha() const { return 0; }
    virtual void intensity_min(double intensity) {}
    virtual double intensity_min() const { return 0.; }
    virtual void intensity_max(double intensity) {}
    virtual double intensity_max() const { return 0.; }
    virtual void gamma(double gamma) {}
    virtual double gamma() const { return 0.; }
    virtual void transparency_min(double t) {}
    virtual double transparency_min() const { return 0.; }
    virtual void transparency_max(double t) {}
    virtual double transparency_max() const { return 0.; }
    virtual void transparent(bool t) {}
    virtual bool transparent() const { return false; }
    virtual void channels(unsigned int red, unsigned int green, unsigned int blue) {}
    virtual void channels(unsigned int &red, unsigned int &green, unsigned int &blue) const {}
    virtual void alpha_channel(bool useAlphaChannel, unsigned int alphaChannel) {}
    virtual void alpha_channel(bool &useAlphaChannel, unsigned int &alphaChannel) const {}
    virtual boost::shared_ptr<color_lookup_table> colorlookuptable();

    virtual unsigned int nb_components() const { return 0; }
    virtual boost::shared_ptr<const histogram_type> histogram(double &min, double &max) const { return boost::shared_ptr<const histogram_type>(); }
    virtual std::string pixel_value(int i, int j) const { return std::string(); }
    // Fin methodes specifiques ImageLayer

    virtual std::vector<std::string> available_formats_extensions() const { return std::vector<std::string>(); }
    virtual std::string available_formats_wildcard() const { return std::string(); }

    virtual void default_display_parameters() {}

    // Methodes specifiques VectorLayer
    virtual void add_vector_layer_content( const std::string &shapefileFileName ) {}

    enum eSNAP //snap modes (may be bitwise-combined using the '&' operator)
    {
        SNAP_NONE = 0,
        SNAP_POINT = 1,
        SNAP_LINE = 2,
        SNAP_INTERSECTION = 4,
    };

    virtual void snap(  eSNAP snap, double x , double y, double& dsnap, double& xsnap, double& ysnap ) const {}
    virtual void add_point( double x , double y ) {}
    virtual void add_text( double x , double y , const std::string &text , const wxColour &color = *wxBLACK ) {}
    virtual void add_line( double x1 , double y1 , double x2 , double y2 ) {}
    virtual void add_polyline( const std::vector<double> &x , const std::vector<double> &y ) {}
    virtual void add_polygon( const std::vector<double> &x , const std::vector<double> &y ) {}
    virtual void add_circle( double x , double y , double radius ) {}
    virtual void add_spline( std::vector<std::pair<double, double> > points ) {}
    virtual void add_ellipse(double x_center, double y_center, double a, double b) {}
    virtual void add_ellipse(double x_center, double y_center, double a, double b, double theta) {}

    void point_color(const wxColor& c, bool update=true)          {m_point_color=c;          if(update) notifyLayerSettingsControl_();}
    void point_width(unsigned int w, bool update=true)            {m_point_width=w;          if(update) notifyLayerSettingsControl_();}
    void line_color(const wxColor& c, bool update=true)           {m_line_color=c;           if(update) notifyLayerSettingsControl_();}
    void line_width(unsigned int w, bool update=true)             {m_line_width=w;           if(update) notifyLayerSettingsControl_();}
    void line_style(unsigned int s, bool update=true)             {m_line_style=s;           if(update) notifyLayerSettingsControl_();}
    void polygon_border_color(const wxColor& c, bool update=true) {m_polygon_border_color=c; if(update) notifyLayerSettingsControl_();}
    void polygon_inner_color(const wxColor& c, bool update=true)  {m_polygon_inner_color=c;  if(update) notifyLayerSettingsControl_();}
    void polygon_border_width(unsigned int w, bool update=true)   {m_polygon_border_width=w; if(update) notifyLayerSettingsControl_();}
    void polygon_border_style(unsigned int s, bool update=true)   {m_polygon_border_style=s; if(update) notifyLayerSettingsControl_();}
    void polygon_inner_style(unsigned int s, bool update=true)    {m_polygon_inner_style=s;  if(update) notifyLayerSettingsControl_();}
    void text_color(const wxColor& c, bool update=true)           {m_text_color=c;           if(update) notifyLayerSettingsControl_();}
    wxColor point_color() const                                   {return m_point_color;}
    unsigned int point_width() const                              {return m_point_width;}
    wxColor line_color() const                                    {return m_line_color;}
    unsigned int line_width() const                               {return m_line_width;}
    unsigned int line_style() const                               {return m_line_style;}
    wxColor polygon_border_color() const                          {return m_polygon_border_color;}
    wxColor polygon_inner_color() const                           {return m_polygon_inner_color;}
    unsigned int polygon_border_width() const                     {return m_polygon_border_width;}
    unsigned int polygon_border_style() const                     {return m_polygon_border_style;}
    unsigned int polygon_inner_style() const                      {return m_polygon_inner_style;}
    wxColor text_color() const                                    {return m_text_color;}

    virtual void text_visibility( bool value , bool update = true ) {}
    virtual bool text_visibility() const { return true; } // ???!!!???

    virtual void clear() {}
    // Methodes specifiques VectorLayer

    /// Callback to notify the LayerControl from changes
    boost::function<void ()> notifyLayerControl_;
    /// Callback to notify the LayerSettingsControl from changes
    boost::function<void ()> notifyLayerSettingsControl_;


    layer_transform& transform() { return m_layer_transform; }
    const layer_transform& transform() const { return m_layer_transform; }

protected:
    bool m_isVisible;
    bool m_isTransformable;
    bool m_hasToBeUpdated;
    // Le nom du layer
    std::string m_name;
    // Le nom du fichier associe si il existe
    std::string m_filename;

    //infos du layer
    std::string m_infos;

    // transformation du layer
    layer_transform m_layer_transform;

    //orientation (possible que pour les calques images)
    boost::shared_ptr<orientation_2d> m_ori;
    bool m_hasOri;

    unsigned int m_point_width, m_line_width, m_line_style, m_polygon_border_width, m_polygon_border_style, m_polygon_inner_style;
    wxColor m_point_color, m_line_color, m_polygon_border_color, m_polygon_inner_color, m_text_color;
};


#endif // __LAYER_HPP__
