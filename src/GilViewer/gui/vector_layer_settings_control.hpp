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

#ifndef __VECTOR_LAYER_SETTINGS_CONTROL_HPP__
#define __VECTOR_LAYER_SETTINGS_CONTROL_HPP__

#include "../gui/layer_settings_control.hpp"

class wxSlider;
class wxChoice;
class wxCheckBox;
class wxFlexGridSizer;
class wxFontPickerCtrl;
class wxColourPickerCtrl;

class vector_layer_settings_control : public layer_settings_control
{
public:
    /// Constructeur
    /// @param index L'index du layer dans le LayerControl (lors d'un ajout, on lui donne l'index egal au nombre de layers)
    /// @param parent ...
    /// @param id ...
    /// @param title ...
    vector_layer_settings_control(unsigned int index, layer_control *parent , wxWindowID id = wxID_ANY, const wxString& title = _("Vector layer settings"),
                                  long style = wxDEFAULT_FRAME_STYLE , const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize );

    void on_ok_button(wxCommandEvent &event);
    void on_cancel_button(wxCommandEvent &event);
    void on_apply_button(wxCommandEvent &event);
    void on_close_window(wxCloseEvent& event);

    // Cette methode permet de mettre a jour l'interface lorsque des changements sont fait a partir du code (changement de style, de couleur ...)
    virtual void update();

    static wxString choices_points[6];
    static int style_points[6];
    static wxString choices_inside_polygons[8];
    static int style_inside_polygons[8];
    static wxString choices_lines[6];
    static int style_lines[6];
    static unsigned int FromWxStyleToSelectionIndex( unsigned int style );

    virtual std::string info_button_name() const {return "APPLICATIONS-OTHERS_16x16";}
    virtual const char** icon_xpm() const;

    //private:
    layer_control *m_parent;

    wxFlexGridSizer *m_main_sizer;
    wxColourPickerCtrl* m_colourPickerPoints;
    wxColourPickerCtrl* m_colourPickerRingsPolygons;
    wxColourPickerCtrl* m_colourPickerInsidePolygons;
    wxColourPickerCtrl* m_colourPickerLines;
    wxSlider* m_sliderWidthPoints;
    wxSlider* m_sliderWidthLines;
    wxSlider* m_sliderWidthRings;
    wxChoice* m_choicePoints;
    wxChoice* m_choicePolygons;
    wxChoice* m_choiceLines;
    wxChoice* m_choiceLabels;

    // Texts
    wxColourPickerCtrl* m_colourPickerTexts;
    wxFontPickerCtrl* m_fontPickerTexts;
    wxCheckBox* m_checkShowTexts;

    DECLARE_EVENT_TABLE()
        };

#endif // __VECTOR_LAYER_SETTINGS_CONTROL_HPP__
