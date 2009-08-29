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

#include "gui/LayerControl.hpp"

#include <list>
#include <sstream>
#include <stdexcept>

#include <boost/filesystem.hpp>
#include <boost/bind.hpp>

#include <wx/xrc/xmlres.h>
#include <wx/checkbox.h>
#include <wx/scrolwin.h>
#include <wx/bmpbuttn.h>
#include <wx/textctrl.h>
#include <wx/confbase.h>
#include <wx/image.h>
#include <wx/progdlg.h>
#include <wx/toolbar.h>
#include <wx/tooltip.h>
#include <wx/tooltip.h>
#include <wx/filedlg.h>
#include <wx/slider.h>
#include <wx/sizer.h>
#include <wx/choice.h>
#include <wx/msgdlg.h>
#include <wx/icon.h>
#include <wx/file.h>
#include <wx/log.h>

#include "layers/Layer.hpp"
#include "layers/ImageLayer.hpp"
#include "layers/VectorLayer.hpp"
#include "layers/VectorLayerGhost.h"
#include "layers/VectorLayerContent.hpp"

#include "gui/VectorLayerSettingsControl.hpp"
#include "gui/ImageLayerSettingsControl.hpp"
#include "gui/GlobalSettingsControl.hpp"
#include "gui/NewVectorLayerControl.hpp"
#include "gui/LayerInfosControl.hpp"
#include "gui/PanelViewer.hpp"
#include "gui/define_id.hpp"

#include "io/XMLDisplayConfigurationIO.hpp"
#include "tools/Orientation2D.h"

#include "resources/eye.xpm"
#include "resources/Measure.xpm"
#include "resources/polygon_icon.xpm"
#include "resources/image_icon.xpm"

extern void InitXmlResource();

////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                  ///
///                                 SelectableStaticText                                             ///
///                                                                                                  ///
////////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(SelectableStaticText, wxStaticText)
EVT_LEFT_DOWN(SelectableStaticText::OnClick)
END_EVENT_TABLE()

void SelectableStaticText::OnClick(wxMouseEvent& event)
{
	/*
	 // On parcourt l'ensemble des autres calques pour les deselectionner
	 std::vector< boost::shared_ptr<LayerControlRow> > allRows = m_parent->GetRows();
	 for (unsigned int i=0;i<allRows.size();++i)
	 {
	 if ( allRows[i]->m_nameStaticText->m_isSelected == true )
	 {
	 allRows[i]->m_nameStaticText->SetBackgroundColour( m_unselectedColour );
	 allRows[i]->m_nameStaticText->ClearBackground();
	 allRows[i]->m_nameStaticText->SetLabel( allRows[i]->m_nameStaticText->GetLabel() );
	 allRows[i]->m_nameStaticText->m_isSelected = false;
	 }
	 }
	 */

	// On effectue la modification de couleur sur le calque selectionne et on change le flag
	if (m_isSelected == false)
	{
		//SetBackgroundColour( m_selectedColour );
		SetForegroundColour(m_selectedColour);
		//ClearBackground();
		SetLabel( GetLabel());
		m_isSelected = true;
	}
	else
	{
		//SetBackgroundColour( m_unselectedColour );
		SetForegroundColour(m_unselectedColour);
		//ClearBackground();
		SetLabel( GetLabel());
		m_isSelected = false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                  ///
///                                 LayerControlRow                                                  ///
///                                                                                                  ///
////////////////////////////////////////////////////////////////////////////////////////////////////////

LayerControlRow::LayerControlRow(LayerControl* parent, const std::string &name, const unsigned int index, LayerSettingsControl *layersettings, const std::string &tooltip) :
	m_parent(parent), m_index(index)
{
	// On ajoute l'ensemble des controles
	wxString name_wx(name.c_str(), *wxConvCurrent);
	wxString tool(tooltip.c_str(), *wxConvCurrent);
	wxFont fontFrameViewer(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	m_nameStaticText = new SelectableStaticText(m_parent->m_scroll, m_parent, wxID_HIGHEST + 12000, name_wx);
	m_nameStaticText->SetFont(fontFrameViewer);
	m_nameStaticText->SetToolTip(tool);

	m_visibilityCheckBox = new wxCheckBox(m_parent->m_scroll, ID_VISIBILITY + m_index, _(""));
	m_visibilityCheckBox->SetValue(true);
	m_visibilityCheckBox->SetToolTip(_("Change layer visibility"));
	m_visibilityCheckBox->Connect(ID_VISIBILITY + m_index, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(LayerControl::OnCheckVisibility), NULL, m_parent);

	m_transformationCheckBox = new wxCheckBox(m_parent->m_scroll, ID_TRANSFORMATION + m_index, _(""));
	m_transformationCheckBox->SetValue(true);
	m_transformationCheckBox->SetToolTip(_("Change layer transformability"));
	m_transformationCheckBox->Connect(ID_TRANSFORMATION + m_index, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(LayerControl::OnCheckTransformable), NULL, m_parent);

	m_boxSizer = new wxBoxSizer(wxHORIZONTAL);

        const wxSize imageSize(16,16);

	// L'icone du bouton  info depend du type : vecteur ou image
	if (dynamic_cast<VectorLayerSettingsControl*> (layersettings) != NULL)
                m_infoButton = new wxBitmapButton(m_parent->m_scroll, ID_INFO + m_index, wxXmlResource::Get()->LoadBitmap( wxT("APPLICATIONS-OTHERS_16x16") ));
	else
                m_infoButton = new wxBitmapButton(m_parent->m_scroll, ID_INFO + m_index, wxXmlResource::Get()->LoadBitmap( wxT("APPLICATIONS-GRAPHICS_16x16") ) );
	m_infoButton->SetToolTip(wxT("Display layer informations"));
	m_boxSizer->Add(m_infoButton, 0, wxALL | wxALIGN_CENTRE, 5);
	m_infoButton->Connect(ID_INFO + m_index, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayerControl::OnInfoButton), NULL, m_parent);

        m_saveButton = new wxBitmapButton(m_parent->m_scroll, ID_SAVE + m_index, wxXmlResource::Get()->LoadBitmap( wxT("DOCUMENT-SAVE_16x16") ));
	m_saveButton->SetToolTip(wxT("Save layer"));
	m_boxSizer->Add(m_saveButton, 0, wxALL | wxALIGN_CENTRE, 5);
	m_saveButton->Connect(ID_SAVE + m_index, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayerControl::OnSaveButton), NULL, m_parent);

        m_deleteButton = new wxBitmapButton(m_parent->m_scroll, ID_DELETE + m_index, wxXmlResource::Get()->LoadBitmap( wxT("USER-TRASH_16x16") ) );
	m_deleteButton->SetToolTip(wxT("Delete layer"));
	m_boxSizer->Add(m_deleteButton, 0, wxALL | wxALIGN_CENTRE, 5);
	m_deleteButton->Connect(ID_DELETE + m_index, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayerControl::OnDeleteButton), NULL, m_parent);

        m_settingsButton = new wxBitmapButton(m_parent->m_scroll, ID_SETTINGS + m_index, wxXmlResource::Get()->LoadBitmap( wxT("APPLICATIONS-SYSTEM_16x16") ) );
	m_settingsButton->SetToolTip(wxT("Layer settings"));
	m_boxSizer->Add(m_settingsButton, 0, wxALL | wxALIGN_CENTRE, 5);
	m_settingsButton->Connect(ID_SETTINGS + m_index, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayerControl::OnSettingsButton), NULL, m_parent);

	// On lie la ligne au LayerSettingsControl
	m_layerSettingsControl = layersettings;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                  ///
