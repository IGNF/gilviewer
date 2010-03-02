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

#include <map>

#include <boost/bind.hpp>

#include "../gui/VectorLayerSettingsControl.hpp"
#include "../layers/Layer.hpp"

class VectorLayer : public Layer
{
public:
    /// Cette methode recupere les donnees d'affichage par defaut dans les parametres et les affecte au nouveau calque
    virtual void SetDefaultDisplayParameters();

    // Abstract interface
    virtual void Draw(wxDC &dc, wxCoord x, wxCoord y, bool transparent) const {}
    virtual void Update(int width, int height) {};

    // Accessors
    virtual std::string get_layer_type_as_string() const {return "Vector";}

    // TODO
    virtual void TextsVisibility( bool value , bool update = true ) { m_isTextVisible = value; if (update) notifyLayerSettingsControl_(); }
    virtual bool TextsVisibility() const { return m_isTextVisible; }

    virtual void Clear();

private:
    // Textx display
    bool m_isTextVisible;
    std::vector< std::pair<double,double> > m_textCoordinates;
    std::vector< std::string > m_textValue;

protected:
    VectorLayer() : m_isTextVisible(true) {}
};


#endif // __VECTOR_LAYER_HPP__
