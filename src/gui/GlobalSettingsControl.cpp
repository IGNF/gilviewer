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

#include "gui/GlobalSettingsControl.hpp"

#include <wx/msgdlg.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/valtext.h>

#include "gui/PanelViewer.hpp"
#include "gui/LayerControl.hpp"

GlobalSettingsControl::GlobalSettingsControl(LayerControl* parent, wxWindowID id, const wxString& title, long style, const wxPoint& pos, const wxSize& size) :
	wxDialog( (wxWindow*)parent, id, title, pos, size, style), m_parent(parent)
{
	wxBoxSizer *main_sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer *intensity_sizer = new wxStaticBoxSizer(wxHORIZONTAL,this,_("Intensity"));
	m_textMinimumGlobalIntensity = new wxTextCtrl(this,wxID_ANY);
	m_textMinimumGlobalIntensity->SetValidator(wxTextValidator(wxFILTER_NUMERIC));
	intensity_sizer->Add(m_textMinimumGlobalIntensity, 1, wxEXPAND|wxALL|wxADJUST_MINSIZE, 5);
	m_textMaximumGlobalIntensity = new wxTextCtrl(this,wxID_ANY);
	m_textMaximumGlobalIntensity->SetValidator(wxTextValidator(wxFILTER_NUMERIC));
	intensity_sizer->Add(m_textMaximumGlobalIntensity, 1, wxEXPAND|wxALL|wxADJUST_MINSIZE, 5);

	main_sizer->Add(intensity_sizer, 0, wxEXPAND|wxALL|wxADJUST_MINSIZE, 5);

	wxStdDialogButtonSizer *buttons_sizer = new wxStdDialogButtonSizer();
	buttons_sizer->AddButton(new wxButton(this,wxID_OK, _("OK")));
	buttons_sizer->AddButton(new wxButton(this,wxID_APPLY, _("Apply")));
	buttons_sizer->AddButton(new wxButton(this,wxID_CANCEL, _("Cancel")));
	buttons_sizer->Realize();
	main_sizer->Add(buttons_sizer, 0, wxALIGN_RIGHT|wxALL, 5);

	main_sizer->SetSizeHints(this);
	SetSizer(main_sizer);
	SetMaxSize(this->GetSize());
	Layout();
	Centre();
}

BEGIN_EVENT_TABLE(GlobalSettingsControl, wxDialog)
EVT_CLOSE(GlobalSettingsControl::OnCloseWindow)
EVT_BUTTON(wxID_OK,GlobalSettingsControl::OnOKButton)
EVT_BUTTON(wxID_CANCEL,GlobalSettingsControl::OnCancelButton)
EVT_BUTTON(wxID_APPLY,GlobalSettingsControl::OnApplyButton)
END_EVENT_TABLE()

void GlobalSettingsControl::OnApplyButton(wxCommandEvent &event)
{
	if ( !Validate() )
		::wxMessageBox(_("Saisie incorrecte !"));

	// et bien on parcourt tous les calques image du layer control et on applique la transformation ...
	// Pour l'instant, on se concentre sur le canal RED ...
	wxString MinLabel = m_textMinimumGlobalIntensity->GetValue();
	wxString MaxLabel = m_textMaximumGlobalIntensity->GetValue();
	double MinDouble, MaxDouble;
	MinLabel.ToDouble(&MinDouble);
	MaxLabel.ToDouble(&MaxDouble);

	for (LayerControl::LayerContainerType::iterator it = m_parent->begin(); it != m_parent->end(); ++it)
	{
		(*it)->IntensityMin(MinDouble);
		(*it)->IntensityMax(MaxDouble);
		(*it)->HasToBeUpdated(true);

                // MAJ de l'interface
                (*it)->notifyLayerSettingsControl_();
	}

	m_parent->GetPanelViewer()->Refresh();
}

void GlobalSettingsControl::OnOKButton(wxCommandEvent &event)
{
	Show(false);
}

void GlobalSettingsControl::OnCancelButton(wxCommandEvent &event)
{
	// On sauvegarde les valeurs des param�tres courants
}

void GlobalSettingsControl::OnCloseWindow(wxCloseEvent& event)
{
	Hide();
}

bool GlobalSettingsControl::Validate()
{
	bool checkTest = wxDialog::Validate();
	return checkTest;
}