///                                 LayerControl                                                     ///
///                                                                                                  ///
////////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(LayerControl, wxFrame)
EVT_CLOSE(LayerControl::OnCloseWindow)
EVT_BUTTON(ID_INFO,LayerControl::OnInfoButton)
EVT_BUTTON(ID_SAVE,LayerControl::OnSaveButton)
EVT_BUTTON(ID_DELETE,LayerControl::OnDeleteButton)
// Gestion des evenements de la toolbar
EVT_TOOL(wxID_RESET,LayerControl::OnReset)
EVT_TOOL(wxID_OPEN,LayerControl::OnOpenLayer)
EVT_TOOL(wxID_NEW,LayerControl::OnNewLayer)
// Gestion des evenements "globaux" du LayerControl
EVT_BUTTON(wxID_UP,LayerControl::OnLayerUp)
EVT_BUTTON(wxID_DOWN,LayerControl::OnLayerDown)
EVT_BUTTON(ID_VISIBILITY_BUTTON,LayerControl::OnVisibilityButton)
EVT_BUTTON(ID_TRANSFORMATION_BUTTON,LayerControl::OnTransformationButton)
EVT_BUTTON(ID_GLOBAL_SETTINGS_BUTTON,LayerControl::OnGlobalSettingsButton)
EVT_BUTTON(wxID_SAVE,LayerControl::OnSaveDisplayConfigButton)
EVT_BUTTON(wxID_OPEN,LayerControl::OnLoadDisplayConfigButton)
EVT_CHAR(LayerControl::OnChar)
END_EVENT_TABLE()

LayerControl::LayerControl(PanelViewer* DrawPane, wxFrame* parent, wxWindowID id, const wxString& title, long style, const wxPoint& pos, const wxSize& size) :
wxFrame(parent, id, title, pos, size, style), m_ghostLayer(new VectorLayerGhost), m_numberOfLayers(0), m_basicDrawPane(DrawPane), m_isOrientationSet(false)

{
        // Initialising resources ...
        wxXmlResource::Get()->InitAllHandlers();
        InitXmlResource();

        SetIcon(wxArtProvider::GetIcon(wxART_LIST_VIEW, wxART_TOOLBAR, wxSize(32,32)));
	// Couleur de fond grisee
	wxColour bgcolor(220, 220, 220);
	SetBackgroundColour(bgcolor);
        ClearBackground();

	// Ajout de la barre d'outils
	this->CreateToolBar(wxNO_BORDER | wxTB_HORIZONTAL, 500);
	InitToolbar(this->GetToolBar());

	wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

	// On initialise le sizer qui permet de gerer efficacement le positionnement des controles des differentes lignes
	m_sizer = new wxFlexGridSizer(1, 4, 5, 5);

	m_scroll = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("scroll"));
	m_scroll->SetScrollRate(20,20);//,50,50);

	// On ajoute l'ensemble des boutons "globaux"
        const wxSize smallButtonSize(12,12);
	// Un sizer pour les fleches "up" et "down"
	wxBoxSizer* upDownButtonSizer = new wxBoxSizer(wxHORIZONTAL);
        // Fleche "up"
        //wxImage im_up = wxXmlResource::Get()->LoadBitmap( wxT("GO-UP_16x16") ).ConvertToImage().Rescale(12,12,wxIMAGE_QUALITY_HIGH);
        wxBitmapButton* upButton = new wxBitmapButton(m_scroll, wxID_UP, wxXmlResource::Get()->LoadBitmap( wxT("GO-UP_16x16") ) );
        //wxBitmapButton* upButton = new wxBitmapButton(m_scroll, wxID_UP, wxArtProvider::GetBitmap(wxART_GO_UP, wxART_BUTTON, smallButtonSize) );
	//upButton->SetToolTip( _("
	upDownButtonSizer->Add(upButton, 0, wxALL | wxALIGN_CENTRE, 5);
        // Fleche "down"
        upDownButtonSizer->Add(new wxBitmapButton(m_scroll, wxID_DOWN, wxXmlResource::Get()->LoadBitmap( wxT("GO-DOWN_16x16") ) ) , 0, wxALL | wxALIGN_CENTRE, 5);
	m_sizer->Add(upDownButtonSizer, 0, wxALL | wxALIGN_CENTRE, 5);
        // Bonuton pour inverser la visibilite de tous les calques selectionnes
        wxBitmapButton* visibilityButton = new wxBitmapButton(m_scroll, ID_VISIBILITY_BUTTON, wxXmlResource::Get()->LoadBitmap( wxT("EYE_16x16") ));
	visibilityButton->SetToolTip(_("Change selected layers visibility"));
	m_sizer->Add(visibilityButton, 0, wxALL | wxALIGN_CENTRE, 5);
	// Bouton pour inverser la transformabilite (!!!) de tous les calques selectionnes
        wxBitmapButton* transformabilityButton = new wxBitmapButton(m_scroll, ID_TRANSFORMATION_BUTTON, wxXmlResource::Get()->LoadBitmap( wxT("MEASURE_16x16") ) );
	transformabilityButton->SetToolTip(_("Change selected layers transformability"));
	m_sizer->Add(transformabilityButton, 0, wxALL | wxALIGN_CENTRE, 5);

	wxBoxSizer *globalSettingsSizer = new wxBoxSizer(wxHORIZONTAL);

	// Bouton pour les modifications globales
        wxBitmapButton* globalSettingsButton = new wxBitmapButton(m_scroll, ID_GLOBAL_SETTINGS_BUTTON, wxXmlResource::Get()->LoadBitmap( wxT("PREFERENCES-SYSTEM_16x16") ) );
	globalSettingsButton->SetToolTip(_("Global settings control. Apply to all layers ..."));
	globalSettingsSizer->Add(globalSettingsButton, 0, wxALL | wxALIGN_CENTRE, 5);

	// Bouton pour la sauvegarde de la configuration d'affichage
        wxBitmapButton* saveDisplayConfig = new wxBitmapButton(m_scroll, wxID_SAVE, wxXmlResource::Get()->LoadBitmap( wxT("DOCUMENT-SAVE_16x16") ) );
	saveDisplayConfig->SetToolTip(_("Save display configuration"));
	globalSettingsSizer->Add(saveDisplayConfig, 0, wxALL | wxALIGN_CENTRE, 5);

	// Bouton pour l'ouverture d'une configuration d'affichage
        wxBitmapButton* loadDisplayConfig = new wxBitmapButton(m_scroll, wxID_OPEN, wxXmlResource::Get()->LoadBitmap( wxT("DOCUMENT-OPEN_16x16") ) );
	loadDisplayConfig->SetToolTip(_("Load display configuration"));
	globalSettingsSizer->Add(loadDisplayConfig, 0, wxALL | wxALIGN_CENTRE, 5);

	m_sizer->Add(globalSettingsSizer, 0, wxALL | wxALIGN_CENTRE, 5);

	// Quand on cree le layercontrol, on en profite pour creer la fenetre de GlobalSettings.
	// On ne l'affiche que sur demande (clic bouton)
	m_globalSettingsControl = new GlobalSettingsControl(this);

