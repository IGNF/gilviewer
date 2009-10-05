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

#ifndef __LAYER_CONTROL_HPP__
#define __LAYER_CONTROL_HPP__

#include <vector>

#include <wx/stattext.h>
#include <wx/frame.h>

#include "layers/Layer.hpp"

class wxCheckBox;
class wxBitmapButton;
class wxBoxSizer;
class wxFlexGridSizer;
class wxToolBar;
class wxScrolledWindow;

class GlobalSettingsControl;
class LayerSettingsControl;
class VectorLayerGhost;
class Orientation2D;
class LayerControl;
class PanelViewer;
class VectorLayer;

/**
* @brief Cette classe permet simplement de changer la couleur de fond d'un texte (wxStaticText) lorsque l'on clique dessus.
**/
class SelectableStaticText : public wxStaticText
{
public:
    /// Constructeur
    /// @param parent La fenetre mere
    /// @param layercontrol Le LayerControl auquel est attache le texte
    /// @param id L'identifiant de la fenetre
    /// @param label Le texte affiche par le controle
    /// @param pos La position du controle
    /// @param size La taille du controle
    /// @param style Le style du controle
    /// @param name Le nom du controle
    SelectableStaticText(wxWindow* parent, LayerControl* layercontrol, wxWindowID id, const wxString& label, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxALIGN_LEFT, const wxString& name = _("staticText")) : wxStaticText(parent,id,label,pos,size, wxBG_STYLE_CUSTOM, name), m_parent(layercontrol), m_isSelected(false)
    {
	    //SetBackgroundStyle(wxBG_STYLE_CUSTOM);
        m_unselectedColour.Set( 100 , 50 , 50 );
        m_selectedColour.Set( 255 , 50 , 50 );
        SetForegroundColour( m_unselectedColour );
    };
    /// Destructeur
    virtual ~SelectableStaticText() {};

    /// Accesseur en lecture au flag de selection
    bool IsSelected() const { return m_isSelected; }
    /// Accesseur en ecriture au flag de selection
    void IsSelected(const bool isSelected) { m_isSelected = isSelected; if(m_isSelected) SetForegroundColour( m_selectedColour ); else SetForegroundColour( m_unselectedColour );}

    /// Cette methode permet de "selectionner" un calque. Lors d'un clic sur le nom du calque, la couleur de fond change
    void OnClick(wxMouseEvent& event);

private:
    /// La couleur lorsque le texte n'a pas ete selectionne (220,220,220)
    wxColour m_unselectedColour;
    /// La couleur lorsque le texte a pas ete selectionne (120,120,220)
    wxColour m_selectedColour;
    /// Le LayerControl auquel appartient le texte
    LayerControl* m_parent;
    /// Flage indiquant si le texte est selectionne
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
/**
* @brief Classe de gestion des differents calques.
**/
class LayerControl: public wxFrame
{
	// On passe la classe de sauvegarde en friend ...
	friend class XMLDisplayConfigurationIO;
	friend class LayerControlRow;

public:
    LayerControl(PanelViewer* DrawPane, wxFrame *parent, wxWindowID id = wxID_ANY, const wxString& title = _("wxToolBar Sample"),
        long style = wxDEFAULT_FRAME_STYLE , const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize );

	/// Cette methode permet d'ajouter un calque
	void AddLayer(const Layer::ptrLayerType &layer);
	///Cette methode permet de savoir s'il y a une orientation definie pour le viewer
	bool IsOriented() const {return m_isOrientationSet; }
	///Recupere l'orientation du viewer
	Orientation2D GetOrientation() const { return m_ori; }

	inline unsigned int GetNumberOfLayers() { return m_numberOfLayers; }

	void update();

	// Iterateurs pour acceder aux layers
	typedef std::vector < Layer::ptrLayerType > LayerContainerType;
	typedef LayerContainerType::iterator iterator;
	typedef LayerContainerType::const_iterator const_iterator;
	typedef LayerContainerType::reverse_iterator reverse_iterator;
	typedef LayerContainerType::const_reverse_iterator const_reverse_iterator;

	iterator begin(){ return m_layers.begin();}
	iterator end(){ return m_layers.end();}
	const_iterator begin() const { return m_layers.begin();}
	const_iterator end() const { return m_layers.end();}
	reverse_iterator rbegin(){ return m_layers.rbegin();}
	reverse_iterator rend(){ return m_layers.rend();}
	const_reverse_iterator rbegin() const { return m_layers.rbegin();}
	const_reverse_iterator rend() const { return m_layers.rend();}

