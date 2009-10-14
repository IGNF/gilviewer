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

#ifndef __NEW_VECTOR_LAYER_CONTROL_HPP__
#define __NEW_VECTOR_LAYER_CONTROL_HPP__

#include <vector>
#include <map>

#include <wx/dialog.h>

class wxChoice;
class wxTextCtrl;
class wxWindow;

class NewVectorLayerControl : public wxDialog
{
public:
	NewVectorLayerControl(wxWindow* parent,
						  wxWindowID id = wxID_ANY,
						  const wxString& title = _("Layer settings"),
						  const wxPoint &pos=wxDefaultPosition,
						  const wxSize &size=wxDefaultSize,
						  const long style=wxDEFAULT_FRAME_STYLE);
	virtual ~NewVectorLayerControl() {;}

	std::string GetLayerTypeAsString();
	int GetLayerType();
	std::string GetLayerName();

	DECLARE_EVENT_TABLE();

	std::vector< std::string > m_associationStringType;
	std::map< std::string , int > m_associationMapStringType;
private:
	wxChoice *m_choiceLayerType;
	wxTextCtrl* m_textName;
	wxWindow *m_parent;
};

#endif // __NEW_VECTOR_LAYER_CONTROL_HPP__