/*
	m_sizer->SetSizeHints(this);
	SetSizer(m_sizer);
	Layout();
	* */

	m_scroll->SetSizer(m_sizer);
    //m_scroll->SetAutoLayout(true);
    m_scroll->Layout();
    // add the scrolledwindow to the main sizer
    main_sizer->Add(m_scroll, 1, wxEXPAND | wxALL, 5);
    SetSizer(main_sizer);
}

void LayerControl::OnCloseWindow(wxCloseEvent& WXUNUSED(event))
{
	// On ne fait que cacher la fenetre. Ca permet de garder les settings courants et de ne pas recalculer tout a chaque fois ...
	Hide();
}

void LayerControl::AddRow(const std::string &name, LayerSettingsControl *layersettings, const std::string &tooltip)
{
	// Le nom du layer a une longueur maximale de 20 caracteres
	std::string layer_name = name;
	layer_name = name + " ";
	std::ostringstream layer_index;
	layer_index << m_numberOfLayers;
	//layer_name = layer_name + layer_index.str();
	wxString layerName(layer_name.substr(0, 50).c_str(), *wxConvCurrent);
	std::string ln(layerName.fn_str());

	// on ajoute la ligne dans le conteneur
	m_rows.push_back(boost::shared_ptr<LayerControlRow>(new LayerControlRow(this, ln, m_numberOfLayers, layersettings, tooltip)));
	// On ajoute a proprement parler les controles de la ligne dans le LayerControl
	m_sizer->Add(m_rows[m_numberOfLayers]->m_nameStaticText, 0, wxTOP | wxALIGN_LEFT, 5);

	m_sizer->Add(m_rows[m_numberOfLayers]->m_visibilityCheckBox, 0, wxALL | wxALIGN_CENTRE, 5);
	m_rows[m_numberOfLayers]->m_visibilityCheckBox->SetValue(m_layers[m_numberOfLayers]->IsVisible());

	m_sizer->Add(m_rows[m_numberOfLayers]->m_transformationCheckBox, 0, wxALL | wxALIGN_CENTRE, 5);
	m_rows[m_numberOfLayers]->m_transformationCheckBox->SetValue(m_layers[m_numberOfLayers]->IsTransformable());

	// Enable or not the save button ?
	boost::shared_ptr<VectorLayer> vl = boost::dynamic_pointer_cast<VectorLayer>(m_layers[m_numberOfLayers]);
	if (vl)
	{
		if (!vl->IsEditable())
		{
			m_rows[m_numberOfLayers]->m_saveButton->Enable(false);
		}
	}

	m_sizer->Add(m_rows[m_numberOfLayers]->m_boxSizer, wxGROW);
	m_sizer->Fit(m_scroll);
    m_scroll->Layout();

	m_numberOfLayers++;
}

void LayerControl::InitToolbar(wxToolBar* toolBar)
{
	if ( !toolBar )
		toolBar = new wxToolBar(m_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTB_HORIZONTAL);

        const wxSize imageSize(16,16);

        toolBar->AddTool(wxID_NEW, _("N"), wxXmlResource::Get()->LoadBitmap( wxT("DOCUMENT-NEW_22x22") ) , wxNullBitmap, wxITEM_NORMAL, _("New file"));
        toolBar->AddTool(wxID_OPEN, _("O"), wxXmlResource::Get()->LoadBitmap( wxT("DOCUMENT-OPEN_22x22") ) , wxNullBitmap, wxITEM_NORMAL, _("Open file"));
        toolBar->AddTool(wxID_SAVE, _("S"), wxXmlResource::Get()->LoadBitmap( wxT("DOCUMENT-SAVE_22x22") ) , wxNullBitmap, wxITEM_NORMAL, _("Save file"));
        toolBar->AddTool(wxID_RESET, _("R"), wxXmlResource::Get()->LoadBitmap( wxT("EDIT-CLEAR_22x22") ) , wxNullBitmap, wxITEM_NORMAL, _("Reset"));

	toolBar->Realize();
}

void LayerControl::update()
{
	for (unsigned int id = 0;id<m_layers.size();++id)
	{
		m_rows[id]->m_transformationCheckBox->SetValue( m_layers[id]->IsTransformable() );
		m_rows[id]->m_visibilityCheckBox->SetValue( m_layers[id]->IsVisible() );
	}
}

