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

#ifndef __IMAGE_LAYER_INFOS_CONTROL_HPP__
#define __IMAGE_LAYER_INFOS_CONTROL_HPP__

#include <wx/frame.h>

class layer_infos_control : public wxFrame
{
public:
        layer_infos_control(const std::string &infos ,wxWindow* parent, wxWindowID id, const wxString& title, const unsigned long style, const wxPoint& position=wxDefaultPosition, const wxSize& size=wxDefaultSize);
        virtual ~layer_infos_control();

	void OnOKButton(wxCommandEvent &event);

	DECLARE_EVENT_TABLE();
};

typedef layer_infos_control LayerInfosControl;

#endif // __IMAGE_LAYER_INFOS_CONTROL_HPP__
