#include "LayerControlUtils.hpp"
#include "../gui/VectorLayerSettingsControl.hpp"
#include "../gui/LayerControl.hpp"

#include "../gui/define_id.hpp"

#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                  ///
///                                 SelectableStaticText                                             ///
///                                                                                                  ///
////////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(selectable_static_text, wxStaticText)
    EVT_LEFT_DOWN(selectable_static_text::on_click)
END_EVENT_TABLE()

void selectable_static_text::on_click(wxMouseEvent& event)
{
    // On effectue la modification de couleur sur le calque selectionne et on change le flag
    if (m_isSelected == false)
    {
        //SetBackgroundColour( m_selectedColour );
        SetForegroundColour(m_selectedColour);
        //ClearBackground();
        SetLabel( GetLabel());
        m_isSelected = true;
    }
    else
    {
        //SetBackgroundColour( m_unselectedColour );
        SetForegroundColour(m_unselectedColour);
        //ClearBackground();
        SetLabel( GetLabel());
        m_isSelected = false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                  ///
///                                 LayerControlRow                                                  ///
///                                                                                                  ///
////////////////////////////////////////////////////////////////////////////////////////////////////////

layer_control_row::layer_control_row(layer_control* parent, const std::string &name, const unsigned int index, layer_settings_control *layersettings, const std::string &tooltip) :
	m_parent(parent), m_index(index)
{
    // On ajoute l'ensemble des controles
    wxString name_wx(name.c_str(), *wxConvCurrent);
    wxString tool(tooltip.c_str(), *wxConvCurrent);
    wxFont fontFrameViewer(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    m_nameStaticText = new selectable_static_text(m_parent->m_scroll, m_parent, wxID_HIGHEST + 12000, name_wx);
    m_nameStaticText->SetFont(fontFrameViewer);
    m_nameStaticText->SetToolTip(tool);

    m_visibilityCheckBox = new wxCheckBox(m_parent->m_scroll, ID_VISIBILITY + m_index, wxT(""));
    m_visibilityCheckBox->SetValue(true);
    m_visibilityCheckBox->SetToolTip(_("Change layer visibility"));
    m_visibilityCheckBox->Connect(ID_VISIBILITY + m_index, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(layer_control::on_check_visibility), NULL, m_parent);

    m_transformationCheckBox = new wxCheckBox(m_parent->m_scroll, ID_TRANSFORMATION + m_index, wxT(""));
    m_transformationCheckBox->SetValue(true);
    m_transformationCheckBox->SetToolTip(_("Change layer transformability"));
    m_transformationCheckBox->Connect(ID_TRANSFORMATION + m_index, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(layer_control::on_check_transformable), NULL, m_parent);

    m_boxSizer = new wxBoxSizer(wxHORIZONTAL);

    const wxSize imageSize(16,16);

    // L'icone du bouton  info depend du type : vecteur ou image
    m_infoButton = new wxBitmapButton(m_parent->m_scroll, ID_INFO + m_index, wxXmlResource::Get()->LoadBitmap(wxString(layersettings->get_info_button_name().c_str(), *wxConvCurrent)));
    m_infoButton->SetToolTip(_("Display layer informations"));
    m_boxSizer->Add(m_infoButton, 0, wxALL | wxALIGN_CENTRE, 5);
    m_infoButton->Connect(ID_INFO + m_index, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(layer_control::on_info_button), NULL, m_parent);

    m_saveButton = new wxBitmapButton(m_parent->m_scroll, ID_SAVE + m_index, wxXmlResource::Get()->LoadBitmap( wxT("MEDIA-FLOPPY_16x16") ));
    m_saveButton->SetToolTip(_("Save layer"));
    m_boxSizer->Add(m_saveButton, 0, wxALL | wxALIGN_CENTRE, 5);
    m_saveButton->Connect(ID_SAVE + m_index, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(layer_control::on_save_button), NULL, m_parent);

    m_deleteButton = new wxBitmapButton(m_parent->m_scroll, ID_DELETE + m_index, wxXmlResource::Get()->LoadBitmap( wxT("USER-TRASH_16x16") ) );
    m_deleteButton->SetToolTip(_("Delete layer"));
    m_boxSizer->Add(m_deleteButton, 0, wxALL | wxALIGN_CENTRE, 5);
    m_deleteButton->Connect(ID_DELETE + m_index, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(layer_control::on_delete_button), NULL, m_parent);

    m_settingsButton = new wxBitmapButton(m_parent->m_scroll, ID_SETTINGS + m_index, wxXmlResource::Get()->LoadBitmap( wxT("APPLICATIONS-SYSTEM_16x16") ) );
    m_settingsButton->SetToolTip(_("Layer settings"));
    m_boxSizer->Add(m_settingsButton, 0, wxALL | wxALIGN_CENTRE, 5);
    m_settingsButton->Connect(ID_SETTINGS + m_index, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(layer_control::on_settings_button), NULL, m_parent);

    m_center_button = new wxBitmapButton(m_parent->m_scroll, ID_CENTER + m_index, wxXmlResource::Get()->LoadBitmap( wxT("VIEW_FULLSCREEN_16x16") ) );
    m_center_button->SetToolTip(_("Center layer"));
    m_boxSizer->Add(m_center_button, 0, wxALL | wxALIGN_CENTRE, 5);
    m_center_button->Connect(ID_CENTER + m_index, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(layer_control::on_center_button), NULL, m_parent);

    // On lie la ligne au LayerSettingsControl
    m_layerSettingsControl = layersettings;
}
