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

#include <wx/frame.h>

#include "../gui/layer_control_utils.hpp"
#include "../layers/layer.hpp"

class wxFlexGridSizer;
class wxToolBar;
class wxScrolledWindow;

class global_settings_control;
class vector_layer_ghost;
class orientation_2d;
class panel_viewer;
class VectorLayer;

/**
* @brief Classe de gestion des differents calques.
**/
class layer_control: public wxFrame
{
    // On passe la classe de sauvegarde en friend ...
    friend class xml_display_configuration_io;
    friend class layer_control_row;

public:
    layer_control(panel_viewer* DrawPane, wxFrame *parent, wxWindowID id = wxID_ANY, const wxString& title = _("wxToolBar Sample"),
                  long style = wxDEFAULT_FRAME_STYLE , const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize );

    /// Cette methode permet d'ajouter un calque
    void add_layer(const layer::ptrLayerType &layer, bool has_transform = false);
    /// Cette methode permet de supprimer un calque
    void delete_layer(unsigned int index);
    void delete_all_layer(){wxCommandEvent event;on_delete_all_rows_button(event);}
    
    /// Recuperation d'un calque par son Id
    layer::ptrLayerType get_layer_with_id(unsigned int id)const;
    layer::ptrLayerType get_layer_with_filename(const std::string& filename)const;
    
    ///Cette methode permet de savoir s'il y a une orientation definie pour le viewer
    bool oriented() const {return m_isOrientationSet; }
    ///Recupere l'orientation du viewer
    boost::shared_ptr<orientation_2d> orientation() const;

    inline unsigned int number_of_layers() { return m_numberOfLayers; }

    void update();

    // Iterateurs pour acceder aux layers
    typedef std::vector < layer::ptrLayerType > LayerContainerType;
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

    LayerContainerType layers() const {return m_layers;}

    inline panel_viewer* panelviewer() {return m_basicDrawPane;}

    inline std::vector< boost::shared_ptr<layer_control_row> > rows() const { return m_rows; }

    /// Cette methode permet d'ajouter un calque existant (image ou shapefile ...)
    void on_open_layer(wxCommandEvent& event);
    /// Cette methode permet de swapper 2 lignes
    void swap_rows(const unsigned int firstRow , const unsigned int secondRow);

    /// Cette methode permet d'ajouter un calque image en donnant tous les parametres
    void create_new_image_layer_with_parameters( const ImageLayerParameters &parameters );
    void create_new_vector_layer_with_parameters( const VectorLayerParameters &parameters );

    void add_layers_from_files( const wxArrayString &names );

    ///Calque ghost
    boost::shared_ptr<vector_layer_ghost> m_ghostLayer;

private:
    void on_close_window(wxCloseEvent& event);
    /// Cette methode permet d'afficher les informations sur le calque
    void on_info_button(wxCommandEvent& event);
    /// Cette methode permet de sauvegarder le calque
    void on_save_button(wxCommandEvent& event);
    /// Cette methode permet de supprimer le calque
    void on_delete_button(wxCommandEvent& event);
    /// Cette methode permet d'acceder aux parametres d'affichage du calque
    void on_settings_button(wxCommandEvent& event);
    void on_center_button(wxCommandEvent& event);
    void on_refresh_button(wxCommandEvent& event);
    void on_check_visibility(wxCommandEvent& event);
    void on_check_transformable(wxCommandEvent& event);
    /// Cette methode permet de remettre l'ensemble des calques a leur position initiale, de reinitialiser les transformations, de tout rendre visible et transformable
    void on_reset(wxCommandEvent& event);
    /// Cette methode permet de deplacer (vers le haut) les calques selectionnes
    void on_layer_up(wxCommandEvent& event);
    /// Cette methode permet de deplacer (vers le bas) les calques selectionnes
    void on_layer_down(wxCommandEvent& event);
    /// Cette methode permet d'inverser la visibilite des calques selectionnes
    void on_visibility_button(wxCommandEvent& event);
    /// Cette methode permet d'inverser le comportement des calques selectionnes par rapport aux transformations
    void on_transformation_button(wxCommandEvent& event);
    /// Cette methode permet d'afficher l'interface de reglages des parametres globaux (pas grand chose pour l'instant ...)
    void on_global_settings_button(wxCommandEvent& event);
    /// Cette methode permet de sauvegarder dans un fichier XML les layers affiches ainsi que la configuration d'affichage courante
    void on_save_display_config_button(wxCommandEvent& event);
    /// Cette methode permet de lire a partir d'un fichier XML les layers affiches ainsi que la configuration d'affichage courante
    void on_load_display_config_button(wxCommandEvent& event);
    /// Cette methode permet de supprimer tous les layers affiches
    void on_delete_all_rows_button(wxCommandEvent& event);

    /// Cette methode permet de construire la barre d'outils
    void init_toolbar(wxToolBar* toolBar);

    /// Cette methode permet d'ajouter une ligne dans le LayerControl (elle est appelee lors de l'ajout d'un calque)
    void add_row(const std::string &name , layer_settings_control *layersettings , const std::string &tooltip = "" );

    wxScrolledWindow    *m_scroll;

protected:
    wxFlexGridSizer     *m_sizer;
    wxBoxSizer          *inner_sizer;
    unsigned int        m_numberOfLayers;
    panel_viewer* m_basicDrawPane;
    LayerContainerType  m_layers;
    // La, il faudrait mettre un boost::shared_array ...
    std::vector< boost::shared_ptr<layer_control_row> > m_rows;

    //Orientation generale du viewer
    boost::shared_ptr<orientation_2d> m_ori;
    bool m_isOrientationSet;

    global_settings_control* m_globalSettingsControl;

    DECLARE_EVENT_TABLE();
};

#endif // __LAYER_CONTROL_HPP__
