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

#include "vector_layer_settings_control.hpp"
#include "../gui/resources/polygon_icon.xpm"

#include <wx/frame.h>
#include <wx/choice.h>
#include <wx/colordlg.h>
#include <wx/radiobox.h>
#include <wx/image.h>
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/slider.h>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include <wx/checkbox.h>
#include <wx/fontpicker.h>
#include <wx/clrpicker.h>

#include "../gui/layer_control.hpp"
#include "../gui/panel_viewer.hpp"

#include "../convenient/wxhelper.hpp"

#include "../gui/define_id.hpp"

wxString vector_layer_settings_control::choices_points[] =
{ _("Point"), _("Transparent"), _("Cross"), _("Plus"), _("Triangle"), _("Circle") };

wxString vector_layer_settings_control::choices_inside_polygons[] =
{ _("Solid"), _("Transparent"), _("Backward diagonal hatch"), _("Forward diagonal hatch"), _("Cross diagonal hatch"), _("Horizontal hatch"), _("Vertical hatch"), _("Cross hatch") };

wxString vector_layer_settings_control::choices_lines[] =
{ _("Solid"), _("Transparent"), _("Dot"), _("Long dash"), _("Short dash"), _("Dot dash") };

int vector_layer_settings_control::style_lines[] =
{ wxSOLID, wxTRANSPARENT, wxDOT, wxLONG_DASH, wxSHORT_DASH, wxDOT_DASH };

int vector_layer_settings_control::style_inside_polygons[] =
{ wxSOLID, wxTRANSPARENT, wxBDIAGONAL_HATCH, wxFDIAGONAL_HATCH, wxCROSSDIAG_HATCH, wxHORIZONTAL_HATCH, wxVERTICAL_HATCH, wxCROSS_HATCH };

BEGIN_EVENT_TABLE(vector_layer_settings_control, wxDialog)
        EVT_BUTTON(wxID_OK,vector_layer_settings_control::on_ok_button)
        EVT_BUTTON(wxID_CANCEL,vector_layer_settings_control::on_cancel_button)
        EVT_BUTTON(wxID_APPLY,vector_layer_settings_control::on_apply_button)
        EVT_CLOSE(vector_layer_settings_control::on_close_window)
END_EVENT_TABLE()

