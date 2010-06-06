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

#ifndef __GLOBAL_SETTINGS_CONTROL_HPP__
#define __GLOBAL_SETTINGS_CONTROL_HPP__

#include <wx/dialog.h>

class layer_control;
class wxTextCtrl;

class global_settings_control : public wxDialog
{
public:
    global_settings_control(layer_control *parent, wxWindowID id = wxID_ANY, const wxString& title = _("Global settings"), long style = wxDEFAULT_FRAME_STYLE , const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize );
    virtual ~global_settings_control() {};

    void OnScroll(wxScrollEvent &event);
    void OnOKButton(wxCommandEvent &event);
    void OnCancelButton(wxCommandEvent &event);
    void OnApplyButton(wxCommandEvent &event);
    void OnCloseWindow(wxCloseEvent& event);

    bool Validate();

    DECLARE_EVENT_TABLE();

private:
    // Je laisse beaucoup de choses pour l'instant, ca pourra servir lors des evolutions futures
    wxTextCtrl* m_textMinimumGlobalIntensity;
    wxTextCtrl* m_textMaximumGlobalIntensity;

    layer_control *m_parent;
};

typedef global_settings_control GlobalSettingsControl;

#endif //__GLOBAL_SETTINGS_CONTROL_HPP__