void LayerControl::OnInfoButton(wxCommandEvent& event)
{
	// Get layer index
	unsigned int id = static_cast<unsigned int> (event.GetId()) - static_cast<unsigned int> (ID_INFO);

	LayerInfosControl *ilsc;

	// Test if the layer is a vector one or an image one ...
	boost::shared_ptr<VectorLayer> vl = boost::dynamic_pointer_cast<VectorLayer>(m_layers[id]);
	//if (typeid( m_layers[id] ) == typeid(VectorLayer))
	if (vl != NULL)
	{
		wxString title(_("Vector settings"));
		title << _(" : ") << wxString(m_layers[id]->Name().c_str(), *wxConvCurrent);
		vl->GetInfos();
		ilsc = new LayerInfosControl(m_layers[id]->GetInfos(), this, wxID_ANY, title, wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL | wxCLOSE_BOX);
	}
	else
	{
		wxString title(_("Image settings"));
		title << _(" : ") << wxString(m_layers[id]->Name().c_str(), *wxConvCurrent);
		ilsc = new LayerInfosControl(m_layers[id]->GetInfos(), this, wxID_ANY, title, wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL | wxCLOSE_BOX);
	}
	ilsc->Show();
}

void LayerControl::OnSaveButton(wxCommandEvent& event)
{
	// On commence par recupere l'indice du calque
	unsigned int id = static_cast<unsigned int> (event.GetId()) - static_cast<unsigned int> (ID_SAVE);
	boost::shared_ptr<VectorLayer> vl = boost::dynamic_pointer_cast<VectorLayer>(m_layers[id]);
	wxString wildcard;
	if (vl) // calque vectoriel
	{
		wildcard << _("Shapefile (*.shp)|*.shp");
	}
	else // calque image
	{
                wildcard << _("All supported files (*.tif;*.tiff;*.png;*.jpg;*.jpeg)|*.tif;*.tiff;*.png;*.jpg;*.jpeg|");
		wildcard << _("TIFF (*.tif;*.tiff)|*.tif;*.tiff|");
		wildcard << _("PNG (*.png)|*.png|");
                wildcard << _("JPEG (*.jpg;*.jpeg)|*.jpg;*.jpeg|");
	}
	wxFileDialog *fileDialog = new wxFileDialog(NULL, _("Save layer"), _(""), _(""), wildcard, wxFD_SAVE | wxFD_CHANGE_DIR);
	if (fileDialog->ShowModal() == wxID_OK)
	{
		try
		{
			Layers()[id]->Save( std::string( fileDialog->GetFilename().To8BitData() ) );
		}
		catch( std::exception &err )
		{
			std::ostringstream oss;
			oss << "File : " << __FILE__ << "\n";
			oss << "Function : " << __FUNCTION__ << "\n";
			oss << "Line : " << __LINE__ << "\n";
			oss << err.what();
			wxMessageBox(wxString(oss.str().c_str(), *wxConvCurrent));
		}
	}
}

void LayerControl::OnDeleteButton(wxCommandEvent& event)
{
	// Get layer index
	unsigned int id = static_cast<unsigned int> (event.GetId()) - static_cast<unsigned int> (ID_DELETE);

	//Swap
	for (unsigned int i = id; i < m_numberOfLayers - 1; ++i)
	{
		SwapRows(i, i + 1);
	}

	//Destroy de la row
	m_rows.back()->m_nameStaticText->Destroy();
	m_rows.back()->m_visibilityCheckBox->Destroy();
	m_rows.back()->m_transformationCheckBox->Destroy();
	m_rows.back()->m_infoButton->Destroy();
	m_rows.back()->m_saveButton->Destroy();
	m_rows.back()->m_deleteButton->Destroy();
	m_rows.back()->m_settingsButton->Destroy();
	m_rows.back()->m_layerSettingsControl->Destroy();
	m_sizer->Remove(m_rows.back()->m_boxSizer);

	m_rows.pop_back();

	//Delete de la layer
	m_layers.pop_back();

	--m_numberOfLayers;

	//Refresh de la vue
	m_basicDrawPane->Refresh();

	//
	m_sizer->Fit(m_scroll);
    m_scroll->Layout();

	//Orientation : supprimee si tous les calques ont ete supprimes
	if (m_layers.empty())
		m_isOrientationSet = false;

}

void LayerControl::OnSettingsButton(wxCommandEvent& event)
{
	// Get layer index
	unsigned int id = static_cast<unsigned int> (event.GetId()) - static_cast<unsigned int> (ID_SETTINGS);
	// Test if the layer is a vector one or an image one ...
//	if (typeid(m_layers[id]) == typeid(VectorLayer))
//	{
//		wxString title(_("Vector settings"));
//		title << _(" : ") << wxString(m_layers[id]->Name().c_str(), *wxConvCurrent);
//		m_rows[id]->m_layerSettingsControl->Show(!m_rows[id]->m_layerSettingsControl->IsVisible());
//	}
//	else
//	{
//		wxString title(_("Image settings"));
//		title << _(" : ") << wxString(m_layers[id]->Name().c_str(), *wxConvCurrent);
		m_rows[id]->m_layerSettingsControl->Show(!m_rows[id]->m_layerSettingsControl->IsVisible());
//	}
}

void LayerControl::OnCheckVisibility(wxCommandEvent& event)
{
	// On commence par recupere l'indice du calque
	unsigned int id = static_cast<unsigned int> (event.GetId()) - static_cast<unsigned int> (ID_VISIBILITY);
	// On inverse ...
	m_layers[id]->IsVisible(m_rows[id]->m_visibilityCheckBox->GetValue());
	// ... et on refresh
	m_layers[id]->HasToBeUpdated(true);
	m_basicDrawPane->Refresh();
}

void LayerControl::OnCheckTransformable(wxCommandEvent& event)
{
	// On commence par recupere l'indice du calque
	unsigned int id = static_cast<unsigned int> (event.GetId()) - static_cast<unsigned int> (ID_TRANSFORMATION);
	// On inverse ...
	m_layers[id]->IsTransformable(m_rows[id]->m_transformationCheckBox->GetValue());
	// ... et on refresh
	m_basicDrawPane->Refresh();
}

void LayerControl::OnReset(wxCommandEvent& event)
{
	// Pour chaque calque, on reinitialise
	for (LayerControl::iterator it = begin(); it != end(); ++it)
	{
		(*it)->ZoomFactor(1.);
		(*it)->TranslationX(0.);
		(*it)->TranslationY(0.);
		(*it)->HasToBeUpdated(true);
		(*it)->IsTransformable(true);
		(*it)->IsVisible(true);
	}

	// Et on remet les checkbox en place (true)
	for (unsigned int i = 0; i < m_numberOfLayers; ++i)
	{
		m_rows[i]->m_visibilityCheckBox->SetValue(true);
		m_rows[i]->m_transformationCheckBox->SetValue(true);
	}

	// On refresh le tout ...
	m_basicDrawPane->Refresh();
}