vector_layer_settings_control::vector_layer_settings_control(unsigned int index_, layer_control* parent, wxWindowID id, const wxString& title, long style, const wxPoint& pos, const wxSize& size) :
layer_settings_control(parent, id, title, pos, size, style), m_parent(parent)
{
    index(index_);
    m_colourPickerPoints = NULL;
    m_colourPickerRingsPolygons = NULL;
    m_colourPickerInsidePolygons = NULL;
    m_colourPickerLines = NULL;
    m_sliderWidthPoints = NULL;
    m_sliderWidthLines = NULL;
    m_sliderWidthRings = NULL;
    m_choicePoints = NULL;
    m_choicePolygons = NULL;
    m_choiceLines = NULL;
    m_choiceLabels = NULL;

    wxColour bgcolor(220, 220, 220);
    SetBackgroundColour(bgcolor);
    ClearBackground();

        m_main_sizer = new wxFlexGridSizer(3,1,0,0);
    m_main_sizer->AddGrowableCol(0);
    m_main_sizer->AddGrowableRow(0);
    m_main_sizer->AddGrowableRow(1);
    m_main_sizer->AddGrowableRow(2);

    ////////////////////////
    ///
    ///  Points settings
    ///
    ////////////////////////
    {
        wxStaticBoxSizer *boxSizerPoints = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Points settings"));
        m_colourPickerPoints = new wxColourPickerCtrl(this, wxID_ANY, *wxRED, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
        wxStaticBoxSizer *boxSizerWidthPoints = new wxStaticBoxSizer(wxVERTICAL, this, _("Width"));
        m_sliderWidthPoints = new wxSlider(this, wxID_ANY, 3, 1, 10, wxDefaultPosition, wxSize(100, 40));
        boxSizerWidthPoints->Add(m_sliderWidthPoints, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
        boxSizerPoints->Add(m_colourPickerPoints, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
        boxSizerPoints->Add(boxSizerWidthPoints, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

        //m_choicePoints = new wxRadioBox( this, wxID_ANY, _("Points style"), wxDefaultPosition, wxDefaultSize, WXSIZEOF(VectorLayerSettingsControl::choices_points), VectorLayerSettingsControl::choices_points, 1, wxRA_SPECIFY_COLS );
        m_choicePoints = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, WXSIZEOF(vector_layer_settings_control::choices_points), vector_layer_settings_control::choices_points);
        m_choicePoints->SetSelection(0);
        m_choicePoints->Enable(false);
        boxSizerPoints->Add(m_choicePoints, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
        m_main_sizer->Add(boxSizerPoints, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
    }

    ////////////////////////
    ///
    ///  Lines settings
    ///
    ////////////////////////
    {
        wxStaticBoxSizer *boxSizerLines = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Lines settings"));
        m_colourPickerLines = new wxColourPickerCtrl(this, wxID_ANY, *wxRED, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
        wxStaticBoxSizer *boxSizerWidthLines = new wxStaticBoxSizer(wxVERTICAL, this, _("Width"));
        m_sliderWidthLines = new wxSlider(this, wxID_ANY, 1, 1, 10, wxDefaultPosition, wxSize(100, 40));
        boxSizerWidthLines->Add(m_sliderWidthLines, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
        boxSizerLines->Add(m_colourPickerLines, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
        boxSizerLines->Add(boxSizerWidthLines, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

        m_choiceLines = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, WXSIZEOF(vector_layer_settings_control::choices_lines), vector_layer_settings_control::choices_lines);
        m_choiceLines->SetSelection(0);
        boxSizerLines->Add(m_choiceLines, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
        m_main_sizer->Add(boxSizerLines, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
    }

    ////////////////////////
    ///
    ///  Polygons settings
    ///
    ////////////////////////
    {
        wxStaticBoxSizer *boxSizerPolygons = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Polygons settings"));
        wxBoxSizer *boxColorPickersPolygons = new wxBoxSizer(wxVERTICAL);
        wxStaticBoxSizer *boxSizerRingsColour = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Rings colour"));
        wxStaticBoxSizer *boxSizerInsideColour = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Inside colour"));
        m_colourPickerRingsPolygons = new wxColourPickerCtrl(this, wxID_ANY, *wxRED, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
        m_colourPickerInsidePolygons = new wxColourPickerCtrl(this, wxID_ANY, *wxBLUE, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
        boxSizerRingsColour->Add(m_colourPickerRingsPolygons, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
        boxSizerInsideColour->Add(m_colourPickerInsidePolygons, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
        boxColorPickersPolygons->Add(boxSizerRingsColour, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
        boxColorPickersPolygons->Add(boxSizerInsideColour, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
        wxBoxSizer *boxSlidersPolygons = new wxBoxSizer(wxVERTICAL);
        wxStaticBoxSizer *boxSizerRingsWidth = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Rings width"));
        // Ring width
        m_sliderWidthRings = new wxSlider(this, wxID_ANY, 1, 1, 10, wxDefaultPosition, wxSize(100, 40));
        boxSizerRingsWidth->Add(m_sliderWidthRings, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
        boxSlidersPolygons->Add(boxSizerRingsWidth, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

        boxSizerPolygons->Add(boxColorPickersPolygons, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
        boxSizerPolygons->Add(boxSlidersPolygons, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

        m_choicePolygons = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, WXSIZEOF(vector_layer_settings_control::choices_inside_polygons), vector_layer_settings_control::choices_inside_polygons);
        m_choicePolygons->SetSelection(0);
        boxSizerPolygons->Add(m_choicePolygons, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
        m_main_sizer->Add(boxSizerPolygons, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
    }

    ////////////////////////
    ///
    ///  DBF : etiquettes
    ///
    ////////////////////////
    /*
    if (ogr_vectorLayer || (vectorLayer && (vectorLayer->LayerContent()->FlagDBF())))
    {
        wxStaticBoxSizer *boxSizerLabels = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Labels"));

        wxArrayString attributesNames;
        // On parcourt le vecteur et on recupere les valeurs (on met une chaine vide en premier --> permet de ne rien afficher)
        attributesNames.Add(wxT(""));
        for (unsigned int i = 0; i < vectorLayer->LayerContent()->m_dbfAttributesNames.size(); ++i)
            attributesNames.Add(vectorLayer->LayerContent()->m_dbfAttributesNames[i]);
        // Pffffffffffffffff
        wxCArrayString temp(attributesNames);
        m_choiceLabels = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, attributesNames.GetCount(), temp.GetStrings());
        boxSizerLabels->Add(m_choiceLabels, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
        m_main_sizer->Add(boxSizerLabels, 1, wxEXPAND | wxALL, 5);
    }
    */

    ////////////////////////
    ///
    ///  Texts
    ///
    ////////////////////////

    wxStaticBoxSizer *boxSizerTexts = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Texts settings"));
    // texts color
    wxStaticBoxSizer *boxSizerTextsColor = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Texts color"));
    boxSizerTexts->Add(boxSizerTextsColor, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
    m_colourPickerTexts = new wxColourPickerCtrl(this, wxID_ANY, *wxRED, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
    boxSizerTextsColor->Add(m_colourPickerTexts, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
    /*
	// texts font
	wxStaticBoxSizer *boxSizerTextsFont = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Texts font"));
	boxSizerTexts->Add(boxSizerTextsFont, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	m_fontPickerTexts = new wxFontPickerCtrl(this, wxID_ANY, wxNullFont, wxDefaultPosition, wxDefaultSize, wxFNTP_USE_TEXTCTRL);
	boxSizerTextsFont->Add(m_fontPickerTexts, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
        */
    // texts visibility
    wxStaticBoxSizer *boxSizerTextsVisibility = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Texts visibility"));
    boxSizerTexts->Add(boxSizerTextsVisibility, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
    m_checkShowTexts = new wxCheckBox(this, wxID_ANY, _("Show texts"));
    m_checkShowTexts->SetValue(true);
    boxSizerTextsVisibility->Add(m_checkShowTexts, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

    m_main_sizer->Add(boxSizerTexts, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

    ////////////////////////
    ///
    ///  Buttons
    ///
    ////////////////////////
    wxStdDialogButtonSizer *buttons_sizer = new wxStdDialogButtonSizer();
    buttons_sizer->AddButton(new wxButton(this, wxID_OK, wxT("OK")));
    buttons_sizer->AddButton(new wxButton(this, wxID_APPLY, wxT("Apply")));
    buttons_sizer->AddButton(new wxButton(this, wxID_CANCEL, wxT("Cancel")));
    buttons_sizer->Realize();
    m_main_sizer->Add(buttons_sizer, 0, wxALIGN_RIGHT | wxALL, 5);

    m_main_sizer->SetSizeHints(this);
    SetSizer(m_main_sizer);
    Layout();
    Centre();
}

void vector_layer_settings_control::update()
{
    m_colourPickerPoints->SetColour(m_parent->layers()[m_index]->point_color());
    m_sliderWidthPoints->SetValue(m_parent->layers()[m_index]->point_width());

    m_colourPickerLines->SetColour(m_parent->layers()[m_index]->line_color());
    m_sliderWidthLines->SetValue(m_parent->layers()[m_index]->line_width());
    m_choiceLines->SetSelection( wxhelper::from_wx_style_to_selection_index(m_parent->layers()[m_index]->line_style()) );

    m_colourPickerRingsPolygons->SetColour(m_parent->layers()[m_index]->polygon_border_color());
    m_colourPickerInsidePolygons->SetColour(m_parent->layers()[m_index]->polygon_inner_color());
    m_sliderWidthRings->SetValue(m_parent->layers()[m_index]->polygon_border_width());
    m_choicePolygons->SetSelection( wxhelper::from_wx_style_to_selection_index(m_parent->layers()[m_index]->polygon_inner_style()) );

    // Text visibility
    m_checkShowTexts->SetValue(m_parent->layers()[m_index]->text_visibility());
    m_colourPickerTexts->SetColour(m_parent->layers()[m_index]->text_color());
}

void vector_layer_settings_control::on_ok_button(wxCommandEvent &event)
{
    on_apply_button(event);
    Hide();
}

void vector_layer_settings_control::on_cancel_button(wxCommandEvent &event)
{
    Hide();
}

void vector_layer_settings_control::on_apply_button(wxCommandEvent &event)
{
    // Texts
    // La, on appelle l'update (via le callback notifier ...) !
    m_parent->layers()[m_index]->text_visibility(m_checkShowTexts->GetValue(),    false);
    m_parent->layers()[m_index]->text_color(m_colourPickerTexts->GetColour(), false);

    m_parent->layers()[m_index]->point_color(m_colourPickerPoints->GetColour(), false);
    m_parent->layers()[m_index]->point_width(m_sliderWidthPoints->GetValue(),   false);

    m_parent->layers()[m_index]->line_color(m_colourPickerLines->GetColour(),          false);
    m_parent->layers()[m_index]->line_width(m_sliderWidthLines->GetValue(),            false);
    m_parent->layers()[m_index]->line_style(style_lines[m_choiceLines->GetSelection()],false);

    m_parent->layers()[m_index]->polygon_border_color(m_colourPickerRingsPolygons->GetColour(),              false);
    m_parent->layers()[m_index]->polygon_inner_color(m_colourPickerInsidePolygons->GetColour(),              false);
    m_parent->layers()[m_index]->polygon_border_width(m_sliderWidthRings->GetValue(),                        false);
    m_parent->layers()[m_index]->polygon_inner_style(style_inside_polygons[m_choicePolygons->GetSelection()],false);

    // On refresh ...
    m_parent->layers()[m_index]->needs_update(true);
    m_parent->panelviewer()->Refresh();
}

void vector_layer_settings_control::on_close_window(wxCloseEvent& event)
{
    Hide();
}

const char** vector_layer_settings_control::icon_xpm() const {return polygon_icon_xpm;}
