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

#ifndef APPLICATIONSETTINGS_HPP_
#define APPLICATIONSETTINGS_HPP_

//#include <wx/clrpicker.h>
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

class ApplicationSettings: public wxDialog
{
public:
	ApplicationSettings(wxWindow *parent = (wxWindow *) NULL, wxWindowID id = wxID_ANY, const wxString& title = _("Application settings"), long style = wxDEFAULT_FRAME_STYLE, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	virtual ~ApplicationSettings();

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
    wxSlider* m_sliderAlphaPolygons;
    wxSlider* m_sliderWidthLines;
    wxSlider* m_sliderWidthRings;
	wxChoice* m_choicePoints;
	wxChoice* m_choicePolygons;
	wxChoice* m_choiceLines;
	wxChoice* m_choiceLabels;
};

#endif /* APPLICATIONSETTINGS_HPP_ */