void LayerControl::OnOpenLayer(wxCommandEvent& event)
{
	wxString wildcard;
        wildcard << _("All supported files (*.tif;*.tiff;*.png;*.jpg;*.jpeg;*.shp)|*.tif;*.tiff;*.TIF;*.TIFF;*.png;*.PNG;*.jpg;*.jpeg;*.JPG;*.JPEG;*.shp;*.SHP|");
        wildcard << _("Image files (*.tif;*.tiff;*.png;*.jpg;*.jpeg)|*.tif;*.tiff;*.png;*.jpg;*.jpeg|");
	wildcard << _("TIFF (*.tif;*.tiff;*.TIF;*.TIFF)|*.tif;*.tiff;*.TIF;*.TIFF|");
	wildcard << _("PNG (*.png;*.PNG)|*.png;*.PNG|");
        wildcard << _("JPEG (*.jpg;*.jpeg;*.JPG;*.JPEG)|*.jpg;*.jpeg;*.JPG;*.JPEG|");
	wildcard << _("Shapefile (*.shp)|*.shp;*.SHP|");
	wildcard << _("CustomFormat (*)|*");
	wxString str;
	wxConfigBase::Get()->Read(_T("/Paths/WorkingDirectory"), &str, ::wxGetCwd());
	wxFileDialog *fileDialog = new wxFileDialog(this, _("Open image or shapefile"), str, _(""), wildcard, wxFD_OPEN | wxFD_MULTIPLE | wxFD_CHANGE_DIR);

	if (fileDialog->ShowModal() == wxID_OK)
	{
		wxArrayString names;
		fileDialog->GetPaths(names);
		AddLayersFromFiles(names);
	}
	fileDialog->Destroy();
	m_basicDrawPane->SetCursor(wxCursor(wxCURSOR_ARROW));

	Layout();
	Refresh();
}

#include <boost/lambda/lambda.hpp>
#include <boost/algorithm/string/case_conv.hpp>

void LayerControl::AddLayersFromFiles(const wxArrayString &names)
{
	unsigned int i;
	m_basicDrawPane->SetCursor(wxCursor(wxCURSOR_WAIT));
	wxProgressDialog *progress = NULL;
	wxProgressDialog *progressLargeFile = NULL;

	//Liste des formats gérés par le viewer
	std::list<std::string> listeFormatsGeres;
	listeFormatsGeres.push_back(".tif");
	listeFormatsGeres.push_back(".tiff");
	listeFormatsGeres.push_back(".jpg");
	listeFormatsGeres.push_back(".jpeg");
        listeFormatsGeres.push_back(".png");
	// On rajoute les mêmes, mais avec des majuscules ...
	listeFormatsGeres.push_back(".TIF");
	listeFormatsGeres.push_back(".TIFF");
	listeFormatsGeres.push_back(".JPG");
	listeFormatsGeres.push_back(".JPEG");
        listeFormatsGeres.push_back(".PNG");

//	boost::algorithm::to_upper_copy
//	std::transform(listeFormatsGeres.begin(),
//			listeFormatsGeres.end(),
//			std::back_inserter(listeFormatsGeres),
//			 );

	if (names.GetCount() >= 2)
		progress = new wxProgressDialog(_("Opening files ..."), _("Reading ..."), names.GetCount(), NULL, wxPD_AUTO_HIDE | wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME | wxPD_REMAINING_TIME);

	for (i = 0; i < names.GetCount(); ++i)
	{
		if (names.GetCount() >= 2)
		{
			wxString m;
			m << _("Reading file ") << i << _("/") << names.GetCount() << _("\n");
			m << names[i] << _("\n");
			progress->Update(i, m);
		}

		//On teste l'extension : soit c'est du shp, soit c'est du format image, soit c'est autre chose et on renvoit vers le panel
		const std::string extension(boost::filesystem::extension(std::string(names[i].fn_str())));
		if (extension == ".shp")
		{
			try
			{
				std::string filename(names[i].fn_str());
				Layer::ptrLayerType layer = VectorLayer::CreateVectorLayer(boost::filesystem::basename(std::string(names[i].fn_str())), filename);
				AddLayer(layer);
			}
			catch (std::exception &err)
			{
				if (names.GetCount() >= 2)
					progress->Destroy();
				std::ostringstream oss;
				oss << "File : " << __FILE__ << "\n";
				oss << "Function : " << __FUNCTION__ << "\n";
				oss << "Line : " << __LINE__ << "\n";
				oss << err.what();
				wxMessageBox(wxString(oss.str().c_str(), *wxConvCurrent));
			}
		}
		else if (std::find(listeFormatsGeres.begin(), listeFormatsGeres.end(), extension) != listeFormatsGeres.end())
		{
			try
			{
				std::string filename(names[i].fn_str());
				wxFile filesize(names[i].c_str(), wxFile::read);
				if ((filesize.Length() / 100000) > 100) // 100 Mo

				{
					progressLargeFile = new wxProgressDialog(_("Opening file"), _("Reading ..."), (int) (filesize.Length() / 100000), NULL, wxPD_AUTO_HIDE | wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME
							| wxPD_REMAINING_TIME);
					progressLargeFile->Update(0);
				}
				Layer::ptrLayerType ptr = ImageLayer::CreateImageLayer(filename);
				if (ptr)
				{
					AddLayer(ptr);
				}
				if (progressLargeFile != NULL)
				{
					progressLargeFile->Destroy();
				}
			}
//			catch (itk::ExceptionObject & err)
//			{
//				std::ostringstream oss;
//				oss << "File : " << __FILE__ << "\n";
//				oss << "Function : " << __FUNCTION__ << "\n";
//				oss << "Line : " << __LINE__ << "\n";
//				oss << err;
//				wxMessageBox(wxString(oss.str().c_str(), *wxConvCurrent));
//			}
			catch (std::exception &err)
			{
				if (progressLargeFile)
					progressLargeFile->Destroy();
				std::ostringstream oss;
				oss << "File : " << __FILE__ << "\n";
				oss << "Function : " << __FUNCTION__ << "\n";
				oss << "Line : " << __LINE__ << "\n";
				oss << err.what();
				wxMessageBox(wxString(oss.str().c_str(), *wxConvCurrent));
			}
		}
		else
		{
			//c'est un format supporté par une appli dérivée du viewer -> on redirige vers PanelViewer::OpenCustomFormat
			m_basicDrawPane->OpenCustomFormat(std::string(names[i].fn_str()));
		}
	}

	m_basicDrawPane->Refresh();
	if (names.GetCount() >= 2)
		progress->Destroy();
	m_basicDrawPane->SetCursor(wxCursor(wxCURSOR_ARROW));
}

