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

#include <wx/icon.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/config.h>
#include <wx/msgdlg.h>
#include <wx/artprov.h>
#include <wx/imaglist.h>
#include <wx/textctrl.h>
#include <wx/toolbook.h>
#include <wx/notebook.h>
#include <wx/clrpicker.h>
#include <wx/filepicker.h>
#include <wx/checkbox.h>
#include <wx/slider.h>
#include <wx/choice.h>

#include "gui/ApplicationSettings.hpp"
#include "gui/VectorLayerSettingsControl.hpp"

#include "convenient/wxhelper.hpp"



BEGIN_EVENT_TABLE(ApplicationSettings, wxDialog)
EVT_CLOSE(ApplicationSettings::OnCloseWindow)
EVT_BUTTON(wxID_APPLY,ApplicationSettings::OnApplyButton)
END_EVENT_TABLE()

ApplicationSettings::ApplicationSettings(wxWindow *parent, wxWindowID id, const wxString& title, long style, const wxPoint& pos, const wxSize& size) :
	wxDialog(parent, id, title, pos, size, style)
{
        const wxSize imageSize(24, 24);

        SetIcon(wxArtProvider::GetIcon(wxART_EXECUTABLE_FILE, wxART_FRAME_ICON, imageSize));

	m_imageList = new wxImageList(imageSize.GetWidth(), imageSize.GetHeight());
	m_imageList-> Add(wxArtProvider::GetIcon(wxART_FOLDER, wxART_TOOLBAR, imageSize));
	m_imageList-> Add(wxArtProvider::GetIcon(wxART_EXECUTABLE_FILE, wxART_TOOLBAR, imageSize));
	m_imageList-> Add(wxArtProvider::GetIcon(wxART_HELP_SETTINGS, wxART_OTHER, imageSize));
	//	m_imageList-> Add(wxArtProvider::GetIcon(wxART_HELP_SIDE_PANEL , wxART_OTHER, imageSize));
	//	m_imageList-> Add(wxArtProvider::GetIcon(wxART_HELP_BOOK, wxART_OTHER, imageSize));
	//	m_imageList-> Add(wxArtProvider::GetIcon(wxART_HELP_FOLDER, wxART_OTHER, imageSize));
	//	m_imageList-> Add(wxArtProvider::GetIcon(wxART_TIP, wxART_OTHER, imageSize));

	m_panel = new wxPanel(this);
	m_sizerFrame = new wxBoxSizer(wxVERTICAL);
	m_bookCtrl = new wxToolbook(m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);

	m_panel->SetSizer(m_sizerFrame);
	m_bookCtrl->SetImageList(m_imageList);

	m_bookCtrl->AddPage(CreatePathsSettingsPanel(), _(""), false, 0);
	m_bookCtrl->AddPage(CreateOptionsSettingsPanel(), _(""), false, 1);
	m_bookCtrl->AddPage(CreateVetorLayerSettingsPanel(), _(""), false, 2);

	m_sizerFrame->Insert(0, m_bookCtrl, wxSizerFlags(5).Expand().Border());
	m_sizerFrame->Show(m_bookCtrl);
	m_sizerFrame->Layout();

    //SetSizer(m_sizerFrame);
    //SetAutoLayout(true);
    Layout();

	// Lecture de la configuration des differentes options ...
	wxConfigBase *pConfig = wxConfigBase::Get();
	if (pConfig == NULL)
		return;

	// Paths
	wxString str;
	pConfig->Read(_("/Paths/WorkingDirectory"), &str, ::wxGetCwd());
	::wxSetWorkingDirectory(str);

	// Options settings
	pConfig->Read(_("/Options/Zoom"), &zoom_, 0.5);
	pConfig->Read(_("/Options/Dezoom"), &deZoom_, 2.);
	pConfig->Read(_("/Options/LoadWoleImage"), &m_loadWholeImage, true);
	pConfig->Read(_("/Options/BilinearZoom"), &m_bilinearZoom, false);
}


