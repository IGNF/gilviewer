/***********************************************************************

This file is part of the GilViewer project source files.

GilViewer is an open source 2D viewer (raster and vector) based on Boost
GIL and wxWidgets. GilViewer is distributed under the CeCILL-B licence. 
See Licence_CeCILL-B_V1-en.txt or http://www.cecill.info for more details.


Homepage: 

	http://launchpad.net/gilviewer
	
Copyright:
	
	Institut Geographique National (2009)

Authors: 

	Olivier Tournaire, Adrien Chauve

	
	

This software is governed by the CeCILL-B license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL-B
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL-B license and that you accept its terms.
 
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