void LayerControl::OnNewLayer(wxCommandEvent& event)
{
	// On cree la boite de dialogue permettant de choisir le type de calques a creer
	// On fait un appel au bon constructeur et on ajoute le layer dans le LayerControl
	NewVectorLayerControl *test = new NewVectorLayerControl(this, wxID_ANY, _("New vector layer"), wxDefaultPosition, wxSize(500, 400));
	if (test->ShowModal() == wxID_OK)
	{
		Layer::ptrLayerType newLayer = VectorLayer::CreateVectorLayer(test->GetLayerName(), test->GetLayerType(), 1, false);

		if (m_layers.size() > 0)
		{
			newLayer->ZoomFactor(m_layers[0]->ZoomFactor());
			newLayer->TranslationX(m_layers[0]->TranslationX());
			newLayer->TranslationY(m_layers[0]->TranslationY());
		}
		newLayer->HasToBeUpdated(true);
		newLayer->IsVisible(true);

		AddLayer(newLayer);

		// Pour l'instant, tout est fait en coordonnees image !!!
		// Dans le cas contraire, il faut jouer avec le flagPRJ
		// Remarque similaire avec le flagDBF
	}

	Refresh();
	m_basicDrawPane->Refresh();

	test->Destroy();
}

void LayerControl::AddLayer(const Layer::ptrLayerType &layer)
{
	if (!layer)
		return;
	// On ajoute le calque dans le conteneur
	layer->SetNotifyLayerControl( boost::bind( &LayerControl::update, this ) );
	m_layers.push_back(layer);

	// On construit le SettingsControl en fonction du type de calque ajoute
	LayerSettingsControl *settingscontrol;
	boost::shared_ptr<VectorLayer> vl = boost::dynamic_pointer_cast<VectorLayer>(layer);
	if (vl != NULL)
		settingscontrol = new VectorLayerSettingsControl(m_layers.size() - 1, this);
	else
		settingscontrol = new ImageLayerSettingsControl(m_layers.size() - 1, this);
	layer->SetNotifyLayerSettingsControl( boost::bind( &LayerSettingsControl::update, settingscontrol ) );
	// On ajoute la ligne correspondante
	std::string name = "  ";
	name = name + boost::filesystem::basename(layer->Name());
	AddRow(name, settingscontrol, layer->Name());

	//Si c'est un calque image avec ori et que c'est le premier on met en place l'orientation generale du viewer
	if (!m_isOrientationSet && m_layers.size() == 1 && layer->HasOri())
	{
		m_ori = layer->Orientation();
		m_isOrientationSet = true;
		::wxLogMessage(_("L'orientation du viewer a ete parametree !"));
	}
	else if (!m_isOrientationSet && m_layers.size() > 1 && layer->HasOri())
	{
		::wxLogMessage(_("Attention l'ori de l'image ne sera pas utilisee car pas d'ori definie pour la premiere image affichee !"));
	}
	else if (!m_isOrientationSet && m_layers.size() > 1 && !layer->HasOri())
	{
		::wxLogMessage(_("Initialisation de la position du calque image en accord avec l'image de depart !"));
		layer->ZoomFactor(m_layers[0]->ZoomFactor());
		layer->TranslationX(m_layers[0]->TranslationX());
		layer->TranslationY(m_layers[0]->TranslationY());

	}

	//Si il y a une orientation definie pour le viewer et pour le nouveau calque image on initialise correctement
	//sa position initiale et son zoom
	if (m_isOrientationSet && layer->HasOri())
	{
		::wxLogMessage(_("Initialisation de la position du calque image en accord avec l'ori globale !"));

		const Orientation2D &oriLayer = layer->Orientation();

		float newZoomFactor = m_ori.Step() / oriLayer.Step();
		float translationInitX = static_cast<float> ((oriLayer.OriginX() - m_ori.OriginX()) / oriLayer.Step());//+ m_layers[0]->TranslationX()/m_layers[0]->ZoomFactor();
		float translationInitY = -static_cast<float> ((oriLayer.OriginY() - m_ori.OriginY()) / oriLayer.Step());//+ m_layers[0]->TranslationY()/m_layers[0]->ZoomFactor();

		layer->ZoomFactor(newZoomFactor * m_layers[0]->ZoomFactor());
		layer->TranslationX(translationInitX + m_layers[0]->TranslationX() * newZoomFactor);//* layer->ZoomFactor());
		layer->TranslationY(translationInitY + m_layers[0]->TranslationY() * newZoomFactor);//* layer->ZoomFactor());
	}

	//Si il y a une orientation definie pour le viewer et et qu'on a affaire a une couche vecteur :
	if (m_isOrientationSet && vl != NULL)
	{
		::wxLogMessage(_("Initialisation de la position du calque vecteur en accord avec l'ori globale !"));

		float translationInitX = static_cast<float> (-m_ori.OriginX());// * m_layers[0]->ZoomFactor() ) + m_layers[0]->TranslationX();
		float translationInitY = -static_cast<float> (-m_ori.OriginY()); //* m_layers[0]->ZoomFactor() ) + m_layers[0]->TranslationY();

		float newZoomFactor = m_ori.Step();
		layer->ZoomFactor(newZoomFactor * m_layers[0]->ZoomFactor());
		layer->TranslationX(translationInitX + m_layers[0]->TranslationX() * newZoomFactor);
		layer->TranslationY(translationInitY + m_layers[0]->TranslationY() * newZoomFactor);
	}
	layer->SetDefaultDisplayParameters();
	layer->notifyLayerSettingsControl_();

	Refresh();
	m_basicDrawPane->Refresh();
}

