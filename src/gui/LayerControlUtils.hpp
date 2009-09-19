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

#ifndef __LAYER_CONTROL_UTILS_HPP__
#define __LAYER_CONTROL_UTILS_HPP__

/**
* @brief This headers contains some classes and structs used in the LayerControl class:
* <ul>
* 	<li>SelectableStaticText</li>
*   <li>paramBase</li>
*   <li>paramImageLayer</li>
*   <li>paramVectorLayer</li>
*   <li>LayerControlRow</li>
* <ul>
**/

#include <wx/stattext.h>

class wxCheckBox;
class wxBitmapButton;
class wxBoxSizer;

class LayerControl;
class LayerSettingsControl;

/**
* @brief Cette classe permet simplement de changer la couleur de fond d'un texte (wxStaticText) lorsque l'on clique dessus.
**/
class SelectableStaticText : public wxStaticText
{
public:
    /// Constructor
    /// @param parent Parent window
    /// @param layercontrol The LayerControl to whom the text is associated to
    /// @param id The window id
    /// @param label The text displayed by the control
    /// @param pos Control position
    /// @param size Control size
    /// @param style Control style
    /// @param name Control name
    SelectableStaticText(wxWindow* parent, LayerControl* layercontrol, wxWindowID id, const wxString& label, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxALIGN_LEFT, const wxString& name = _("staticText")) : wxStaticText(parent,id,label,pos,size, wxBG_STYLE_CUSTOM, name), m_parent(layercontrol), m_isSelected(false)
    {
	    //SetBackgroundStyle(wxBG_STYLE_CUSTOM);
        m_unselectedColour.Set( 100 , 50 , 50 );
        m_selectedColour.Set( 255 , 50 , 50 );
        SetForegroundColour( m_unselectedColour );
    };
    /// Destructor
    virtual ~SelectableStaticText() {};

    /// Getter for the selection flag
    bool IsSelected() const { return m_isSelected; }
    /// Setter for the selection flag
    void IsSelected(const bool isSelected) { m_isSelected = isSelected; if(m_isSelected) SetForegroundColour( m_selectedColour ); else SetForegroundColour( m_unselectedColour );}

    /// This methods enables to select a wxStaticText when the user clicks.
    void OnClick(wxMouseEvent& event);

private:
    /// Unselected text color (220,220,220)
    wxColour m_unselectedColour;
    /// Selected text color (120,120,220)
    wxColour m_selectedColour;
    /// The LayerControl to whom the text is associated to
    LayerControl* m_parent;
    /// Flag indicating if the text is selected
    bool m_isSelected;

    DECLARE_EVENT_TABLE();
};

struct paramBase
{
	std::string path;
	bool isVisible;
	bool isTransformable;
	double zoomFactor;
	double translationX;
	double translationY;
};

typedef struct paramImageLayer : paramBase
{
	unsigned char alpha;
	double gamma;
	double intensityMin;
	double intensityMax;
	bool isTransparent;
	double transparencyMin;
	double transparencyMax;
	unsigned int red;
	unsigned int green;
	unsigned int blue;
	bool useAlphaChannel;
	unsigned int alphaChannel;
} ImageLayerParameters;

typedef struct paramVectorLayer : paramBase
{
	wxColour pointsColor;
	unsigned int pointsWidth;
	wxColour linesColor;
	unsigned int linesWidth;
	int linesStyle;
	wxColour polygonsRingsColor;
	wxColour polygonsInsideColor;
	unsigned int polygonsRingsWidth;
	unsigned int polygonsRingsStyle;
	int polygonsInsideStyle;
} VectorLayerParameters;

class LayerControlRow
{
public:
	LayerControlRow( LayerControl* parent , const std::string &name , const unsigned int index , LayerSettingsControl *layersettings  , const std::string &tooltip="");
	~LayerControlRow(){}

	SelectableStaticText	*m_nameStaticText;
	wxCheckBox				*m_visibilityCheckBox;
	wxCheckBox				*m_transformationCheckBox;
	wxBitmapButton			*m_infoButton;
	wxBitmapButton			*m_saveButton;
	wxBitmapButton			*m_deleteButton;
	wxBitmapButton			*m_settingsButton;
	wxBoxSizer				*m_boxSizer;
	LayerControl			*m_parent;
	LayerSettingsControl	*m_layerSettingsControl;
	unsigned int			m_index;
};

#endif // __LAYER_CONTROL_UTILS_HPP__
