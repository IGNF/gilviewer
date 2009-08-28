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

#ifndef __VECTOR_LAYER_SETTINGS_CONTROL_HPP__
#define __VECTOR_LAYER_SETTINGS_CONTROL_HPP__

class wxSlider;
class wxChoice;
class wxCheckBox;
class wxFlexGridSizer;
class wxFontPickerCtrl;
class wxColourPickerCtrl;

#include "gui/LayerSettingsControl.hpp"

class VectorLayerSettingsControl : public LayerSettingsControl
{
public:
	/// Constructeur
	/// @param index L'index du layer dans le LayerControl (lors d'un ajout, on lui donne l'index egal au nombre de layers)
	/// @param parent ...
	/// @param id ...
	/// @param title ...
	VectorLayerSettingsControl(unsigned int index, LayerControl *parent , wxWindowID id = wxID_ANY, const wxString& title = _("Vector layer settings"),
		long style = wxDEFAULT_FRAME_STYLE , const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize );

	void OnOKButton(wxCommandEvent &event);
	void OnCancelButton(wxCommandEvent &event);
	void OnApplyButton(wxCommandEvent &event);
	void OnCloseWindow(wxCloseEvent& event);

	// Cette methode permet de mettre a jour l'interface lorsque des changements sont fait a partir du code (changement de style, de couleur ...)
	virtual void update();

    static wxString choices_points[6];
    static int style_points[6];
    static wxString choices_inside_polygons[8];
    static int style_inside_polygons[8];
    static wxString choices_lines[6];
    static int style_lines[6];
    static unsigned int FromWxStyleToSelectionIndex( unsigned int style );

//private:
	LayerControl *m_parent;

	wxFlexGridSizer *m_main_sizer;
    wxColourPickerCtrl* m_colourPickerPoints;
    wxColourPickerCtrl* m_colourPickerRingsPolygons;
	wxColourPickerCtrl* m_colourPickerInsidePolygons;
    wxColourPickerCtrl* m_colourPickerLines;
    wxSlider* m_sliderWidthPoints;
    wxSlider* m_sliderAlphaPolygons;
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
