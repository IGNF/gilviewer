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

#ifndef __VECTOR_LAYER_HPP__
#define __VECTOR_LAYER_HPP__
#ifdef WIN32
	#pragma warning(disable : 4251)
	#pragma warning(disable : 4275)
#endif
#include <boost/bind.hpp>

#include "../layers/layer.hpp"

class vector_layer : public layer
{
public:
    virtual ~vector_layer() {}
    /// Cette methode recupere les donnees d'affichage par defaut dans les parametres et les affecte au nouveau calque
    virtual void default_display_parameters();

    // Abstract interface
    virtual void draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent) const {}
    virtual void update(int width, int height) {};

    // Accessors
    virtual std::string layer_type_as_string() const {return "Vector";}
    virtual bool saveable() const {return true;}

    // TODO
    virtual void text_visibility( bool value , bool update = true ) { m_is_text_visible = value; if (update) notifyLayerSettingsControl_(); }
    virtual bool text_visibility() const { return m_is_text_visible; }

    virtual void clear();

    virtual unsigned int num_polygons() const { return 0; }
    virtual void get_polygon(unsigned int i, std::vector<double> &x , std::vector<double> &y ) const {}

    virtual unsigned int num_points() const { return 0; }
    virtual void get_point(unsigned int i, double &x , double &y ) const {}

    virtual unsigned int num_polylines() const { return 0; }
    virtual void get_polyline(unsigned int i, std::vector<double> &x , std::vector<double> &y ) const {}

    virtual unsigned int num_lines() const { return 0; }
    virtual void get_line(unsigned int i, double& x1 , double& y1 , double& x2 , double& y2 ) const {}

private:
    // Textx display
    bool m_is_text_visible;
    std::vector< std::pair<double,double> > m_text_coordinates;
    std::vector< std::string > m_text_value;

protected:
    vector_layer(): m_is_text_visible(true)
            , m_text_coordinates(std::vector< std::pair<double,double> >())
            , m_text_value(std::vector< std::string >()) {}
};


#endif // __VECTOR_LAYER_HPP__
