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

#include <boost/bind.hpp>

#include "../layers/Layer.hpp"

class vector_layer : public layer
{
public:
    virtual ~vector_layer() {}
    /// Cette methode recupere les donnees d'affichage par defaut dans les parametres et les affecte au nouveau calque
    virtual void default_display_parameters();

    // Abstract interface
    virtual void draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent) const {}
    virtual void update(int width, int height) {};

    virtual std::string available_formats_wildcard() const;
    virtual std::vector<std::string> available_formats_extensions() const;

    // Accessors
    virtual std::string layer_type_as_string() const {return "Vector";}
    virtual bool saveable() const {return true;}

    // TODO
    virtual void text_visibility( bool value , bool update = true ) { m_is_text_visible = value; if (update) notifyLayerSettingsControl_(); }
    virtual bool text_visibility() const { return m_is_text_visible; }

    virtual void clear();

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
