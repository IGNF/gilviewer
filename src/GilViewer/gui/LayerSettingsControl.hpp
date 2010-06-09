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

#ifndef __LAYER_SETTINGS_CONTROL_HPP__
#define __LAYER_SETTINGS_CONTROL_HPP__

#include <wx/dialog.h>

class layer_control;

class layer_settings_control : public wxDialog
{
public:
    layer_settings_control(layer_control *parent, wxWindowID id = wxID_ANY, const wxString& title = _("Layer settings"), const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize, const long style=wxDEFAULT_FRAME_STYLE) : wxDialog( (wxWindow*)parent, id, title, pos, size, style){;}
    ~layer_settings_control() {}

    // Cette methode permet de mettre a jour l'interface lorsque des changements sont fait a partir du code (changement de style, de couleur ...)
    virtual void update() {}

    unsigned int Index() { return m_index; }
    void Index(const unsigned int index){  m_index = index; }

    virtual std::string get_info_button_name() const {return "";}
    virtual const char** icon_xpm() const {return NULL;}

protected:
    unsigned int m_index;
};

typedef layer_settings_control LayerSettingsControl;

#endif // __LAYER_SETTINGS_CONTROL_HPP__