ApplicationSettings::~ApplicationSettings()
{
	WriteConfig();
}

wxPanel* ApplicationSettings::CreatePathsSettingsPanel()
{
	wxPanel *panel = new wxPanel(m_bookCtrl, wxID_ANY);
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxConfigBase *pConfig = wxConfigBase::Get();
	wxString str;

	// LUT
	pConfig->Read(_("/Paths/LUT"), &str, ::wxGetCwd());
	wxStaticBoxSizer *boxSizerLUT = new wxStaticBoxSizer(wxHORIZONTAL, panel, _("LUT directory"));
	dirPickerLUT = new wxDirPickerCtrl(panel, wxID_ANY, str, _("Select LUT directory"), wxDefaultPosition, wxDefaultSize, /*wxDIRP_USE_TEXTCTRL | */wxDIRP_DIR_MUST_EXIST);
	boxSizerLUT->Add(dirPickerLUT, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL, 5);
	mainSizer->Add(boxSizerLUT, 0, wxEXPAND | wxHORIZONTAL, 5);

	// Working directory
	pConfig->Read(_("/Paths/WorkingDirectory"), &str, ::wxGetCwd());
	wxStaticBoxSizer *boxSizerWD = new wxStaticBoxSizer(wxHORIZONTAL, panel, _("Working directory"));
	dirPickerWD = new wxDirPickerCtrl(panel, wxID_ANY, str, _("Select working directory"), wxDefaultPosition, wxDefaultSize, /*wxDIRP_USE_TEXTCTRL | */wxDIRP_DIR_MUST_EXIST);
	boxSizerWD->Add(dirPickerWD, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL, 5);
	mainSizer->Add(boxSizerWD, 0, wxEXPAND | wxHORIZONTAL, 5);

	// Plugins directory
	pConfig->Read(_("/Paths/Plugins"), &str, ::wxGetCwd());
	wxStaticBoxSizer *boxSizerPlugins = new wxStaticBoxSizer(wxHORIZONTAL, panel, _("Plugins directory"));
	dirPickerPlugins = new wxDirPickerCtrl(panel, wxID_ANY, str, _("Select plugins directory"), wxDefaultPosition, wxDefaultSize, /*wxDIRP_USE_TEXTCTRL | */wxDIRP_DIR_MUST_EXIST);
	boxSizerPlugins->Add(dirPickerPlugins, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL, 5);
	mainSizer->Add(boxSizerPlugins, 0, wxEXPAND | wxHORIZONTAL, 5);


	mainSizer->Add(new wxButton(panel, wxID_APPLY, _("Apply")), 0, wxALIGN_CENTER_HORIZONTAL, 5);

	mainSizer->SetSizeHints(panel);
	panel->SetSizer(mainSizer);
	panel->Layout();

	return panel;
}

