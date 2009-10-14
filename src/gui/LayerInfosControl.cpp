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

#include "gui/LayerInfosControl.hpp"

#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/stattext.h>

BEGIN_EVENT_TABLE(LayerInfosControl, wxFrame)
EVT_BUTTON(wxID_OK,LayerInfosControl::OnOKButton)
END_EVENT_TABLE()

LayerInfosControl::LayerInfosControl(const std::string &infos ,wxWindow* parent, wxWindowID id, const wxString& title, const unsigned long style, const wxPoint& position, const wxSize& size) :
wxFrame(parent,id,title,position,size,style)
{
	wxColour bgcolor( 220, 220, 220);
	SetBackgroundColour(bgcolor);
	ClearBackground();

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText *text = new wxStaticText(this,wxID_ANY,wxString(infos.c_str(),*wxConvCurrent));
	sizer->Add( text , 0 , wxALL|wxALIGN_CENTRE , 5);

	wxButton *okButton = new wxButton(this,wxID_OK,_("OK"));
	sizer->Add( okButton , 0 , wxALL|wxALIGN_CENTRE , 5);

	sizer->SetSizeHints(this);
	SetSizer(sizer);
}

LayerInfosControl::~LayerInfosControl()
{
	Destroy();
}

void LayerInfosControl::OnOKButton(wxCommandEvent &event)
{
	Destroy();
}
