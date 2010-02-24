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
#ifndef OGR_VECTOR_LAYER_HPP
#define OGR_VECTOR_LAYER_HPP

#include "Layer.hpp"

#include <boost/mpl/vector.hpp>
#include <boost/variant/variant.hpp>

// TODO
#include <iostream>

class LayerSettingsControl;

class OGRGeometry;
class OGRFeature;
class OGRMultiLineString;
class OGRLinearRing;
class OGRLineString;
class OGRMultiPoint;
class OGRMultiPolygon;
class OGRPoint;
class OGRPolygon;
class OGRSpatialReference;
class OGRLayer;

class wxColor;

typedef boost::variant< OGRLinearRing*,
                        OGRLineString*,
                        OGRMultiLineString*,
                        OGRMultiPoint*,
                        OGRMultiPolygon*,
                        OGRPoint*,
                        OGRPolygon* > geometry_types;

class ogr_vector_layer : public Layer
{
public:
    /// Constructeur a partir d'un nom de calque et d'un fichier shapefile
    ogr_vector_layer(const std::string &layer_name, const std::string &filename);
    /// Constructeur vide: pour creer un layer a remplir a la main ...
    ogr_vector_layer(const std::string &layer_name="default name");
    /// @param layerName Le nom du calque
    /// @param shapefileFileName Le chemin vers le fichier shapefile
    virtual ~ogr_vector_layer();

    virtual void Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent) const;
    virtual void Update(int, int) {}

    virtual bool is_saveable() const {return true;}
    virtual std::string get_available_formats_wildcard() const;

    void build_infos(OGRSpatialReference *spatial_reference);

    virtual LayerSettingsControl* build_layer_settings_control(unsigned int index, LayerControl* parent);

    inline int Type() {return m_layertype;}
    inline virtual double get_center_x() {return m_center_x;}
    inline virtual double get_center_y() {return m_center_y;}

    const std::vector<std::pair<geometry_types,OGRFeature*> >& get_geometries_features() const;
    std::vector<std::pair<geometry_types,OGRFeature*> >& get_geometries_features();

    static wxPoint FromLocal(double zoomFactor, double translationX, double translationY, double delta, double x, double y, int coordinates);
    inline void set_coordinates(int c) {m_coordinates=c;}
    virtual std::string get_layer_type_as_string() const {return "Vector";}

    virtual void set_point_color(const wxColor& c, bool update=true);
    virtual wxColor get_point_color() const;
    virtual void set_point_width(unsigned int w, bool update=true) {m_point_width=w; if(update) notifyLayerSettingsControl_();}
    virtual unsigned int get_point_width() const  {return m_point_width;}
    virtual void set_line_color(const wxColor& c, bool update=true);
    virtual wxColor get_line_color();
    virtual void set_line_width(unsigned int w, bool update=true) {m_line_width=w; if(update) notifyLayerSettingsControl_();}
    virtual unsigned int get_line_width() const  {return m_line_width;}
    virtual void set_line_style(unsigned int s, bool update=true)  {m_line_style=s; if(update) notifyLayerSettingsControl_();};
    virtual unsigned int get_line_style() const  {return m_line_style;}
    virtual void set_polygon_border_color(const wxColor& c, bool update=true);
    virtual wxColor get_polygon_border_color();
    virtual void set_polygon_inner_color(const wxColor& c, bool update=true);
    virtual wxColor get_polygon_inner_color();
    virtual void set_polygon_border_width(unsigned int w, bool update=true)  {m_polygon_border_width=w; if(update) notifyLayerSettingsControl_();}
    virtual unsigned int get_polygon_border_width() const  {return m_line_width;}
    virtual void set_polygon_border_style(unsigned int s, bool update=true)  {m_polygon_border_style=s; if(update) notifyLayerSettingsControl_();}
    virtual unsigned int get_polygon_border_style() const {return m_polygon_border_style;}
    virtual void set_polygon_inner_style(unsigned int s, bool update=true) {m_polygon_inner_style=s; if(update) notifyLayerSettingsControl_();}
    virtual unsigned int get_polygon_inner_style() const {return m_polygon_inner_style;}

    virtual void AddPoint( double x , double y );
    virtual void AddText( double x , double y , const std::string &text , const wxColour &color = *wxBLACK );
    virtual void AddLine( double x1 , double y1 , double x2 , double y2 );
    virtual void AddPolyline( const std::vector<double> &x , const std::vector<double> &y );
    virtual void AddPolygon( const std::vector<double> &x , const std::vector<double> &y );
    virtual void AddCircle( double x , double y , double radius );
    virtual void AddSpline( std::vector<std::pair<double, double> > points );
    virtual void AddEllipse(double x_center, double y_center, double a, double b);
    virtual void AddEllipse(double x_center, double y_center, double a, double b, double theta);

private:
    std::vector<std::pair<geometry_types,OGRFeature*> > m_geometries_features;
    int m_layertype;
    double m_center_x, m_center_y;
    unsigned int m_nb_geometries;
    // 1 --> image; -1 --> cartographic coordinates
    int m_coordinates;

    void compute_center(OGRLayer* layer, int nb_layers);

    unsigned int m_point_width, m_line_width, m_line_style, m_polygon_border_width, m_polygon_border_style, m_polygon_inner_style;
    wxColor m_point_color, m_line_color, m_polygon_border_color, m_polygon_inner_color;
};

#endif // OGR_VECTOR_LAYER_HPP
