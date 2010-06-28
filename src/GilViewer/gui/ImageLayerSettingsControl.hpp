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

#ifndef __IMAGE_LAYER_SETTINGS_CONTROL_HPP__
#define __IMAGE_LAYER_SETTINGS_CONTROL_HPP__

#include <vector>
#include <wx/panel.h>

#include "../gui/LayerSettingsControl.hpp"

class histogram_plotter;
class layer_control;
class wxSlider;
class wxTextCtrl;
class wxCheckBox;
class wxFilePickerCtrl;
class wxFlexGridSizer;

class image_layer_settings_control : public layer_settings_control
{
public:
    image_layer_settings_control(unsigned int index, layer_control *parent, wxWindowID id = wxID_ANY, const wxString& title = _("Image layer settings"),
                                 long style = wxDEFAULT_FRAME_STYLE , const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize );

    void on_ok_button(wxCommandEvent &event);
    void on_cancel_button(wxCommandEvent &event);
    void on_apply_button(wxCommandEvent &event);
    void on_close_window(wxCloseEvent& event);
    void on_check_alpha_range(wxCommandEvent &event);
    void on_check_alpha_channel(wxCommandEvent &event);
    void on_get_focus(wxFocusEvent &event);

    wxSlider* alpha_slider() const {return m_alphaSlider;}

    layer_control* layercontrol() { return m_parent; }
    histogram_plotter* histogramplotter() { return m_histogramPanel; }

    // Cette methode permet de mettre a jour l'interface lorsque des changements sont fait a partir du code (changement de style, de couleur ...)
    virtual void update();

    virtual std::string info_button_name() const {return "APPLICATIONS-GRAPHICS_16x16";}
    virtual const char** icon_xpm() const;

protected:
    wxFlexGridSizer *m_main_sizer;
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

protected:
    histogram_plotter *m_histogramPanel;
    layer_control *m_parent;
    bool IsNumeric( const std::string& value );
};

class histogram_plotter : public wxPanel
{
public:
    histogram_plotter(image_layer_settings_control* parent, const unsigned int redChannel, const unsigned int greenChannel, const unsigned int blueChannel, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL);
    virtual ~histogram_plotter() {;}

    inline std::vector< std::vector< double > > &Data() { return m_histogram; }
    double &Min(){ return m_min; }
    double &Max(){ return m_max; }

    void on_paint(wxPaintEvent &event);
    void on_size(wxSizeEvent &event);
    void on_mouse_move(wxMouseEvent &event);

    bool init() { return m_isInit; }
    void init(const bool init) {m_isInit=init;}
    void channels(const unsigned int red, const unsigned int green, const unsigned int blue) { m_redChannel=red; m_greenChannel=green; m_blueChannel=blue; }

    DECLARE_EVENT_TABLE();

private:
    image_layer_settings_control* m_parent;
    std::vector< std::vector< double > > m_histogram;
    double m_min, m_max;
    bool m_isInit;
    unsigned int m_redChannel, m_greenChannel, m_blueChannel;

};

class thread_histogram : public wxThread
{
public:
    thread_histogram(image_layer_settings_control *parent);

    // thread execution starts here
    virtual void *Entry();

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    virtual void OnExit();

public:
    size_t   m_count;
    image_layer_settings_control *m_parent;
};

typedef image_layer_settings_control ImageLayerSettingsControl;
typedef histogram_plotter histogram_plotter;
typedef thread_histogram thread_histogram;

#endif // __IMAGE_LAYER_SETTINGS_CONTROL_HPP__
