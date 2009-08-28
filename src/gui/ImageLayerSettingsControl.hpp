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

#ifndef __IMAGE_LAYER_SETTINGS_CONTROL_HPP__
#define __IMAGE_LAYER_SETTINGS_CONTROL_HPP__

#include <vector>
//#include <wx/filepicker.h>
#include <wx/panel.h>

#include "gui/LayerSettingsControl.hpp"

class HistogramPlotter;
class LayerControl;
class wxSlider;
class wxTextCtrl;
class wxCheckBox;
class wxFilePickerCtrl;

class ImageLayerSettingsControl : public LayerSettingsControl
{
public:
	ImageLayerSettingsControl(unsigned int index, LayerControl *parent, wxWindowID id = wxID_ANY, const wxString& title = _("Image layer settings"),
		long style = wxDEFAULT_FRAME_STYLE , const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize );

	void OnScroll(wxScrollEvent &event);
	void OnOKButton(wxCommandEvent &event);
	void OnCancelButton(wxCommandEvent &event);
	void OnApplyButton(wxCommandEvent &event);
	void OnSize(wxSizeEvent &event);
	void OnCloseWindow(wxCloseEvent& event);
	void OnCheckAlphaRange(wxCommandEvent &event);
	void OnCheckAlphaChannel(wxCommandEvent &event);
	void OnGetFocus(wxFocusEvent &event);

	wxSlider* AlphaSlider() const {return m_alphaSlider;}

	LayerControl* GetLayerControl() { return m_parent; }
	HistogramPlotter* GetHistogramPlotter() { return m_histogramPanel; }

	// Cette methode permet de mettre a jour l'interface lorsque des changements sont fait a partir du code (changement de style, de couleur ...)
	virtual void update();

//private:
	//wxSlider *m_minimumSlider;
	//wxSlider *m_maximumSlider;
	wxSlider *m_alphaSlider;
	//wxSlider *m_gammaSlider;

	wxTextCtrl* m_textRedChannel;
	wxTextCtrl* m_textRedMinimumIntensity;
	wxTextCtrl* m_textRedMaximumIntensity;
	wxTextCtrl* m_textRedGamma;
	wxTextCtrl* m_textGreenChannel;
	wxTextCtrl* m_textGreenMinimumIntensity;
	wxTextCtrl* m_textGreenMaximumIntensity;
	wxTextCtrl* m_textGreenGamma;
	wxTextCtrl* m_textBlueChannel;
	wxTextCtrl* m_textBlueMinimumIntensity;
	wxTextCtrl* m_textBlueMaximumIntensity;
	wxTextCtrl* m_textBlueGamma;
	wxTextCtrl* m_textAlphaRangeMin;
	wxTextCtrl* m_textAlphaRangeMax;
	wxTextCtrl* m_textAlphaChannel;

	wxCheckBox* m_checkAlphaRange;
	wxCheckBox* m_checkAlphaChannel;

	//Fichier de Color Lookup Table pour les images mono canal
	wxFilePickerCtrl *m_filePicker_CLUT;

	unsigned int m_nbComponents;

	DECLARE_EVENT_TABLE();
private:
	HistogramPlotter *m_histogramPanel;

	LayerControl *m_parent;

	bool IsNumeric( const std::string& value );
};

class HistogramPlotter : public wxPanel
{
public:
	HistogramPlotter(ImageLayerSettingsControl* parent, const unsigned int redChannel, const unsigned int greenChannel, const unsigned int blueChannel, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL);
	virtual ~HistogramPlotter() {;}

    inline std::vector< std::vector< double > > &Data() { return m_histogram; }
    double &Min(){ return m_min; }
    double &Max(){ return m_max; }

	void OnPaint(wxPaintEvent &event);
	void OnSize(wxSizeEvent &event);
	void OnMouseMove(wxMouseEvent &event);
	bool IsInit() { return m_isInit; }
	void IsInit(const bool init) {m_isInit=init;}
	void SetChannels(const unsigned int red, const unsigned int green, const unsigned int blue) { m_redChannel=red; m_greenChannel=green; m_blueChannel=blue; }

	DECLARE_EVENT_TABLE();

private:
	ImageLayerSettingsControl* m_parent;
	std::vector< std::vector< double > > m_histogram;
	double m_min, m_max;
	bool m_isInit;
	unsigned int m_redChannel, m_greenChannel, m_blueChannel;

};

class ThreadHistogram : public wxThread
{
public:
	ThreadHistogram(ImageLayerSettingsControl *parent);

	// thread execution starts here
	virtual void *Entry();

	// called when the thread exits - whether it terminates normally or is
	// stopped with Delete() (but not when it is Kill()ed!)
	virtual void OnExit();

public:
	size_t   m_count;
	ImageLayerSettingsControl *m_parent;
};
#endif // __IMAGE_LAYER_SETTINGS_CONTROL_HPP__