void LayerControl::SwapRows(const unsigned int firstRow, const unsigned int secondRow)
{
	if (firstRow < 0 || secondRow < 0)
	{
		std::ostringstream oss;
		oss << "You passed a negative index for a row !" << std::endl;
		oss << "firstRow = " << firstRow << "  --  secondRow = " << secondRow << std::endl;
		oss << "File : " << __FILE__ << std::endl;
		oss << "Line : " << __LINE__ << std::endl;
		oss << "Function : " << __FUNCTION__ << std::endl;
		// On est dans du wx, je m'autorise une MessageBox ...
		std::string mess(oss.str());
		wxString mes(mess.c_str(), *wxConvCurrent);
		::wxLogMessage(mes);
		return;
	}
	else if (firstRow >= m_numberOfLayers || secondRow >= m_numberOfLayers)
	{
		std::ostringstream oss;
		oss << "You passed an invalid index for a row !" << std::endl;
		oss << "firstRow = " << firstRow << "  --  secondRow = " << secondRow << std::endl;
		oss << "m_numberOfLayers = " << m_numberOfLayers << std::endl;
		oss << "File : " << __FILE__ << std::endl;
		oss << "Line : " << __LINE__ << std::endl;
		oss << "Function : " << __FUNCTION__ << std::endl;
		// On est dans du wx, je m'autorise une MessageBox ...
		std::string mess(oss.str());
		wxString mes(mess.c_str(), *wxConvCurrent);
		::wxLogMessage(mes);
		return;
	}

	//statictext
	{
		// On fait aussi la mise a jour du tooltip ...
		wxString temp(m_rows[firstRow]->m_nameStaticText->GetLabel());
		wxString strTooltip(m_rows[firstRow]->m_nameStaticText->GetToolTip()->GetTip());

		m_rows[firstRow]->m_nameStaticText->SetLabel(m_rows[secondRow]->m_nameStaticText->GetLabel());
		m_rows[firstRow]->m_nameStaticText->GetToolTip()->SetTip(m_rows[secondRow]->m_nameStaticText->GetToolTip()->GetTip());
		m_rows[secondRow]->m_nameStaticText->SetLabel(temp);
		m_rows[secondRow]->m_nameStaticText->GetToolTip()->SetTip(strTooltip);
		m_rows[firstRow]->m_nameStaticText->IsSelected(false);
		m_rows[secondRow]->m_nameStaticText->IsSelected(true);
	}

	//visibility

	{
		bool temp(m_rows[firstRow]->m_visibilityCheckBox->GetValue());
		m_rows[firstRow]->m_visibilityCheckBox->SetValue(m_rows[secondRow]->m_visibilityCheckBox->GetValue());
		m_rows[secondRow]->m_visibilityCheckBox->SetValue(temp);
	}

	//transform
	{
		bool temp(m_rows[firstRow]->m_transformationCheckBox->GetValue());
		m_rows[firstRow]->m_transformationCheckBox->SetValue(m_rows[secondRow]->m_transformationCheckBox->GetValue());
		m_rows[secondRow]->m_transformationCheckBox->SetValue(temp);
	}

	//layer
	{
		Layer::ptrLayerType temp = m_layers[firstRow];
		m_layers[firstRow] = m_layers[secondRow];
		m_layers[secondRow] = temp;
	}

	//layersettingscontrol
	{
		LayerSettingsControl *temp = m_rows[firstRow]->m_layerSettingsControl;
		m_rows[firstRow]->m_layerSettingsControl = m_rows[secondRow]->m_layerSettingsControl;
		m_rows[secondRow]->m_layerSettingsControl = temp;

		m_rows[firstRow]->m_layerSettingsControl->Index(firstRow);
		m_rows[secondRow]->m_layerSettingsControl->Index(secondRow);
	}

	// Finalement, si necessaire, il faut changer le bitmap associe au bouton d'infos
	{
		if (dynamic_cast<VectorLayerSettingsControl*> (m_rows[firstRow]->m_layerSettingsControl))
			m_rows[firstRow]->m_infoButton->SetBitmapLabel(wxBitmap(polygon_icon_xpm));
		else if (dynamic_cast<ImageLayerSettingsControl*> (m_rows[firstRow]->m_layerSettingsControl))
			m_rows[firstRow]->m_infoButton->SetBitmapLabel(wxBitmap(image_icon_xpm));
		if (dynamic_cast<VectorLayerSettingsControl*> (m_rows[secondRow]->m_layerSettingsControl))
			m_rows[secondRow]->m_infoButton->SetBitmapLabel(wxBitmap(polygon_icon_xpm));
		else if (dynamic_cast<ImageLayerSettingsControl*> (m_rows[secondRow]->m_layerSettingsControl))
			m_rows[secondRow]->m_infoButton->SetBitmapLabel(wxBitmap(image_icon_xpm));
	}

	// Ticket #9 : bouton sauvegarde ne bouge pas ...
	{
		VectorLayerSettingsControl* tempVector = dynamic_cast<VectorLayerSettingsControl*> (m_rows[firstRow]->m_layerSettingsControl);
		if (tempVector)
		{
			boost::shared_ptr< VectorLayer > vectorLayer = boost::dynamic_pointer_cast< VectorLayer >( m_layers[tempVector->Index()] );
			if ( vectorLayer->IsEditable() )
				m_rows[firstRow]->m_saveButton->Enable(true);
			else
				m_rows[firstRow]->m_saveButton->Enable(false);
		}
		VectorLayerSettingsControl* tempVector2 = dynamic_cast<VectorLayerSettingsControl*> (m_rows[secondRow]->m_layerSettingsControl);
		if (tempVector2)
		{
			boost::shared_ptr< VectorLayer > vectorLayer = boost::dynamic_pointer_cast< VectorLayer >( m_layers[tempVector2->Index()] );
			if ( vectorLayer->IsEditable() )
				m_rows[secondRow]->m_saveButton->Enable(true);
			else
				m_rows[secondRow]->m_saveButton->Enable(false);
		}
	}
}

void LayerControl::OnLayerUp(wxCommandEvent& WXUNUSED(event))
{
	for (unsigned int i = 0; i < m_numberOfLayers; ++i)
	{
		if (m_rows[i]->m_nameStaticText->IsSelected())
		{
			SwapRows(i, i - 1);
			break;
		}
	}

	Refresh();
	m_basicDrawPane->Refresh();

}

void LayerControl::OnLayerDown(wxCommandEvent& WXUNUSED(event))
{
	for (unsigned int i = 0; i < m_numberOfLayers; ++i)
	{
		if (m_rows[i]->m_nameStaticText->IsSelected())
		{
			SwapRows(i, i + 1);
			break;
		}
	}

	Refresh();
	m_basicDrawPane->Refresh();
}

void LayerControl::OnVisibilityButton(wxCommandEvent& event)
{
	// On parcourt tous les calques et on inverse la visibilite si la selection est a true
	for (unsigned int i = 0; i < m_numberOfLayers; ++i)
	{
		if (m_rows[i]->m_nameStaticText->IsSelected() == true)
		{
			m_rows[i]->m_visibilityCheckBox->SetValue(!m_rows[i]->m_visibilityCheckBox->GetValue());
			m_layers[i]->IsVisible(m_rows[i]->m_visibilityCheckBox->GetValue());
		}
	}

	// On refresh le tout ...
	m_basicDrawPane->Refresh();
}

