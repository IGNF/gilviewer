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

#include <list>
#include <sstream>
#include <stdexcept>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/bind.hpp>

#include <wx/scrolwin.h>
#include <wx/toolbar.h>
#include <wx/bmpbuttn.h>
#include <wx/checkbox.h>
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/tooltip.h>
#include <wx/sizer.h>
#include <wx/file.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/progdlg.h>
#include <wx/log.h>

#include "../gui/layer_control.hpp"

#include "../layers/image_layer.hpp"
#include "../layers/vector_layer.hpp"
#include "../layers/vector_layer_ghost.hpp"

#include "../gui/vector_layer_settings_control.hpp"
#include "../gui/image_layer_settings_control.hpp"
#include "../gui/global_settings_control.hpp"
#include "../gui/layer_infos_control_impl.h"
#include "../gui/layer_control_utils.hpp"
#include "../gui/panel_viewer.hpp"
#include "../gui/define_id.hpp"

#include "../io/xml_display_configuration_io.hpp"
#include "../io/gilviewer_file_io.hpp"
#include "../io/gilviewer_io_factory.hpp"

#include "../tools/orientation_2d.hpp"
#include "../tools/color_lookup_table.hpp"
#include "GilViewer/tools/pattern_singleton.hpp"

#include "../convenient/utils.hpp"
#include "../convenient/macros_gilviewer.hpp"

#include "../config/config.hpp"

#ifdef _WINDOWS
#   include <wx/msw/winundef.h>
#endif

using namespace boost;
using namespace boost::filesystem;
using namespace std;

BEGIN_EVENT_TABLE(layer_control, wxFrame)
    EVT_TOOL(wxID_OPEN,layer_control::on_open_layer)
    EVT_CLOSE(layer_control::on_close_window)
    EVT_BUTTON(ID_INFO,layer_control::on_info_button)
    EVT_BUTTON(ID_SAVE,layer_control::on_save_button)
    EVT_BUTTON(ID_DELETE,layer_control::on_delete_button)
    EVT_TOOL(wxID_RESET,layer_control::on_reset)
    EVT_BUTTON(wxID_UP,layer_control::on_layer_up)
    EVT_BUTTON(wxID_DOWN,layer_control::on_layer_down)
    EVT_BUTTON(ID_VISIBILITY_BUTTON,layer_control::on_visibility_button)
    EVT_BUTTON(ID_TRANSFORMATION_BUTTON,layer_control::on_transformation_button)
    EVT_BUTTON(ID_GLOBAL_SETTINGS_BUTTON,layer_control::on_global_settings_button)
    EVT_BUTTON(wxID_SAVE,layer_control::on_save_display_config_button)
    EVT_BUTTON(wxID_OPEN,layer_control::on_load_display_config_button)
    EVT_BUTTON(ID_DELETE_ALL_ROWS,layer_control::on_delete_all_rows_button)
END_EVENT_TABLE()