	LayerContainerType Layers() const {return m_layers;}

	inline PanelViewer* GetPanelViewer() {return m_basicDrawPane;}

	inline std::vector< boost::shared_ptr<LayerControlRow> > GetRows() const { return m_rows; }

	/// Cette methode permet d'ajouter un calque existant (image ou shapefile ...)
	void OnOpenLayer(wxCommandEvent& event);
	/// Cette methode permet de creer un nouveau calque
	void OnNewLayer(wxCommandEvent& event);
	/// Cette methode permet de swapper 2 lignes
	void SwapRows(const unsigned int firstRow , const unsigned int secondRow);

	/// Cette methode permet d'ajouter un calque image en donnant tous les parametres
	void CreateNewImageLayerWithParameters( const ImageLayerParameters &parameters );
	void CreateNewVectorLayerWithParameters( const VectorLayerParameters &parameters );

	void AddLayersFromFiles( const wxArrayString &names );

    ///Calque ghost
	boost::shared_ptr<VectorLayerGhost> m_ghostLayer;

private:
	 void OnCloseWindow(wxCloseEvent& event);
	/// Cette methode permet d'afficher les informations sur le calque
	void OnInfoButton(wxCommandEvent& event);
	/// Cette methode permet de sauvegarder le calque
	void OnSaveButton(wxCommandEvent& event);
	/// Cette methode permet de supprimer le calque
	void OnDeleteButton(wxCommandEvent& event);
	/// Cette methode permet d'acceder aux parametres d'affichage du calque
	void OnSettingsButton(wxCommandEvent& event);
	void OnCheckVisibility(wxCommandEvent& event);
	void OnCheckTransformable(wxCommandEvent& event);
	/// Cette methode permet de remettre l'ensemble des calques a leur position initiale, de reinitialiser les transformations, de tout rendre visible et transformable
	void OnReset(wxCommandEvent& event);
	/// Cette methode permet de deplacer (vers le haut) les calques selectionnes
	void OnLayerUp(wxCommandEvent& event);
	/// Cette methode permet de deplacer (vers le bas) les calques selectionnes
	void OnLayerDown(wxCommandEvent& event);
	/// Cette methode permet d'inverser la visibilite des calques selectionnes
	void OnVisibilityButton(wxCommandEvent& event);
	/// Cette methode permet d'inverser le comportement des calques selectionnes par rapport aux transformations
	void OnTransformationButton(wxCommandEvent& event);
	/// Cette methode permet d'afficher l'interface de reglages des parametres globaux (pas grand chose pour l'instant ...)
	void OnGlobalSettingsButton(wxCommandEvent& event);
	/// Cette methode permet de sauvegarder dans un fichier XML les layers affiches ainsi que la configuration d'affichage courante
	void OnSaveDisplayConfigButton(wxCommandEvent& event);
	/// Cette methode permet de lire a partir d'un fichier XML les layers affiches ainsi que la configuration d'affichage courante
	void OnLoadDisplayConfigButton(wxCommandEvent& event);

	/// Cette methode permet de construire la barre d'outils
	void InitToolbar(wxToolBar* toolBar);

	/// Cette methode permet d'ajouter une ligne dans le LayerControl (elle est appelee lors de l'ajout d'un calque)
	void AddRow(const std::string &name , LayerSettingsControl *layersettings , const std::string &tooltip = "" );

	// Keyboard events
	void OnChar(wxKeyEvent& event);
	wxScrolledWindow 	*m_scroll;

protected:
	wxFlexGridSizer		*m_sizer;
	wxBoxSizer			*inner_sizer;
	unsigned int		m_numberOfLayers;
	PanelViewer* m_basicDrawPane;
	LayerContainerType	m_layers;
	// La, il faudrait mettre un boost::shared_array ...
	std::vector< boost::shared_ptr<LayerControlRow> > m_rows;

	//Orientation generale du viewer
	Orientation2D m_ori;
	bool m_isOrientationSet;

	GlobalSettingsControl* m_globalSettingsControl;


	DECLARE_EVENT_TABLE();
};

#endif // __LAYER_CONTROL_HPP__