wxPanel* ApplicationSettings::CreateOptionsSettingsPanel()
{
	wxPanel *panel = new wxPanel(m_bookCtrl, wxID_ANY);
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxConfigBase *pConfig = wxConfigBase::Get();
	wxString str;

	/////ZOOM / DEZOOM
	wxStaticBoxSizer *boxSizerZoom = new wxStaticBoxSizer(wxHORIZONTAL, panel, _("Zoom - dezoom"));
	wxBoxSizer *zoomSizer = new wxBoxSizer(wxHORIZONTAL);
	// Zoom
	pConfig->Read(_("/Options/Zoom"), &zoom_, 0.5);
	str << zoom_;
	m_textZoom = new wxTextCtrl(panel, wxID_ANY, str);
	zoomSizer->Add(m_textZoom, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL, 5);
	// Dezoom
	pConfig->Read(_("/Options/Dezoom"), &deZoom_, 2.);
	str.Clear();
	str << deZoom_;
	m_textDezoom = new wxTextCtrl(panel, wxID_ANY, str);
	zoomSizer->Add(m_textDezoom, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL, 5);

	boxSizerZoom->Add(zoomSizer, 1, wxEXPAND | wxHORIZONTAL, 5);

	///// Font de l'applications
	wxStaticBoxSizer *boxSizerFonts = new wxStaticBoxSizer(wxHORIZONTAL, panel, _("Fonts"));
	pConfig->Read(_("/Options/FontSize"), &fontSize_, 8);
	str.Clear();
	str << fontSize_;
	m_textFontSize = new wxTextCtrl(panel, wxID_ANY, str);

	boxSizerFonts->Add(m_textFontSize, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL, 5);

	///////LOAD WHOLE IMAGE
	wxStaticBoxSizer *boxSizerPerformance = new wxStaticBoxSizer(wxHORIZONTAL, panel, _("Performances"));
	m_checkBoxLoadWholeImage = new wxCheckBox(panel, wxID_ANY, _("Load whole image (Warning : you need GDAL !!)"));
	pConfig->Read(_("/Options/LoadWoleImage"), &m_loadWholeImage, false);
	m_checkBoxLoadWholeImage->SetValue(m_loadWholeImage);

	boxSizerPerformance->Add(m_checkBoxLoadWholeImage, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL, 5);

	///////Bilinear zoom
	wxStaticBoxSizer *boxSizerBilinearZoom = new wxStaticBoxSizer(wxHORIZONTAL, panel, _("Use NN or bilinear zoom"));
	m_checkBoxBilinearZoom = new wxCheckBox(panel, wxID_ANY, _("bilinear"));
	pConfig->Read(_("/Options/BilinearZoom"), &m_bilinearZoom, false);
	m_checkBoxBilinearZoom->SetValue(m_bilinearZoom);

	boxSizerBilinearZoom->Add(m_checkBoxBilinearZoom, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL, 5);



	mainSizer->Add(boxSizerZoom, 0, wxEXPAND | wxHORIZONTAL, 5);
	mainSizer->Add(boxSizerFonts, 0, wxEXPAND | wxHORIZONTAL, 5);
	mainSizer->Add(boxSizerPerformance, 0, wxEXPAND | wxHORIZONTAL, 5);
	mainSizer->Add(boxSizerBilinearZoom, 0, wxEXPAND | wxHORIZONTAL, 5);

	mainSizer->Add(new wxButton(panel, wxID_APPLY, _("Apply")), 0, wxALIGN_CENTER_HORIZONTAL, 5);

	mainSizer->SetSizeHints(panel);
	panel->SetSizer(mainSizer);
	panel->Layout();

	return panel;
}


