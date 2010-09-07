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

#include "GilViewer/gui/GlobalSettingsControl.hpp"

#include <wx/msgdlg.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/valtext.h>

#include "../gui/PanelViewer.hpp"
#include "../gui/LayerControl.hpp"

BEGIN_EVENT_TABLE(global_settings_control, wxDialog)
        EVT_BUTTON(wxID_OK,global_settings_control::on_ok_button)
        EVT_BUTTON(wxID_CANCEL,global_settings_control::on_cancel_button)
        EVT_BUTTON(wxID_APPLY,global_settings_control::on_apply_button)
        EVT_CLOSE(global_settings_control::on_close_window)
        END_EVENT_TABLE()

global_settings_control::global_settings_control(layer_control* parent, wxWindowID id, const wxString& title, long style, const wxPoint& pos, const wxSize& size) :
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
    buttons_sizer->AddButton(new wxButton(this,wxID_OK, wxT("OK")));
    buttons_sizer->AddButton(new wxButton(this,wxID_APPLY, wxT("Apply")));
    buttons_sizer->AddButton(new wxButton(this,wxID_CANCEL, wxT("Cancel")));
    buttons_sizer->Realize();
    main_sizer->Add(buttons_sizer, 0, wxALIGN_RIGHT|wxALL, 5);

    main_sizer->SetSizeHints(this);
    SetSizer(main_sizer);
    SetMaxSize(this->GetSize());
    Layout();
    Centre();
}

void global_settings_control::on_ok_button(wxCommandEvent &event)
{
    on_apply_button(event);
    Hide();
}

void global_settings_control::on_cancel_button(wxCommandEvent &event)
{
    Hide();
}

void global_settings_control::on_apply_button(wxCommandEvent &event)
{
    if ( !Validate() )
        ::wxMessageBox(_("Incorrect entry!"));

    // et bien on parcourt tous les calques image du layer control et on applique la transformation ...
    // Pour l'instant, on se concentre sur le canal RED ...
    wxString MinLabel = m_textMinimumGlobalIntensity->GetValue();
    wxString MaxLabel = m_textMaximumGlobalIntensity->GetValue();
    double MinDouble, MaxDouble;
    MinLabel.ToDouble(&MinDouble);
    MaxLabel.ToDouble(&MaxDouble);

    for (layer_control::LayerContainerType::iterator it = m_parent->begin(); it != m_parent->end(); ++it)
    {
        (*it)->intensity_min(MinDouble);
        (*it)->intensity_max(MaxDouble);
        (*it)->needs_update(true);

        // MAJ de l'interface
        (*it)->notifyLayerSettingsControl_();
    }

    m_parent->panelviewer()->Refresh();
}

void global_settings_control::on_close_window(wxCloseEvent& event)
{
    Hide();
}

bool global_settings_control::validate()
{
    bool checkTest = wxDialog::Validate();
    return checkTest;
}
