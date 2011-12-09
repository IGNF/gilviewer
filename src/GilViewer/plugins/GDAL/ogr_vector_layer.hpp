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

#include "GilViewer/layers/vector_layer.hpp"

#include <boost/mpl/vector.hpp>
#include <boost/variant/variant.hpp>

class layer_settings_control;

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

class ogr_vector_layer : public vector_layer
{
public:
    /// Constructeur a partir d'un nom de calque et d'un fichier shapefile
    ogr_vector_layer(const std::string &layer_name, const std::string &filename);
    /// Constructeur vide: pour creer un layer a remplir a la main ...
    ogr_vector_layer(const std::string &layer_name="default name"): vector_layer(),
            m_nb_geometries(0) { m_name=layer_name;}
    /// @param layerName Le nom du calque
    /// @param shapefileFileName Le chemin vers le fichier shapefile
    virtual ~ogr_vector_layer();

    virtual void draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent) const;
    virtual void update(int, int) {}

    virtual std::string available_formats_wildcard() const;

    virtual layer_settings_control* build_layer_settings_control(unsigned int index, layer_control* parent);

    inline virtual double center_x() {return m_center_x;}
    inline virtual double center_y() {return m_center_y;}

    const std::vector<std::pair<geometry_types,OGRFeature*> >& geometries_features() const;
    std::vector<std::pair<geometry_types,OGRFeature*> >& geometries_features();


    virtual void add_point( double x , double y );
    virtual void add_text( double x , double y , const std::string &text , const wxColour &color = *wxRED );
    virtual void add_line( double x1 , double y1 , double x2 , double y2 );
    virtual void add_polyline( const std::vector<double> &x , const std::vector<double> &y );
    virtual void add_polygon( const std::vector<double> &x , const std::vector<double> &y );
    virtual void add_circle( double x , double y , double radius );
    virtual void add_spline( std::vector<std::pair<double, double> > points );
    virtual void add_ellipse(double x_center, double y_center, double a, double b);
    virtual void add_ellipse(double x_center, double y_center, double a, double b, double theta);

    virtual unsigned int num_polygons() const;
    virtual void get_polygon(unsigned int i, std::vector<double> &x , std::vector<double> &y ) const;
    virtual unsigned int num_points() const;
    virtual void get_point(unsigned int i, double &x , double &y ) const;
    virtual unsigned int num_polylines() const;
    virtual void get_polyline(unsigned int i, std::vector<double> &x , std::vector<double> &y ) const;

    virtual void clear();

    typedef OGRPolygon* polygon_type;

private:
    std::vector<std::pair<geometry_types,OGRFeature*> > m_geometries_features;
    typedef struct __internal_point { double x, y; } internal_point_type;
    std::vector< std::pair< internal_point_type , std::string > > m_texts;
    double m_center_x, m_center_y;
    unsigned int m_nb_geometries;
    unsigned int m_nb_linear_rings;
    unsigned int m_nb_line_strings;
    unsigned int m_nb_multiline_strings;
    unsigned int m_nb_multipoints;
    unsigned int m_nb_points;
    unsigned int m_nb_multipolygons;
    unsigned int m_nb_polygons;

    void compute_center(OGRLayer* layer, int nb_layers);
    void build_infos(OGRSpatialReference *spatial_reference);

};

#endif // OGR_VECTOR_LAYER_HPP