wxPanel* ApplicationSettings::CreateVetorLayerSettingsPanel()
{
	wxPanel *panel = new wxPanel(m_bookCtrl, wxID_ANY);
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxConfigBase *pConfig = wxConfigBase::Get();

	////////////////////////
	///
	///  Points settings
	///
	////////////////////////
	wxStaticBoxSizer *boxSizerPoints = new wxStaticBoxSizer(wxHORIZONTAL, panel, _("Points settings"));
	m_colourPickerPoints = new wxColourPickerCtrl(panel, wxID_ANY, *wxRED, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE | wxCLRP_USE_TEXTCTRL);
	wxStaticBoxSizer *boxSizerWidthPoints = new wxStaticBoxSizer(wxVERTICAL, panel, _("Width"));
	m_sliderWidthPoints = new wxSlider(panel, wxID_ANY, 3, 1, 10, wxDefaultPosition, wxSize(100, 40), wxSL_HORIZONTAL | wxSL_LABELS);
	boxSizerWidthPoints->Add(m_sliderWidthPoints, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	boxSizerPoints->Add(m_colourPickerPoints, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	boxSizerPoints->Add(boxSizerWidthPoints, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	m_choicePoints = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, WXSIZEOF(VectorLayerSettingsControl::choices_points), VectorLayerSettingsControl::choices_points);
	m_choicePoints->SetSelection(0);
	m_choicePoints->Enable(false);
	boxSizerPoints->Add(m_choicePoints, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	if ( pConfig )
	{
		int red, green, blue, width;
		pConfig->Read(_("/Options/VectorLayerPoint/Color/Red"), &red, 255);
		pConfig->Read(_("/Options/VectorLayerPoint/Color/Green"), &green, 0);
		pConfig->Read(_("/Options/VectorLayerPoint/Color/Blue"), &blue, 0);
		pConfig->Read(_("/Options/VectorLayerPoint/Width"), &width, 3);
		m_colourPickerPoints->SetColour( wxColour(red,green,blue) );
		m_sliderWidthPoints->SetValue( width );
	}

	////////////////////////
	///
	///  Lines settings
	///
	////////////////////////
	wxStaticBoxSizer *boxSizerLines = new wxStaticBoxSizer(wxHORIZONTAL, panel, _("Lines settings"));
	m_colourPickerLines = new wxColourPickerCtrl(panel, wxID_ANY, *wxRED, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE | wxCLRP_USE_TEXTCTRL);
	wxStaticBoxSizer *boxSizerWidthLines = new wxStaticBoxSizer(wxVERTICAL, panel, _("Width"));
	m_sliderWidthLines = new wxSlider(panel, wxID_ANY, 1, 1, 10, wxDefaultPosition, wxSize(100, 40), wxSL_HORIZONTAL | wxSL_LABELS);
	boxSizerWidthLines->Add(m_sliderWidthLines, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	boxSizerLines->Add(m_colourPickerLines, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	boxSizerLines->Add(boxSizerWidthLines, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	m_choiceLines = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, WXSIZEOF(VectorLayerSettingsControl::choices_lines), VectorLayerSettingsControl::choices_lines);
	m_choiceLines->SetSelection(0);
	boxSizerLines->Add(m_choiceLines, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	if ( pConfig )
	{
		int redLine = 255, blueLine = 0, greenLine = 0;
		int widthLine = 3;
		int styleLine = wxSOLID;
		pConfig->Read(_T("/Options/VectorLayerArc/Color/Red"), &redLine, 255);
		pConfig->Read(_T("/Options/VectorLayerArc/Color/Green"), &greenLine, 0);
		pConfig->Read(_T("/Options/VectorLayerArc/Color/Blue"), &blueLine, 0);
		m_colourPickerLines->SetColour( wxColour(redLine,greenLine,blueLine) );
		pConfig->Read(_T("/Options/VectorLayerArc/Width"), &widthLine, 3);
		m_sliderWidthLines->SetValue( widthLine );
		pConfig->Read(_T("/Options/VectorLayerArc/Style"), &styleLine, wxSOLID);
		m_choiceLines->SetSelection( wxhelper::FromWxStyleToSelectionIndex(styleLine) );
	}

	////////////////////////
	///
	///  Polygons settings
	///
	////////////////////////
	wxStaticBoxSizer *boxSizerPolygons = new wxStaticBoxSizer(wxHORIZONTAL, panel, _("Polygons settings"));
	wxBoxSizer *boxColorPickersPolygons = new wxBoxSizer(wxVERTICAL);
	wxStaticBoxSizer *boxSizerRingsColour = new wxStaticBoxSizer(wxHORIZONTAL, panel, _("Rings colour"));
	wxStaticBoxSizer *boxSizerInsideColour = new wxStaticBoxSizer(wxHORIZONTAL, panel, _("Inside colour"));
	m_colourPickerRingsPolygons = new wxColourPickerCtrl(panel, wxID_ANY, *wxRED, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE | wxCLRP_USE_TEXTCTRL);
	m_colourPickerInsidePolygons = new wxColourPickerCtrl(panel, wxID_ANY, *wxBLUE, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE | wxCLRP_USE_TEXTCTRL);
	boxSizerRingsColour->Add(m_colourPickerRingsPolygons, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	boxSizerInsideColour->Add(m_colourPickerInsidePolygons, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	boxColorPickersPolygons->Add(boxSizerRingsColour, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	boxColorPickersPolygons->Add(boxSizerInsideColour, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	wxBoxSizer *boxSlidersPolygons = new wxBoxSizer(wxVERTICAL);
	wxStaticBoxSizer *boxSizerRingsWidth = new wxStaticBoxSizer(wxHORIZONTAL, panel, _("Rings width"));
#if defined __WXMAC_OSX__
	wxStaticBoxSizer *boxSizerAlphaPolygons = new wxStaticBoxSizer(wxVERTICAL, panel, _("Alpha"));
	m_sliderAlphaPolygons = new wxSlider(panel, wxID_ANY, 0, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);
	m_sliderAlphaPolygons->Enable(false);
	m_sliderAlphaPolygons->SetToolTip(_("Tu veux de l'alpha ? Passe sous OSX !"));
	boxSizerAlphaPolygons->Add(m_sliderAlphaPolygons, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	boxSlidersPolygons->Add(boxSizerAlphaPolygons, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
#endif // __WXMAC_OSX__
	// Ring width
	m_sliderWidthRings = new wxSlider(panel, wxID_ANY, 1, 1, 10, wxDefaultPosition, wxSize(100, 40), wxSL_HORIZONTAL | wxSL_LABELS);
	boxSizerRingsWidth->Add(m_sliderWidthRings, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	boxSlidersPolygons->Add(boxSizerRingsWidth, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	boxSizerPolygons->Add(boxColorPickersPolygons, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	boxSizerPolygons->Add(boxSlidersPolygons, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	m_choicePolygons = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, WXSIZEOF(VectorLayerSettingsControl::choices_inside_polygons), VectorLayerSettingsControl::choices_inside_polygons);
	m_choicePolygons->SetSelection(0);
	boxSizerPolygons->Add(m_choicePolygons, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	if ( pConfig )
	{
		int red, green, blue, width;
		int penStyle, brushStyle;
		pConfig->Read(_("/Options/VectorLayerPolygon/Ring/Color/Red"), &red, 255);
		pConfig->Read(_("/Options/VectorLayerPolygon/Ring/Color/Green"), &green, 0);
		pConfig->Read(_("/Options/VectorLayerPolygon/Ring/Color/Blue"), &blue, 0);
		pConfig->Read(_("/Options/VectorLayerPolygon/Ring/Width"), &width, 1);
		m_colourPickerRingsPolygons->SetColour( wxColour(red,green,blue) );
		m_sliderWidthRings->SetValue( width );
		pConfig->Read(_("/Options/VectorLayerPolygon/Shape/Color/Red"), &red, 255);
		pConfig->Read(_("/Options/VectorLayerPolygon/Shape/Color/Green"), &green, 0);
		pConfig->Read(_("/Options/VectorLayerPolygon/Shape/Color/Blue"), &blue, 0);
		m_colourPickerInsidePolygons->SetColour( wxColour(red,green,blue) );
		pConfig->Read(_("/Options/VectorLayerPolygon/Style/Pen"), &penStyle, wxSOLID);
		pConfig->Read(_("/Options/VectorLayerPolygon/Style/Brush"), &brushStyle, wxSOLID);
		m_choicePolygons->SetSelection( wxhelper::FromWxStyleToSelectionIndex(brushStyle) );
	}

	mainSizer->Add(boxSizerPoints, 0, wxEXPAND | wxHORIZONTAL, 5);
	mainSizer->Add(boxSizerLines, 0, wxEXPAND | wxHORIZONTAL, 5);
	mainSizer->Add(boxSizerPolygons, 0, wxEXPAND | wxHORIZONTAL, 5);

	mainSizer->Add(new wxButton(panel, wxID_APPLY, _("Apply")), 0, wxALIGN_CENTER_HORIZONTAL, 5);

	mainSizer->SetSizeHints(panel);
	panel->SetSizer(mainSizer);
	panel->Layout();

	return panel;
}

void ApplicationSettings::OnCloseWindow(wxCloseEvent& event)
{
	Hide();
}

void ApplicationSettings::OnApplyButton(wxCommandEvent &event)
{
	WriteConfig();
}

void ApplicationSettings::WriteConfig()
{
	wxConfigBase *pConfig = wxConfigBase::Get();
	if (pConfig == NULL)
		return;

	// Paths settings
	pConfig->Write(_("/Paths/LUT"), dirPickerLUT->GetPath());
	pConfig->Write(_("/Paths/WorkingDirectory"), dirPickerWD->GetPath());
	pConfig->Write(_("/Paths/Plugins"), dirPickerPlugins->GetPath());

	// Options settings
	m_textZoom->GetValue().ToDouble(&zoom_);
	m_textDezoom->GetValue().ToDouble(&deZoom_);
	pConfig->Write(_("/Options/Zoom"), zoom_);
	pConfig->Write(_("/Options/Dezoom"), deZoom_);
	m_textFontSize->GetValue().ToDouble(&fontSize_);
	pConfig->Write(_("/Options/FontSize"), fontSize_);
	m_loadWholeImage = m_checkBoxLoadWholeImage->GetValue();
	pConfig->Write(_("/Options/LoadWoleImage"), m_loadWholeImage);
	m_bilinearZoom = m_checkBoxBilinearZoom->GetValue();
	pConfig->Write(_("/Options/BilinearZoom"), m_bilinearZoom);

	// Vector layers
	pConfig->Write(_("/Options/VectorLayerPoint/Color/Red"), m_colourPickerPoints->GetColour().Red());
	pConfig->Write(_("/Options/VectorLayerPoint/Color/Green"), m_colourPickerPoints->GetColour().Green());
	pConfig->Write(_("/Options/VectorLayerPoint/Color/Blue"), m_colourPickerPoints->GetColour().Blue());
	pConfig->Write(_("/Options/VectorLayerPoint/Width"), m_sliderWidthPoints->GetValue());

	pConfig->Write(_("/Options/VectorLayerLine/Color/Red"), m_colourPickerLines->GetColour().Red());
	pConfig->Write(_("/Options/VectorLayerLine/Color/Green"), m_colourPickerLines->GetColour().Green());
	pConfig->Write(_("/Options/VectorLayerLine/Color/Blue"), m_colourPickerLines->GetColour().Blue());
	pConfig->Write(_("/Options/VectorLayerLine/Width"), m_sliderWidthLines->GetValue());
	pConfig->Write(_("/Options/VectorLayerLine/Style"), wxhelper::FromLineSelectionIndexToWxStyle(m_choiceLines->GetSelection()) );

	pConfig->Write(_("/Options/VectorLayerPolygon/Ring/Color/Red"), m_colourPickerRingsPolygons->GetColour().Red());
	pConfig->Write(_("/Options/VectorLayerPolygon/Ring/Color/Green"), m_colourPickerRingsPolygons->GetColour().Green());
	pConfig->Write(_("/Options/VectorLayerPolygon/Ring/Color/Blue"), m_colourPickerRingsPolygons->GetColour().Blue());
	pConfig->Write(_("/Options/VectorLayerPolygon/Ring/Width"), m_sliderWidthRings->GetValue());
	pConfig->Write(_("/Options/VectorLayerPolygon/Shape/Color/Red"), m_colourPickerInsidePolygons->GetColour().Red());
	pConfig->Write(_("/Options/VectorLayerPolygon/Shape/Color/Green"), m_colourPickerInsidePolygons->GetColour().Green());
	pConfig->Write(_("/Options/VectorLayerPolygon/Shape/Color/Blue"), m_colourPickerInsidePolygons->GetColour().Blue());
	pConfig->Write(_("/Options/VectorLayerPolygon/Style/Pen"), wxSOLID); // pour l'instant, le choix n'est pas possible, donc on le laisse en dur
	pConfig->Write(_("/Options/VectorLayerPolygon/Style/Brush"), wxhelper::FromPolygonSelectionIndexToWxStyle(m_choicePolygons->GetSelection()) );
}