layer_control::layer_control(panel_viewer* DrawPane, wxFrame* parent, wxWindowID id, const wxString& title, long style, const wxPoint& pos, const wxSize& size) :
wxFrame(parent, id, title, pos, size, style), m_ghostLayer(new vector_layer_ghost), m_basicDrawPane(DrawPane), m_ori(shared_ptr<orientation_2d>(new orientation_2d)), m_isOrientationSet(false)
{
    m_layers.reserve(100);

    SetIcon(wxArtProvider::GetIcon(wxART_LIST_VIEW, wxART_TOOLBAR, wxSize(32,32)));
    // Couleur de fond grisee
    wxColour bgcolor(220, 220, 220);
    SetBackgroundColour(bgcolor);
    ClearBackground();

    // Ajout de la barre d'outils
    this->CreateToolBar(wxNO_BORDER | wxTB_HORIZONTAL, 500);
    init_toolbar(this->GetToolBar());

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
    wxBitmapButton* saveDisplayConfig = new wxBitmapButton(m_scroll, wxID_SAVE, wxXmlResource::Get()->LoadBitmap( wxT("MEDIA-FLOPPY_16x16") ) );
    saveDisplayConfig->SetToolTip(_("Save display configuration"));
    globalSettingsSizer->Add(saveDisplayConfig, 0, wxALL | wxALIGN_CENTRE, 5);

    // Bouton pour l'ouverture d'une configuration d'affichage
    wxBitmapButton* loadDisplayConfig = new wxBitmapButton(m_scroll, wxID_OPEN, wxXmlResource::Get()->LoadBitmap( wxT("DOCUMENT-OPEN_16x16") ) );
    loadDisplayConfig->SetToolTip(_("Load display configuration"));
    globalSettingsSizer->Add(loadDisplayConfig, 0, wxALL | wxALIGN_CENTRE, 5);

    // Bouton pour la suppression de tous les layers
    wxBitmapButton* delete_all_rows = new wxBitmapButton(m_scroll, ID_DELETE_ALL_ROWS, wxXmlResource::Get()->LoadBitmap( wxT("USER-TRASH_16x16") ) );
    delete_all_rows->SetToolTip(_("Delete all rows"));
    globalSettingsSizer->Add(delete_all_rows, 0, wxALL | wxALIGN_CENTRE, 5);

    m_sizer->Add(globalSettingsSizer, 0, wxALL | wxALIGN_CENTRE, 5);

    // Quand on cree le layer_control, on en profite pour creer la fenetre de GlobalSettings.
    // On ne l'affiche que sur demande (clic bouton)
    m_globalSettingsControl = new global_settings_control(this);

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

void layer_control::on_close_window(wxCloseEvent& WXUNUSED(event))
{
    // On ne fait que cacher la fenetre. Ca permet de garder les settings courants et de ne pas recalculer tout a chaque fois ...
    Hide();
}

void layer_control::add_row(const string &name, layer_settings_control *layersettings, const string &tooltip)
{
    // Le nom du layer a une longueur maximale de 20 caracteres
    wxString layerName(name.substr(0, 50).c_str(), *wxConvCurrent);
    string ln((const char*)(layerName.mb_str()) );

    // on ajoute la ligne dans le conteneur
    m_rows.push_back(boost::shared_ptr<layer_control_row>(new layer_control_row(this, ln, static_cast<unsigned int>(m_rows.size()), layersettings, tooltip)));
   
    m_sizer->SetRows(static_cast<int>(m_rows.size())+1 );
    // On ajoute a proprement parler les controles de la ligne dans le layer_control
    m_rows.back()->m_nameStaticText->selected(true);
    m_sizer->Add(m_rows.back()->m_nameStaticText, 0, wxTOP | wxALIGN_LEFT, 5);

    m_sizer->Add(m_rows.back()->m_visibilityCheckBox, 0, wxALL | wxALIGN_CENTRE, 5);
    m_rows.back()->m_visibilityCheckBox->SetValue(m_layers.back()->visible());


    m_sizer->Add(m_rows.back()->m_transformationCheckBox, 0, wxALL | wxALIGN_CENTRE, 5);
    m_rows.back()->m_transformationCheckBox->SetValue(m_layers.back()->transformable());

    m_rows.back()->m_saveButton->Enable( m_layers.back()->saveable() );

    m_sizer->Add(m_rows.back()->m_boxSizer, wxGROW);
    m_sizer->Fit(m_scroll);
    m_scroll->Layout();
}

void layer_control::init_toolbar(wxToolBar* toolBar)
{
    if ( !toolBar )
        toolBar = new wxToolBar(m_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTB_HORIZONTAL);

    toolBar->AddTool(wxID_OPEN, wxT("O"), wxXmlResource::Get()->LoadBitmap( wxT("DOCUMENT-OPEN_22x22") ) , wxNullBitmap, wxITEM_NORMAL, _("Open file"));
    toolBar->AddTool(wxID_RESET, wxT("R"), wxXmlResource::Get()->LoadBitmap( wxT("EDIT-CLEAR_22x22") ) , wxNullBitmap, wxITEM_NORMAL, _("Reset"));

    toolBar->Realize();
}

void layer_control::update()
{
    for (unsigned int id = 0;id<m_layers.size();++id)
    {
        m_rows[id]->m_transformationCheckBox->SetValue( m_layers[id]->transformable() );
        m_rows[id]->m_visibilityCheckBox->SetValue( m_layers[id]->visible() );
    }
}

void layer_control::on_info_button(wxCommandEvent& event)
{
    // Get layer index
    unsigned int id = static_cast<unsigned int> (event.GetId()) - static_cast<unsigned int> (ID_INFO);

    wxString title(_("Infos: "));
    title << wxString(m_layers[id]->name().c_str(), *wxConvCurrent);
    //layer_infos_control *lic = new layer_infos_control(m_layers[id]->GetInfos(), this, wxID_ANY, title, wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL | wxCLOSE_BOX);
    layer_infos_control_impl *lic = new layer_infos_control_impl(this);
    lic->m_text_control->AppendText(wxString(m_layers[id]->infos().c_str(), *wxConvCurrent));
    lic->Show();
}

void layer_control::on_save_button(wxCommandEvent& event)
{
    // On commence par recupere l'indice du calque
    unsigned int id = static_cast<unsigned int> (event.GetId()) - static_cast<unsigned int> (ID_SAVE);
    wxString wildcard(m_layers[id]->available_formats_wildcard().c_str(), *wxConvCurrent);
    string file = m_layers[id]->filename();

    #if wxMINOR_VERSION < 9
    wxFileDialog *fileDialog = new wxFileDialog(NULL, _("Save layer"), wxT(""), wxString(file.c_str(), *wxConvCurrent), 
        wildcard, wxFD_SAVE | wxFD_CHANGE_DIR | wxOVERWRITE_PROMPT);
    #else
    wxFileDialog *fileDialog = new wxFileDialog(NULL, _("Save layer"), wxT(""), wxString(file.c_str(), *wxConvCurrent), 
        wildcard, wxFD_SAVE | wxFD_CHANGE_DIR | wxFD_OVERWRITE_PROMPT);
    #endif
    if (fileDialog->ShowModal() == wxID_OK)
    {
        string filename(fileDialog->GetFilename().To8BitData());
        string extension(filesystem::extension(filename));
        extension = extension.substr(1,extension.size()-1);
        to_lower(extension);
        try
        {
            shared_ptr<gilviewer_file_io> file_out = PatternSingleton<gilviewer_io_factory>::instance()->create_object(extension);
            file_out->save(layers()[id],filename);
        }
        catch( std::exception &e )
        {
            GILVIEWER_LOG_EXCEPTION(e.what())
        }
    }
}

void layer_control::on_delete_button(wxCommandEvent& event)
{
    // Get layer index
    unsigned int id = static_cast<unsigned int> (event.GetId()) - static_cast<unsigned int> (ID_DELETE);
    delete_layer(id);
}

void layer_control::on_settings_button(wxCommandEvent& event)
{
    // Get layer index
    unsigned int id = static_cast<unsigned int> (event.GetId()) - static_cast<unsigned int> (ID_SETTINGS);
    m_rows[id]->m_layerSettingsControl->Show(!m_rows[id]->m_layerSettingsControl->IsVisible());
}

void layer_control::on_center_button(wxCommandEvent& event)
{
    /*
    // Get layer index
    unsigned int id = static_cast<unsigned int> (event.GetId()) - static_cast<unsigned int> (ID_CENTER);
    for (layer_control::iterator it = begin(); it != end(); ++it)
    {
        // TODO: handle cartographic or image coordinates
        (*it)->translation_x((*it)->translation_x()-m_layers[id]->center_x());
        (*it)->translation_y((*it)->translation_y()+m_layers[id]->center_y());
        (*it)->needs_update(true);
    }
    m_basicDrawPane->Refresh();
    * */
}

void layer_control::on_refresh_button(wxCommandEvent& event)
{
    unsigned int id = static_cast<unsigned int> (event.GetId()) - static_cast<unsigned int> (ID_REFRESH);

    string ext(extension(m_layers[id]->filename()));
    ext = ext.substr(1,ext.size()-1);
    to_lower(ext);
    try
    {
        shared_ptr<gilviewer_file_io> file = PatternSingleton<gilviewer_io_factory>::instance()->create_object(ext);
        m_layers[id] = file->load(m_layers[id]->filename());
    }
    catch(const std::exception &e)
    {
        GILVIEWER_LOG_EXCEPTION("Unable to refresh layer!")
        return;
    }

    m_layers[id]->needs_update(true);
    m_basicDrawPane->Refresh();
}

void layer_control::on_check_visibility(wxCommandEvent& event)
{
    // On commence par recupere l'indice du calque
    unsigned int id = static_cast<unsigned int> (event.GetId()) - static_cast<unsigned int> (ID_VISIBILITY);
    // On inverse ...
    m_layers[id]->visible(m_rows[id]->m_visibilityCheckBox->GetValue());
    // ... et on refresh
    m_layers[id]->needs_update(true);
    m_basicDrawPane->Refresh();
}

void layer_control::on_check_transformable(wxCommandEvent& event)
{
    // On commence par recupere l'indice du calque
    unsigned int id = static_cast<unsigned int> (event.GetId()) - static_cast<unsigned int> (ID_TRANSFORMATION);
    // On inverse ...
    m_layers[id]->transformable(m_rows[id]->m_transformationCheckBox->GetValue());
    // ... et on refresh
    m_basicDrawPane->Refresh();
}

void layer_control::on_reset(wxCommandEvent& event)
{
    // Pour chaque calque, on reinitialise
    for (layer_control::iterator it = begin(); it != end(); ++it)
    {
        
        (*it)->transform().reset();
        (*it)->needs_update(true);
        (*it)->transformable(true);
        (*it)->visible(true);
    }

    // Et on remet les checkbox en place (true)
    for (unsigned int i = 0; i < m_rows.size(); ++i)
    {
        m_rows[i]->m_visibilityCheckBox->SetValue(true);
        m_rows[i]->m_transformationCheckBox->SetValue(true);
    }

    // On refresh le tout ...
    m_basicDrawPane->Refresh();
}

void layer_control::on_open_layer(wxCommandEvent& event)
{
    string wild = gilviewer_utils::build_wx_wildcard_from_io_factory();
    wxString wildcard(wild.c_str(), *wxConvCurrent);
    wxFileDialog *fileDialog = new wxFileDialog(this, _("Open file"), wxT(""), wxT(""), wildcard, wxFD_OPEN|wxFD_CHANGE_DIR|wxFD_MULTIPLE );

    if (fileDialog->ShowModal() == wxID_OK)
    {
        wxArrayString names;
        fileDialog->GetPaths(names);
        add_layers_from_files(names);
    }
    //fileDialog->Destroy();
    m_basicDrawPane->SetCursor(wxCursor(wxCURSOR_ARROW));

    Layout();
    Refresh();
}

void layer_control::add_layers_from_files(const wxArrayString &names)
{
    unsigned int i;
    m_basicDrawPane->SetCursor(wxCursor(wxCURSOR_WAIT));
    wxProgressDialog *progress = NULL;
    //wxProgressDialog *progressLargeFile = NULL;

    if (names.GetCount() >= 2)
        progress = new wxProgressDialog(_("Opening files ..."), _("Reading ..."), static_cast<unsigned int>(names.GetCount()), NULL, wxPD_AUTO_HIDE | wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME | wxPD_REMAINING_TIME);

    for (i = 0; i < names.GetCount(); ++i)
    {
        if (progress)
        {
            wxString m;
            m << _("Reading file ") << i << wxT("/") << names.GetCount() << wxT("\n");
            m << names[i] << wxT("\n");
            progress->Update(i, m);
        }

        add_layer_from_file(names[i]);
    }

    if (progress) progress->Destroy();
    m_basicDrawPane->SetCursor(wxCursor(wxCURSOR_ARROW));
}


void layer_control::add_layer_from_file( const wxString &name )
{
    string filename((const char*) (name.mb_str()) );
    string extension(filesystem::extension(filename));
    extension = extension.substr(1,extension.size()-1);
    to_lower(extension);
    try
    {
        shared_ptr<gilviewer_file_io> file = PatternSingleton<gilviewer_io_factory>::instance()->create_object(extension);
        add_layer( file->load(filename) );
        m_basicDrawPane->Refresh();
    }
    catch (const std::exception &e)
    {
        GILVIEWER_LOG_EXCEPTION(e.what());
    }
}

void layer_control::add_layer(const layer::ptrLayerType &layer, bool has_transform)
{
    if (!layer) return;

    // On ajoute le calque dans le conteneur
    layer->notify_layer_control( bind( &layer_control::update, this ) );
    m_layers.push_back(layer);

    // On construit le SettingsControl en fonction du type de calque ajoute
    layer_settings_control *settingscontrol = layer->build_layer_settings_control(static_cast<unsigned int>(m_layers.size())-1, this);
    layer->notify_layer_settings_control( bind( &layer_settings_control::update, settingscontrol ) );
    // On ajoute la ligne correspondante
    add_row(layer->name(), settingscontrol, layer->filename());

    //Si c'est un calque image avec ori et que c'est le premier on met en place l'orientation generale du viewer
    if (!m_isOrientationSet && m_layers.size() == 1 && layer->has_ori())
    {
        m_ori = layer->orientation();
        m_isOrientationSet = true;
        GILVIEWER_LOG_MESSAGE("Viewer orientation has been set!");
    }
    else if (!m_isOrientationSet && m_layers.size() > 1 && layer->has_ori())
    {
        GILVIEWER_LOG_MESSAGE("Warning! Image orientation will not be used, because there is no orientation defined for the first displayed image!");
    }
    else if (!m_isOrientationSet && m_layers.size() > 1 && !layer->has_ori() && !has_transform)
    {
        GILVIEWER_LOG_MESSAGE("Image layer position initialised with respect to first image!");
        layer->transform()=m_ghostLayer->transform();

    }

    //Si il y a une orientation definie pour le viewer et pour le nouveau calque image on initialise correctement
    //sa position initiale et son zoom
    if (m_isOrientationSet && layer->has_ori())
    {
        GILVIEWER_LOG_MESSAGE("Image layer position initialised with respect to global orientation!");

        const boost::shared_ptr<orientation_2d> &oriLayer = layer->orientation();

        double newzoom_factor = m_ori->step() / oriLayer->step();
        double translationInitX = (oriLayer->origin_x() - m_ori->origin_x()) / oriLayer->step();
        double translationInitY = -(oriLayer->origin_y() - m_ori->origin_y()) / oriLayer->step();

        layer->transform().zoom_factor(newzoom_factor * m_layers[0]->transform().zoom_factor());
        layer->transform().translation_x(translationInitX + m_layers[0]->transform().translation_x() * newzoom_factor);
        layer->transform().translation_y(translationInitY + m_layers[0]->transform().translation_y() * newzoom_factor);
    }

    //Si il y a une orientation definie pour le viewer et qu'on a affaire a une couche vecteur :
    if (m_isOrientationSet && layer->layer_type_as_string() == "Vector")
    {
        GILVIEWER_LOG_MESSAGE("Vector layer position initialised with respect to global orientation!");

        double translationInitX =-m_ori->origin_x();
        double translationInitY = m_ori->origin_y();

        double newzoom_factor = m_ori->step();
        //layer->zoom_factor(newzoom_factor * m_layers[0]->zoom_factor());
        layer->transform().zoom_factor(m_layers[0]->transform().zoom_factor());
        layer->transform().translation_x(translationInitX + m_layers[0]->transform().translation_x() * newzoom_factor);
        layer->transform().translation_y(translationInitY + m_layers[0]->transform().translation_y() * newzoom_factor);
    }
    layer->default_display_parameters();
    layer->notifyLayerSettingsControl_();


    if(m_isOrientationSet)
        layer->transform().resolution(m_ori->step());
    else
    {
        layer->transform().resolution(1.);
    }

    Refresh();
    m_parent->Refresh();
    m_basicDrawPane->Refresh();
}


layer::ptrLayerType layer_control::get_layer_with_id(unsigned int id)const{
    for(LayerContainerType::const_iterator it=m_layers.begin();it!=m_layers.end();++it)
        if((*it)->getId()==id)
            return (*it);
            
    return layer::ptrLayerType() ;
}

layer::ptrLayerType layer_control::get_layer_with_filename(const string&filename)const
{
    for(LayerContainerType::const_iterator it=m_layers.begin();it!=m_layers.end();++it)
        if((*it)->filename()==filename)
            return (*it);

    return layer::ptrLayerType() ;
}

    
void layer_control::delete_layer(unsigned int index){
    if(m_layers.size()<=index)
        return;
    //Swap
    for (unsigned int i = index; i < m_rows.size() - 1; ++i)
    {
        swap_rows(i, i + 1);
    }

    //Destroy de la row
    m_rows.back()->m_nameStaticText->Destroy();
    m_rows.back()->m_visibilityCheckBox->Destroy();
    m_rows.back()->m_transformationCheckBox->Destroy();
    m_rows.back()->m_infoButton->Destroy();
    m_rows.back()->m_saveButton->Destroy();
    m_rows.back()->m_deleteButton->Destroy();
    m_rows.back()->m_settingsButton->Destroy();
    //m_rows.back()->m_center_button->Destroy();
    m_rows.back()->m_refresh_button->Destroy();
    m_rows.back()->m_layerSettingsControl->Destroy();
    m_sizer->Remove(m_rows.back()->m_boxSizer);

    m_rows.pop_back();

    //Delete de la layer
    m_layers.pop_back();

    //Refresh de la vue
    m_basicDrawPane->Refresh();

    //
    m_sizer->Fit(m_scroll);
    m_scroll->Layout();

    //Orientation : supprimee si tous les calques ont ete supprimes
    if (m_layers.empty())
        m_isOrientationSet = false;

}
    
boost::shared_ptr<orientation_2d> layer_control::orientation() const
{
    return m_ori;
}

void layer_control::swap_rows(const unsigned int firstRow, const unsigned int secondRow)
{
    if (firstRow < 0 || secondRow < 0)
    {
        ostringstream os;
        os << "You passed a negative index for a row !" << endl;
        os << "firstRow = " << firstRow << "  --  secondRow = " << secondRow;
        GILVIEWER_LOG_ERROR(os.str());
        return;
    }
    else if (firstRow >= m_rows.size() || secondRow >= m_rows.size())
    {
        ostringstream os;
        os << "You passed an invalid index for a row !" << endl;
        os << "firstRow = " << firstRow << "  --  secondRow = " << secondRow << endl;
        os << "m_numberOfLayers = " << m_rows.size();
        GILVIEWER_LOG_ERROR(os.str());
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
        m_rows[firstRow]->m_nameStaticText->selected(false);
        m_rows[secondRow]->m_nameStaticText->selected(true);
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
        layer::ptrLayerType temp = m_layers[firstRow];
        m_layers[firstRow] = m_layers[secondRow];
        m_layers[secondRow] = temp;
    }

    //layersettingscontrol
    {
        layer_settings_control *temp = m_rows[firstRow]->m_layerSettingsControl;
        m_rows[firstRow]->m_layerSettingsControl = m_rows[secondRow]->m_layerSettingsControl;
        m_rows[secondRow]->m_layerSettingsControl = temp;

        m_rows[firstRow]->m_layerSettingsControl->index(firstRow);
        m_rows[secondRow]->m_layerSettingsControl->index(secondRow);
    }

    // Finalement, si necessaire, il faut changer le bitmap associe au bouton d'infos
    {
        m_rows[firstRow]->m_infoButton->SetBitmapLabel(wxBitmap(m_rows[firstRow]->m_layerSettingsControl->icon_xpm()));
        m_rows[secondRow]->m_infoButton->SetBitmapLabel(wxBitmap(m_rows[secondRow]->m_layerSettingsControl->icon_xpm()));
    }

    // Ticket #9 : bouton sauvegarde ne bouge pas ...
    {
        m_rows[firstRow]->m_saveButton->Enable( m_layers[firstRow]->saveable() );
        m_rows[secondRow]->m_saveButton->Enable( m_layers[secondRow]->saveable() );
    }
}

void layer_control::on_layer_up(wxCommandEvent& WXUNUSED(event))
{
    for (unsigned int i = 0; i < m_rows.size(); ++i)
    {
        if (m_rows[i]->m_nameStaticText->selected())
        {
            swap_rows(i, i - 1);
            break;
        }
    }

    Refresh();
    m_basicDrawPane->Refresh();

}

void layer_control::on_layer_down(wxCommandEvent& WXUNUSED(event))
{
    for (unsigned int i = 0; i < m_rows.size(); ++i)
    {
        if (m_rows[i]->m_nameStaticText->selected())
        {
            swap_rows(i, i + 1);
            break;
        }
    }

    Refresh();
    m_basicDrawPane->Refresh();
}

void layer_control::on_visibility_button(wxCommandEvent& event)
{
    // On parcourt tous les calques et on inverse la visibilite si la selection est a true
    for (unsigned int i = 0; i < m_rows.size(); ++i)
    {
        if (m_rows[i]->m_nameStaticText->selected() == true)
        {
            m_rows[i]->m_visibilityCheckBox->SetValue(!m_rows[i]->m_visibilityCheckBox->GetValue());
            m_layers[i]->visible(m_rows[i]->m_visibilityCheckBox->GetValue());
        }
    }

    // On refresh le tout ...
    m_basicDrawPane->Refresh();
}

void layer_control::on_transformation_button(wxCommandEvent& event)
{
    // On parcourt tous les calques et on inverse la transformabilite (!!!) si la section est a true
    for (unsigned int i = 0; i < m_rows.size(); ++i)
    {
        if (m_rows[i]->m_nameStaticText->selected() == true)
        {
            m_rows[i]->m_transformationCheckBox->SetValue(!m_rows[i]->m_transformationCheckBox->GetValue());
            m_layers[i]->transformable(m_rows[i]->m_transformationCheckBox->GetValue());
        }
    }

    // On refresh le tout ...
    m_basicDrawPane->Refresh();
}

void layer_control::on_global_settings_button(wxCommandEvent& event)
{
    m_globalSettingsControl->Show();
}

void layer_control::on_save_display_config_button(wxCommandEvent& event)
{
    // Choix du fichier de sauvegarde
    wxFileDialog* fd = new wxFileDialog(this, _("Save display configuration"), wxT(""), wxT(""), wxT("*.xml"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
    if (fd->ShowModal() == wxID_OK)
    {
        string savename((const char*) (fd->GetPath().mb_str()) );
        if (filesystem::extension(savename) != ".xml")
            filesystem::change_extension(savename, ".xml");

        xml_display_configuration_io::write(this, savename.c_str());
    }
    delete fd;
}

void layer_control::on_load_display_config_button(wxCommandEvent& event)
{
    // Choix du fichier de sauvegarde
    wxFileDialog* fd = new wxFileDialog(this, _("Load display configuration"), wxT(""), wxT(""), wxT("*.xml"), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);
    if (fd->ShowModal() == wxID_OK)
    {
        ostringstream os;
        os << _("Reading a display configuration file: ") << (const char*) (fd->GetPath().mb_str());
        GILVIEWER_LOG_MESSAGE(os.str());

        string loadname((const char*) (fd->GetPath().mb_str()) );
        if (filesystem::extension(loadname) != ".xml")
        {
            GILVIEWER_LOG_ERROR("Display configuration file must have the extension .xml !");
            return;
        }

        xml_display_configuration_io::read(this, loadname);
        m_basicDrawPane->Refresh();
    }

    delete fd;
}

void layer_control::on_delete_all_rows_button(wxCommandEvent& event)
{
    while( m_rows.size() > 0 )
    {
        wxCommandEvent event;
        event.SetInt(static_cast<unsigned int>(m_rows.size()));
        on_delete_button(event);
    }
}

void layer_control::create_new_image_layer_with_parameters(const ImageLayerParameters &parameters)
{
    string filename(parameters.path.c_str());
    string extension(filesystem::extension(filename));
    extension = extension.substr(1,extension.size()-1);
    to_lower(extension);
    try
    {
        shared_ptr<gilviewer_file_io> file = PatternSingleton<gilviewer_io_factory>::instance()->create_object(extension);
        layer::ptrLayerType ptr = file->load(filename);
        if (!ptr)
            return;
        add_layer(ptr);

        // Et on sette l'ensemble des parametres qu'on a pu lire ...
        this->m_layers.back()->visible(parameters.visible);
        this->m_layers.back()->transformable(parameters.transformable);
        this->m_layers.back()->alpha(parameters.alpha);
        this->m_layers.back()->gamma(parameters.gamma);
        this->m_layers.back()->intensity_min(parameters.intensity_min);
        this->m_layers.back()->intensity_max(parameters.intensity_max);
        this->m_layers.back()->transparent(parameters.transparent);
        this->m_layers.back()->transparency_min(parameters.transparency_min);
        this->m_layers.back()->transparency_max(parameters.transparency_max);
        this->m_layers.back()->transform().zoom_factor(parameters.zoom_factor);
        this->m_layers.back()->transform().translation_x(parameters.translation_x);
        this->m_layers.back()->transform().translation_y(parameters.translation_y);
        this->m_layers.back()->alpha_channel(parameters.useAlphaChannel,parameters.alphaChannel);
        // TODO: binary or text?
        this->m_layers.back()->colorlookuptable()->load_from_binary_file(parameters.lut_file);

        // MAJ de l'interface
        this->m_layers.back()->notifyLayerControl_();
        this->m_layers.back()->notifyLayerSettingsControl_();
    }
    catch (const std::exception &e)
    {
        GILVIEWER_LOG_EXCEPTION(e.what())
    }
}

void layer_control::create_new_vector_layer_with_parameters(const VectorLayerParameters &parameters)
{
    string filename(parameters.path.c_str());
    string extension(filesystem::extension(filename));
    extension = extension.substr(1,extension.size()-1);
    to_lower(extension);
    try
    {
        shared_ptr<gilviewer_file_io> file = PatternSingleton<gilviewer_io_factory>::instance()->create_object(extension);
        add_layer(file->load(filename) );

        // Et on sette l'ensemble des parametres qu'on a pu lire ...
        this->m_layers.back()->visible(parameters.visible);
        this->m_layers.back()->transformable(parameters.transformable);
        /*
        this->m_layers.back()->SetPointsStyle(parameters.pointsColor, parameters.pointsWidth);
        this->m_layers.back()->SetLinesStyle(parameters.linesColor, parameters.linesWidth, parameters.linesStyle);
        this->m_layers.back()->PolygonsRingsColour(parameters.polygonsRingsColor);
        this->m_layers.back()->PolygonsRingsWidth(parameters.polygonsRingsWidth);
        this->m_layers.back()->PolygonsInsideColour(parameters.polygonsInsideColor);
        this->m_layers.back()->PolygonsRingsStyle(parameters.polygonsRingsStyle);
        this->m_layers.back()->PolygonsInsideStyle(parameters.polygonsInsideStyle);
        */
        //this->m_layers.back()->style(parameters.polygonsInsideColor,parameters.polygonsRingsColor,parameters.polygonsInsideStyle,parameters.polygonsRingsStyle,parameters.polygonsRingsWidth);

        this->m_layers.back()->point_color(parameters.pointsColor);
        this->m_layers.back()->point_width(parameters.pointsWidth);

        this->m_layers.back()->line_color(parameters.linesColor);
        this->m_layers.back()->line_width(parameters.linesWidth);
        this->m_layers.back()->line_style(parameters.linesStyle);

        this->m_layers.back()->polygon_border_color(parameters.polygonsRingsColor);
        this->m_layers.back()->polygon_inner_color(parameters.polygonsInsideColor);
        this->m_layers.back()->polygon_border_width(parameters.polygonsRingsWidth);
        this->m_layers.back()->polygon_border_style(parameters.polygonsRingsStyle);
        this->m_layers.back()->polygon_inner_style(parameters.polygonsInsideStyle);

        this->m_layers.back()->transform().zoom_factor(parameters.zoom_factor);
        this->m_layers.back()->transform().translation_x(parameters.translation_x);
        this->m_layers.back()->transform().translation_y(parameters.translation_y);

        // MAJ de l'interface
        this->m_layers.back()->notifyLayerControl_();
        this->m_layers.back()->notifyLayerSettingsControl_();
    }
    catch (std::exception &e)
    {
        GILVIEWER_LOG_EXCEPTION(e.what())
    }
}
