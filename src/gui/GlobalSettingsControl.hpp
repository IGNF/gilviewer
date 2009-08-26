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

#ifndef __GLOBAL_SETTINGS_CONTROL_HPP__
#define __GLOBAL_SETTINGS_CONTROL_HPP__

#include <wx/dialog.h>

class LayerControl;
class wxTextCtrl;

class GlobalSettingsControl : public wxDialog
{
public:
	GlobalSettingsControl(LayerControl *parent, wxWindowID id = wxID_ANY, const wxString& title = _("Global settings"), long style = wxDEFAULT_FRAME_STYLE , const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize );
	virtual ~GlobalSettingsControl() {};

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

	LayerControl *m_parent;
};

#endif //__GLOBAL_SETTINGS_CONTROL_HPP__
