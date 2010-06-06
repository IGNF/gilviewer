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

#ifndef APPLICATIONSETTINGS_HPP_
#define APPLICATIONSETTINGS_HPP_

#include <wx/dialog.h>

class wxPanel;
class wxBoxSizer;
class wxTextCtrl;
class wxImageList;
class wxBookCtrlBase;
class wxFlexGridSizer;
class wxDirPickerCtrl;
class wxColourPickerCtrl;
class wxFileDirPickerEvent;
class wxCheckBox;
class wxSlider;
class wxChoice;

class application_settings: public wxDialog
{
public:
        application_settings(wxWindow *parent = (wxWindow *) NULL, wxWindowID id = wxID_ANY, const wxString& title = _("Application settings"), long style = wxDEFAULT_FRAME_STYLE, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
        virtual ~application_settings();

	void OnCloseWindow(wxCloseEvent& event);
	void OnApplyButton(wxCommandEvent &event);

	void WriteConfig();

	DECLARE_EVENT_TABLE();

private:
	wxPanel *m_panel;
	wxBoxSizer *m_sizerFrame;
	wxBookCtrlBase *m_bookCtrl;

	wxImageList *m_imageList;

	wxDirPickerCtrl *dirPickerLUT;
	wxDirPickerCtrl *dirPickerWD;
	wxDirPickerCtrl *dirPickerPlugins;

	wxCheckBox *m_checkBoxLoadWholeImage;
	wxCheckBox *m_checkBoxBilinearZoom;
	bool m_loadWholeImage;
	bool m_bilinearZoom;

	wxTextCtrl* m_textZoom;
	wxTextCtrl* m_textDezoom;
	double zoom_;
	double deZoom_;
	wxTextCtrl* m_textFontSize;
	double fontSize_;

	wxPanel* CreatePathsSettingsPanel();
	wxPanel* CreateOptionsSettingsPanel();
	wxPanel* CreateVetorLayerSettingsPanel();

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
};

typedef application_settings ApplicationSettings;

#endif /* APPLICATIONSETTINGS_HPP_ */
