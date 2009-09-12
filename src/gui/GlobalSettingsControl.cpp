/***********************************************************************

This file is part of the GilViewer project source files.

GilViewer is an open source 2D viewer (raster and vector) based on Boost
GIL and wxWidgets.


Homepage: 

	http://launchpad.net/gilviewer
	
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
