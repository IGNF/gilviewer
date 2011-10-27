/***********************************************************************

This file is part of the GilViewer project source files.

GilViewer is an open source 2D viewer (raster and vector) based on Boost
GIL and wxWidgets.


Homepage:

        http://code.google.com/p/gilviewer

Copyright:

        Institut Geographique National (2011)

Authors:

        Mathieu Brédif




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
#ifndef CGAL_VECTOR_LAYER_HPP
#define CGAL_VECTOR_LAYER_HPP

#include "GilViewer/layers/vector_layer.hpp"

#include <boost/mpl/vector.hpp>
#include <boost/variant/variant.hpp>

class layer_settings_control;


class wxColor;
class Arrangement;

class cgal_vector_layer : public vector_layer
{
public:
    /// Constructeur a partir d'un nom de calque et d'un fichier shapefile
    cgal_vector_layer(const std::string &layer_name, const std::string &filename);
    /// Constructeur vide: pour creer un layer a remplir a la main ...
    cgal_vector_layer(const std::string &layer_name="default name");
    /// @param layerName Le nom du calque
    /// @param shapefileFileName Le chemin vers le fichier shapefile
    virtual ~cgal_vector_layer();

    void load(const std::string& filename);

    virtual void draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent) const;
    virtual void update(int, int) {}
    virtual bool snap( eSNAP snap, double d2[], const wxRealPoint& p, wxRealPoint& psnap );

    virtual std::string available_formats_wildcard() const;

    virtual layer_settings_control* build_layer_settings_control(unsigned int index, layer_control* parent);

    virtual void add_point( double x , double y );
    virtual void add_line( double x1 , double y1 , double x2 , double y2 );
    virtual void add_polyline( const std::vector<double> &x , const std::vector<double> &y );
    virtual void add_polygon( const std::vector<double> &x , const std::vector<double> &y );
    void save( const std::string& filename ) const;

 /*
    virtual void add_text( double x , double y , const std::string &text , const wxColour &color = *wxRED );
    virtual void add_circle( double x , double y , double radius );
    virtual void add_spline( std::vector<std::pair<double, double> > points );
    virtual void add_ellipse(double x_center, double y_center, double a, double b);
    virtual void add_ellipse(double x_center, double y_center, double a, double b, double theta);
*/

    void select(const wxRealPoint& p);

    virtual void clear();

private:
    Arrangement *m_arrangement;
    wxColor m_selection_color;
};

#endif // CGAL_VECTOR_LAYER_HPP