void LayerControl::OnTransformationButton(wxCommandEvent& event)
{
	// On parcourt tous les calques et on inverse la transformabilite (!!!) si la section est a true
	for (unsigned int i = 0; i < m_numberOfLayers; ++i)
	{
		if (m_rows[i]->m_nameStaticText->IsSelected() == true)
		{
			m_rows[i]->m_transformationCheckBox->SetValue(!m_rows[i]->m_transformationCheckBox->GetValue());
			m_layers[i]->IsTransformable(m_rows[i]->m_transformationCheckBox->GetValue());
		}
	}

	// On refresh le tout ...
	m_basicDrawPane->Refresh();
}

void LayerControl::OnGlobalSettingsButton(wxCommandEvent& event)
{
	m_globalSettingsControl->Show();
}

void LayerControl::OnSaveDisplayConfigButton(wxCommandEvent& event)
{
	// Choix du fichier de sauvegarde
	wxFileDialog* fd = new wxFileDialog(this, _("Sauvegarde de la configuration d'affichage"), _(""), _(""), _("*.xml"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
	if (fd->ShowModal() == wxID_OK)
	{
		std::string savename(fd->GetPath().fn_str());
		if (boost::filesystem::extension(savename) != ".xml")
			boost::filesystem::change_extension(savename, ".xml");

		XMLDisplayConfigurationIO::Write(this, savename.c_str());
	}
	delete fd;
}

void LayerControl::OnLoadDisplayConfigButton(wxCommandEvent& event)
{
	// Choix du fichier de sauvegarde
	wxFileDialog* fd = new wxFileDialog(this, _("Chargement d'une configuration d'affichage"), _(""), _(""), _("*.xml"), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);
	if (fd->ShowModal() == wxID_OK)
	{
		wxString message;
		message << _("Lecture d'un fichier de configration d'affichage : ") << fd->GetPath();
		::wxLogMessage(message);

		std::string loadname(fd->GetPath().fn_str());
		if (boost::filesystem::extension(loadname) != ".xml")
		{
			wxLogMessage(_("Le fichier de configuration doit avoir l'extension .xml !"), _("Erreur !"));
			return;
		}

		XMLDisplayConfigurationIO::Read(this, loadname);
		m_basicDrawPane->Refresh();
	}

	delete fd;
}

void LayerControl::OnChar(wxKeyEvent& event)
{

	if (event.GetKeyCode() == 'l')
	{
		this->Hide();
	}

	event.Skip();
}

void LayerControl::CreateNewImageLayerWithParameters(const ImageLayerParameters &parameters)
{
	try
	{
		// On cree cette fameuse image ...
		std::string filename(parameters.path.c_str());

		Layer::ptrLayerType ptr = ImageLayer::CreateImageLayer(filename);
		if (ptr)
			AddLayer(ptr);
		else
			return;

		// Et on sette l'ensemble des parametres qu'on a pu lire ...
		this->m_layers.back()->IsVisible(parameters.isVisible);
		this->m_layers.back()->IsTransformable(parameters.isTransformable);
		this->m_layers.back()->Alpha(parameters.alpha);
		this->m_layers.back()->Gamma(parameters.gamma);
		this->m_layers.back()->IntensityMin(parameters.intensityMin);
		this->m_layers.back()->IntensityMax(parameters.intensityMax);
		this->m_layers.back()->IsTransparent(parameters.isTransparent);
		this->m_layers.back()->TransparencyMin(parameters.transparencyMin);
		this->m_layers.back()->TransparencyMax(parameters.transparencyMax);
		this->m_layers.back()->ZoomFactor(parameters.zoomFactor);
		this->m_layers.back()->TranslationX(parameters.translationX);
		this->m_layers.back()->TranslationY(parameters.translationY);
		this->m_layers.back()->SetAlphaChannel(parameters.useAlphaChannel,parameters.alphaChannel);

		// MAJ de l'interface
		this->m_layers.back()->notifyLayerControl_();
		this->m_layers.back()->notifyLayerSettingsControl_();
	}
	catch (std::exception &err)
	{
		std::ostringstream oss;
		oss << "File : " << __FILE__ << "\n";
		oss << "Function : " << __FUNCTION__ << "\n";
		oss << "Line : " << __LINE__ << "\n";
		oss << err.what();
		::wxMessageBox(wxString(oss.str().c_str(), *wxConvCurrent));
	}
}

void LayerControl::CreateNewVectorLayerWithParameters(const VectorLayerParameters &parameters)
{
	try
	{
		// On cree cette fameuse image ...
		std::string filename(parameters.path.c_str());
		AddLayer(VectorLayer::CreateVectorLayer(boost::filesystem::basename(filename), filename));
		// Et on sette l'ensemble des parametres qu'on a pu lire ...

		this->m_layers.back()->IsVisible(parameters.isVisible);
		this->m_layers.back()->IsTransformable(parameters.isTransformable);
		this->m_layers.back()->SetPointsStyle(parameters.pointsColor, parameters.pointsWidth);
		this->m_layers.back()->SetLinesStyle(parameters.linesColor, parameters.linesWidth, parameters.linesStyle);
		this->m_layers.back()->PolygonsRingsColour(parameters.polygonsRingsColor);
		this->m_layers.back()->PolygonsRingsWidth(parameters.polygonsRingsWidth);
		this->m_layers.back()->PolygonsInsideColour(parameters.polygonsInsideColor);
		this->m_layers.back()->PolygonsRingsStyle(parameters.polygonsRingsStyle);
		this->m_layers.back()->PolygonsInsideStyle(parameters.polygonsInsideStyle);
		this->m_layers.back()->ZoomFactor(parameters.zoomFactor);
		this->m_layers.back()->TranslationX(parameters.translationX);
		this->m_layers.back()->TranslationY(parameters.translationY);

		// MAJ de l'interface
		this->m_layers.back()->notifyLayerControl_();
		this->m_layers.back()->notifyLayerSettingsControl_();
	}
	catch (std::exception &err)
	{
		std::ostringstream oss;
		oss << "File : " << __FILE__ << "\n";
		oss << "Function : " << __FUNCTION__ << "\n";
		oss << "Line : " << __LINE__ << "\n";
		oss << err.what();
		::wxMessageBox(wxString(oss.str().c_str(), *wxConvCurrent));
	}
}
