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

#ifdef _WINDOWS
#   define NOMINMAX
#endif

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/bind.hpp>
#ifdef WIN32
	#pragma warning(disable : 4251)
	#pragma warning(disable : 4275)
#endif
#include <wx/xrc/xmlres.h>
#include <wx/dcbuffer.h>
#include <wx/confbase.h>
#include <wx/dataobj.h>
#include <wx/clipbrd.h>
#include <wx/toolbar.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/image.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/filedlg.h>

#include "../layers/vector_layer_ghost.hpp"
#include "../layers/vector_layer.hpp"
#include "../gui/layer_control_utils.hpp"
#include "../gui/layer_control.hpp"
#include "../gui/define_id.hpp"
#include "../gui/panel_manager.hpp"

#include "../tools/orientation_2d.hpp"
#include "../plugins/plugin_manager.hpp"
#include "../convenient/wxrealpoint.hpp"

#include "GilViewer/config/config_plugins.hpp"
#include "GilViewer/io/gilviewer_io_factory.hpp"


#ifdef _WINDOWS
#   include <wx/msw/winundef.h>
#endif

BEGIN_EVENT_TABLE(panel_viewer, wxPanel)
        EVT_PAINT(panel_viewer::on_paint)
        EVT_SIZE(panel_viewer::on_size)
        EVT_MOTION(panel_viewer::on_mouse_move)
        EVT_LEFT_DOWN(panel_viewer::on_left_down)
        EVT_LEFT_UP(panel_viewer::on_left_up)
        EVT_MIDDLE_DOWN(panel_viewer::on_middle_down)
        EVT_RIGHT_DOWN(panel_viewer::on_right_down)
        EVT_LEFT_DCLICK(panel_viewer::on_left_double_click)
        EVT_RIGHT_DCLICK(panel_viewer::on_right_double_click)
        EVT_MOUSEWHEEL(panel_viewer::on_mouse_wheel)
        EVT_KEY_DOWN(panel_viewer::on_keydown)
        ADD_GILVIEWER_EVENTS_TO_TABLE(panel_viewer)
        END_EVENT_TABLE()

        IMPLEMENTS_GILVIEWER_METHODS_FOR_EVENTS_TABLE(panel_viewer,this)

        using namespace std;

void panel_viewer::on_size(wxSizeEvent &e) {
    for (layer_control::iterator it = m_layerControl->begin(); it != m_layerControl->end(); ++it)
        (*it)->needs_update(true);
}


template<typename Event>
panel_viewer::eMode panel_viewer::mode(Event& event) const
{
    if(event.ShiftDown()) return MODE_NAVIGATION;
    return m_mode;
}

void panel_viewer::mode_navigation     () { m_mode = MODE_NAVIGATION; }
void panel_viewer::mode_capture        () { m_mode = MODE_CAPTURE; }
void panel_viewer::mode_geometry_moving() { m_mode = MODE_GEOMETRY_MOVING; }
void panel_viewer::mode_edition        () { m_mode = MODE_EDITION; }
void panel_viewer::mode_selection      () { m_mode = MODE_SELECTION; }

void panel_viewer::geometry_null     () { m_ghostLayer->reset<vector_layer_ghost::Nothing>  (); Refresh(); }
void panel_viewer::geometry_point    () { m_ghostLayer->reset<vector_layer_ghost::Point>    (); Refresh(); }
void panel_viewer::geometry_circle   () { m_ghostLayer->reset<vector_layer_ghost::Circle>   (); Refresh(); }
void panel_viewer::geometry_rectangle() { m_ghostLayer->reset<vector_layer_ghost::Rectangle>(); Refresh(); }
void panel_viewer::geometry_line     () { m_ghostLayer->reset<vector_layer_ghost::Polyline> (); Refresh(); }
void panel_viewer::geometry_polygon  () { m_ghostLayer->reset<vector_layer_ghost::Polygon>  (); Refresh(); }

layer_control* panel_viewer::layercontrol() const {
    return m_layerControl;
}


panel_viewer::panel_viewer(wxFrame* parent, wxAuiManager *dockmanager) :
        wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS), m_parent(parent), m_mainToolbar(NULL), m_modeAndGeometryToolbar(NULL), m_menuBar(NULL),
        //m_menuMain(NULL),
        m_mouseMovementStarted(false), m_translationDrag(0, 0),
        // Construction des differentes fenetres liees au PanelViewer :
        //      - layer control
        m_layerControl(new layer_control(this, parent, wxID_ANY, _("Layers control"))),
        //      - applications settings
        //reference au ghostLayer du LayerControl
        m_ghostLayer(layercontrol()->m_ghostLayer),
        //Setting des modes d'interface :
        m_mode(MODE_NAVIGATION), m_snap(SNAP_ALL)
{

#if wxUSE_DRAG_AND_DROP
    SetDropTarget(new gilviewer_file_drop_target(this));
#endif // wxUSE_DRAG_AND_DROP
    // Avoids flickering effect under windows !!!
    //  SetFocus();
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    this->init_toolbar();

    /// Menubar
    m_menuFile = new wxMenu;
    m_menuFile->Append(wxID_OPEN, _("Open\tCtrl-O"));

    m_menuAbout = new wxMenu;
    m_menuAbout->Append(wxID_ABOUT, wxT("?"));

    m_menuBar = new wxMenuBar;
    m_menuBar->Insert(0, m_menuFile, _("File"));
    m_menuBar->Insert(1, m_menuAbout, _("About ..."));
    m_menuBar->SetParent(parent);


    m_bgbrush.SetColour(wxColor(200,200,200));
    
    //  /////Popup menu
    //  m_menuMain = new wxMenu;
    //  //  wxMenu* menuRemove = new wxMenu;
    //  //  wxMenu* menuVisibility = new wxMenu;
    //  //  wxMenu* menuTransformability = new wxMenu;
    //  //
    //  //  for (unsigned int i = 0; i < m_layerControl->GetNumberOfLayers(); ++i)
    //  //  {
    //  //      unsigned int index_row = m_layerControl->GetRows()[i]->m_index;
    //  //      menuRemove->Append(ID_POPMENU_REMOVE + index_row, wxString(m_layerControl->Layers()[i]->Name().c_str(), *wxConvCurrent));
    //  //      menuVisibility->AppendCheckItem(ID_POPMENU_VISIBILITY + index_row, wxString(m_layerControl->Layers()[i]->Name().c_str(), *wxConvCurrent));
    //  //      menuTransformability->AppendCheckItem(ID_POPMENU_TRANSFORMABILITY + index_row, wxString(m_layerControl->Layers()[i]->Name().c_str(), *wxConvCurrent));
    //  //      menuVisibility->Check(ID_POPMENU_VISIBILITY + index_row, m_layerControl->Layers()[i]->IsVisible());
    //  //      menuTransformability->Check(ID_POPMENU_TRANSFORMABILITY + index_row, m_layerControl->Layers()[i]->IsTransformable());
    //  //  }
    //
    //  m_menuMain->Append(wxID_NEW, _("New"));
    //  m_menuMain->Append(wxID_OPEN, _("Open"));
    //  m_menuMain->AppendSeparator();
    //  //  m_menuMain->AppendSubMenu(menuRemove, _("Remove layer ...") );
    //  //  m_menuMain->AppendSubMenu(menuVisibility, _("Change layer visibility ...") );
    //  //  m_menuMain->AppendSubMenu(menuTransformability, _("Change layer transformability ...") );
    //  //  m_menuMain->AppendSeparator();
    //  m_menuMain->Append(ID_BASIC_SNAPSHOT, _("Screenshot\tCtrl-C"));
    //
    //  //  wxMenu* menuMode = new wxMenu;
    //  m_menuMain->AppendSeparator();
    //  //  m_menuMain->AppendSubMenu(menuMode, _("Mode"));
    //  m_menuMain->AppendRadioItem(ID_MODE_NAVIGATION, _("Navigation"));
    //  m_menuMain->AppendRadioItem(ID_MODE_CAPTURE, _("Saisie"));
    //  m_menuMain->AppendRadioItem(ID_MODE_GEOMETRY_MOVING, _("Move"));
    //  m_menuMain->AppendRadioItem(ID_MODE_EDITION, _("Edition"));
    //  m_menuMain->AppendRadioItem(ID_MODE_SELECTION, _("Selection"));
    //  m_menuMain->AppendRadioItem(ID_SINGLE_CROP, _("Crop"));
    //
    //  m_menuMain->AppendSeparator();
    //  m_menuMain->Append(wxID_ABOUT, _("About"));
    //
    //  //  wxMenu* menuGeometry = new wxMenu;
    //  //  m_menuMain->AppendSubMenu(menuGeometry, _("Geometry"));
    //  m_menuMain->AppendSeparator();
    //  m_menuMain->AppendRadioItem(ID_GEOMETRY_NULL, _("Nothing"));
    //  m_menuMain->AppendRadioItem(ID_GEOMETRY_POINT, _("Point"));
    //  m_menuMain->AppendRadioItem(ID_GEOMETRY_LINE, _("Line"));
    //  m_menuMain->AppendRadioItem(ID_GEOMETRY_CIRCLE, _("Circle"));
    //  m_menuMain->AppendRadioItem(ID_GEOMETRY_RECTANGLE, _("Rectangle"));
    //  m_menuMain->AppendRadioItem(ID_GEOMETRY_POLYGONE, _("Polygon"));

    ///Shortcuts
    wxAcceleratorEntry entries[3];
    entries[0].Set(wxACCEL_CTRL, (int) 'C', ID_BASIC_SNAPSHOT);
    entries[1].Set(wxACCEL_ALT, (int) 'N', ID_MODE_NAVIGATION);
    entries[2].Set(wxACCEL_ALT, (int) 'C', ID_MODE_CAPTURE);
    wxAcceleratorTable acceleratorTable(3, entries);
    this->SetAcceleratorTable(acceleratorTable);

    register_all_file_formats(PatternSingleton<gilviewer_io_factory>::instance());
    m_plugin_manager = new plugin_manager;

#ifndef _WINDOWS
    wxConfigBase *pConfig = wxConfigBase::Get();
    wxString str;
    pConfig->Read(wxT("/Paths/Plugins"), &str, wxString(plugins_dir.c_str(), *wxConvCurrent));
	std::cout<<str.mb_str()<<std::endl;
    m_plugin_manager->register_plugins( (const char *) str.mb_str(), m_menuBar, dockmanager, m_parent);
#endif // _WINDOWS

    // Log all available formats ...
    std::vector<factory_key> ids = PatternSingleton<gilviewer_io_factory>::instance()->available_identifiers();
    std::ostringstream mes;
    mes << "Available file formats:";
    for(vector<factory_key>::const_iterator it=ids.begin(); it!=ids.end(); ++it)
        mes << it->extension << " ";
    GILVIEWER_LOG_MESSAGE(mes.str());
}

wxToolBar* panel_viewer::main_toolbar(wxWindow* parent, wxAuiManager *dockmanager) {

    if (!m_mainToolbar) {
        m_mainToolbar = new wxToolBar(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTB_HORIZONTAL);

        m_mainToolbar->AddTool(wxID_OPEN, wxT("O"), wxXmlResource::Get()->LoadBitmap(wxT("DOCUMENT-OPEN_22x22")), wxNullBitmap, wxITEM_NORMAL, _("Open layer"));
        m_mainToolbar->AddTool(ID_SHOW_HIDE_LAYER_CONTROL, wxT("SHLC"), wxArtProvider::GetIcon(wxART_LIST_VIEW, wxART_TOOLBAR, wxSize(22, 22)), wxNullBitmap, wxITEM_NORMAL, _("Show / Hide layer control"));

        m_mainToolbar->AddTool(ID_BASIC_SNAPSHOT, wxT("S"), wxXmlResource::Get()->LoadBitmap(wxT("CAMERA_PHOTO_22x22")), wxNullBitmap, wxITEM_NORMAL, _("Snapshot"));

        m_mainToolbar->AddTool(wxID_PREFERENCES, wxT("AS"), wxXmlResource::Get()->LoadBitmap(wxT("APPLICATIONS-SYSTEM_22x22")), wxNullBitmap, wxITEM_NORMAL, _("Application settings"));

        m_mainToolbar->AddTool(wxID_HELP, wxT("AS"), wxXmlResource::Get()->LoadBitmap(wxT("HELP_22x22")), wxNullBitmap, wxITEM_NORMAL, _("Help"));
        m_mainToolbar->Realize();

        wxAuiPaneInfo info;
        info.ToolbarPane();
        info.Caption( _("Main Toolbar") );
        info.TopDockable();
        info.Top();
        info.Fixed();
        info.Resizable(false);
        info.CloseButton(false);
        info.CaptionVisible(false);

        dockmanager->AddPane(m_mainToolbar,info);
    }

    return m_mainToolbar;
}

wxToolBar* panel_viewer::mode_and_geometry_toolbar(wxWindow* parent, wxAuiManager *dockmanager) {
    if (!m_modeAndGeometryToolbar) {
        m_modeAndGeometryToolbar = new wxToolBar(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTB_HORIZONTAL);

        m_modeAndGeometryToolbar->AddTool(ID_MODE_NAVIGATION, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("HAND_22x22")), wxNullBitmap, wxITEM_RADIO, _("Navigation"));
        m_modeAndGeometryToolbar->AddTool(ID_MODE_CAPTURE, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("PEN_22x22")), wxNullBitmap, wxITEM_RADIO, _("Saisie"));
        m_modeAndGeometryToolbar->AddTool(ID_MODE_EDITION, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("APPLICATIONS_OFFICE_22x22")), wxNullBitmap, wxITEM_RADIO, _("Edition"));
        m_modeAndGeometryToolbar->AddTool(ID_MODE_GEOMETRY_MOVING, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("GEOMETRY_MOVING_22x22")), wxNullBitmap, wxITEM_RADIO, _("Geometry moving"));
        m_modeAndGeometryToolbar->AddTool(ID_MODE_SELECTION, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("SELECTION_22x22")), wxNullBitmap, wxITEM_RADIO, _("Selection"));

        m_modeAndGeometryToolbar->AddSeparator();
        m_modeAndGeometryToolbar->AddTool(ID_GEOMETRY_NULL, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("STOP_22x22")), wxNullBitmap, wxITEM_RADIO, _("None"));
        m_modeAndGeometryToolbar->AddTool(ID_GEOMETRY_POINT, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("POINTS_22x22")), wxNullBitmap, wxITEM_RADIO, _("Point"));
        m_modeAndGeometryToolbar->AddTool(ID_GEOMETRY_CIRCLE, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("CIRCLE_22x22")), wxNullBitmap, wxITEM_RADIO, _("Circle"));
        m_modeAndGeometryToolbar->AddTool(ID_GEOMETRY_LINE, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("POLYLINE_22x22")), wxNullBitmap, wxITEM_RADIO, _("Line"));
        m_modeAndGeometryToolbar->AddTool(ID_GEOMETRY_RECTANGLE, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("RECTANGLE_22x22")), wxNullBitmap, wxITEM_RADIO, _("Rectangle"));
        m_modeAndGeometryToolbar->AddTool(ID_GEOMETRY_POLYGONE, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("POLYGON_22x22")), wxNullBitmap, wxITEM_RADIO, _("Polygon"));

        m_modeAndGeometryToolbar->AddSeparator();
        m_modeAndGeometryToolbar->AddTool(ID_CROP, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("CROP_22x22")), wxNullBitmap, wxITEM_NORMAL, _("Crop raster"));

        m_modeAndGeometryToolbar->Realize();

        wxAuiPaneInfo info;
        info.ToolbarPane();
        info.Caption( _("Mode and geometry Toolbar") );
        info.TopDockable();
        info.Top();
        info.Fixed();
        info.Resizable(false);
        info.CloseButton(false);
        info.CaptionVisible(false);

        dockmanager->AddPane(m_modeAndGeometryToolbar,info);
    }

    return m_modeAndGeometryToolbar;
}

wxMenuBar* panel_viewer::menubar() {
    return m_menuBar;
}

bool panel_viewer::init_toolbar() {

    return true;
}

void panel_viewer::on_paint(wxPaintEvent& evt) {
    wxBufferedPaintDC dc(this);
    if (!dc.IsOk())
        return;

    dc.SetBackgroundMode    ( wxSOLID);     
    dc.SetBackground    (m_bgbrush);     
    dc.Clear();
    wxSize tailleImage(this->GetSize());
    int dx = static_cast<int> (m_translationDrag.x);
    int dy = static_cast<int> (m_translationDrag.y);

    for (layer_control::iterator it = m_layerControl->begin(); it != m_layerControl->end(); ++it) {
        if ((*it)->visible()) {
            if ((*it)->needs_update()) {
                try {
                    (*it)->update(tailleImage.GetX(), tailleImage.GetY());
                } catch (const std::exception &e) {
                    GILVIEWER_LOG_EXCEPTION(e.what())
                    wxMessageBox( _("Exception: see log!"), _("Exception!"), wxICON_ERROR);
                    return;
                }
                (*it)->needs_update(false);
            }

            if ((*it)->transformable()) {
                (*it)->draw(dc,dx,dy, true);
            } else {
                (*it)->draw(dc, 0, 0, true);
            }
        }
    }
    m_ghostLayer->draw(dc,dx,dy,false);
}

void panel_viewer::on_left_down(wxMouseEvent &event) {
    m_mouseMovementInit = snap(event);

    // on commence un cliquer-glisser pour bouger
    m_mouseMovementStarted = true;
    SetCursor(wxCursor(wxCURSOR_HAND));

    ///saisie d'un point
    switch(mode(event))
    {
    case MODE_NAVIGATION :
        break;
    case MODE_CAPTURE :
    case MODE_SELECTION :
        geometry_add_point(m_mouseMovementInit);
        break;
    default:
        break;
    }
}

void panel_viewer::on_left_up(wxMouseEvent &event) {
    ///si on était en mode navigation
    if (m_mouseMovementStarted) {
        SetCursor(wxCursor(wxCURSOR_ARROW));
        m_mouseMovementStarted = false;

        m_translationDrag.x = 0;
        m_translationDrag.y = 0;

        update_if_transformable();

        Refresh();
    }
}

void panel_viewer::on_right_down(wxMouseEvent &event) {
    //  /////Clique droit popup menu
    //  if (event.m_shiftDown) //pour ne pas interférer avec le double-click !
    //  {
    //      wxPoint point = event.GetPosition();
    //      // If from keyboard
    //      if (point.x == -1 && point.y == -1) {
    //          wxSize size = GetSize();
    //          point.x = size.x / 2;
    //          point.y = size.y / 2;
    //      }
    //      ShowPopUpMenu(point);
    //  } else

    switch(mode(event))
    {
    case MODE_SELECTION :
    case MODE_CAPTURE :
        geometry_add_point(snap(event),true);
        break;
    default:
        break;
    }
}

void panel_viewer::on_left_double_click(wxMouseEvent &event) {
    switch(mode(event))
    {
    case MODE_NAVIGATION :
        {
            wxConfigBase *pConfig = wxConfigBase::Get();
            double zoom_;
            pConfig->Read(wxT("/Options/Zoom"), &zoom_, 0.5);
            zoom(zoom_, event);
            break;
        }
    default:
        break;
    }
    update_statusbar(snap(event));
}

void panel_viewer::on_right_double_click(wxMouseEvent &event) {
    switch(mode(event))
    {
    case MODE_NAVIGATION :
        {
            wxConfigBase *pConfig = wxConfigBase::Get();
            double dezoom;
            pConfig->Read(wxT("/Options/Dezoom"), &dezoom, 2.);
            zoom(dezoom, event);
            break;
        }
    default:
        break;
    }
    update_statusbar(snap(event));
}

void panel_viewer::on_middle_down(wxMouseEvent & event) {
    ///Affichage du layer control
    wxPoint pos = event.GetPosition();
    pos += GetScreenPosition();
    m_layerControl->SetPosition(pos);

    show_layer_control(!m_layerControl->IsVisible());

    event.Skip();
    update_statusbar(snap(event));
}

void panel_viewer::on_mouse_wheel(wxMouseEvent& event) {
    ////Zoom
    wxConfigBase *pConfig = wxConfigBase::Get();
    double zoom_, dezoom;
    pConfig->Read(wxT("/Options/Zoom"), &zoom_, 0.5);
    pConfig->Read(wxT("/Options/Dezoom"), &dezoom, 2.);
    int deltaMouseWheel = event.GetWheelRotation();
    double zoom_factor;
    if (deltaMouseWheel < 0)
        zoom_factor = dezoom;
    else
        zoom_factor = zoom_;
    zoom(zoom_factor, event);
    update_statusbar(snap(event));
}

void panel_viewer::on_mouse_move(wxMouseEvent &event) {
    wxRealPoint p = snap(event);
    //déplacement (de l'image ou de la géométrie) si on a bouton gauche enfoncé (m_mouseMovementStarted)
    if (m_mouseMovementStarted) {
        wxRealPoint translation(p-m_mouseMovementInit);
        m_mouseMovementInit = p;

        switch(mode(event))
        {
        case MODE_NAVIGATION :
            m_translationDrag += translation;
            scene_move(translation);
            break;
        case MODE_GEOMETRY_MOVING :
            ///si en mode geometry moving : c'est la géométrie qui bouge !!
            geometry_move_absolute(p);
            break;
        default:
            break;
        }
    } else {
        switch(mode(event))
        {
        case MODE_SELECTION :
        case MODE_CAPTURE :
            geometry_update_absolute(p);
            break;
        default:
            break;
        }//scwith
    }

    for(unsigned int i=0;i<m_plugin_manager->size();i=i+1)
    {
        plugin_base *p = m_plugin_manager->at(i);
        if(wx_plugin_base *wxp = dynamic_cast<wx_plugin_base *>(p))
            wxp-> on_mouse_move(event);
    }
    update_statusbar(p);
    //  SetFocus();
}

void panel_viewer::on_keydown(wxKeyEvent& event) {
    int step;
    if (event.m_controlDown)
        step = 50;
    else
        step = 1;

    wxRealPoint deplacement(0, 0);
    //déplacement effectif
    if (event.m_keyCode == WXK_RIGHT)
        deplacement = wxRealPoint(step, 0);
    else if (event.m_keyCode == WXK_LEFT)
        deplacement = wxRealPoint(-step, 0);
    else if (event.m_keyCode == WXK_DOWN)
        deplacement = wxRealPoint(0, step);
    else if (event.m_keyCode == WXK_UP)
        deplacement = wxRealPoint(0, -step);

    if (event.m_keyCode == WXK_RIGHT || event.m_keyCode == WXK_LEFT || event.m_keyCode == WXK_UP || event.m_keyCode == WXK_DOWN) {
        ///si on est en mode navigation (ou qu'on appuie sur shift -> force le mode navigation) : déplacement de l'image

        switch(mode(event))
        {
        case MODE_NAVIGATION :
            update_if_transformable();
            scene_move(deplacement);
            break;
        case MODE_CAPTURE :
            geometry_update_relative(deplacement);
            break;
        case MODE_GEOMETRY_MOVING :
            geometry_move_relative(deplacement);
            break;
        default:
            break;
        }
    }

    event.Skip();
    update_statusbar(snap(event));

}

void panel_viewer::update_if_transformable() {
    for (layer_control::iterator it = m_layerControl->begin(); it != m_layerControl->end(); ++it) {
        if ((*it)->transformable()) {
            (*it)->needs_update(true);
        }
    }
}

void panel_viewer::scene_move(const wxRealPoint& translation) {
    for (layer_control::iterator it = m_layerControl->begin(); it != m_layerControl->end(); ++it)
        if ((*it)->transformable()) (*it)->transform().translate(translation);
    m_ghostLayer->transform().translate(translation);
    Refresh();
}
/*
bool panel_viewer::coord_image(const int mouseX, const int mouseY, int &i, int &j) const {
    bool coordOK = false;
    for (layer_control::iterator it = m_layerControl->begin(); it != m_layerControl->end() && !coordOK; ++it) {
        if ((*it)->visible() && ((*it)->pixel_value(i, j).length() != 0)) {
            coordOK = true;
            double i_,j_;
            (*it)->transform().to_local((double)mouseX,(double)mouseY,i_,j_);
            i=static_cast<int> (i_);
            j=static_cast<int> (j_);
        }
    }
    return coordOK;
}

bool panel_viewer::subpix_coord_image(const int mouseX, const int mouseY, double &i, double &j) const {
    bool coordOK = false;
    for (layer_control::iterator it = m_layerControl->begin(); it != m_layerControl->end() && !coordOK; ++it) {
        if ((*it)->visible()) {
            coordOK = true;
            (*it)->transform().to_local(mouseX,mouseY,i,j);
        }
    }
    return coordOK;
}
*/
void panel_viewer::open_custom_format(const std::string &filename) {
    std::ostringstream oss;
    oss << "File : " << __FILE__ << "\n";
    oss << "Function : " << __FUNCTION__ << "\n";
    oss << "Line : " << __LINE__ << "\n";
    oss << "Format non supporté !\n";
    ::wxMessageBox(wxString(oss.str().c_str(), *wxConvCurrent));
}

void panel_viewer::update_statusbar(const wxRealPoint& p) {
    // On n'update que lorsque la status bar existe chez le parent ...
    if (m_parent->GetStatusBar()) {
        unsigned int nb = 0;
        for (layer_control::iterator it = m_layerControl->begin(); it != m_layerControl->end(); ++it) {
            if ((*it)->visible()) // && ((*it)->GetPixelValue(i, j).length() != 0))
                nb++;
        }

        if (nb == 0)
            return;

        boost::shared_ptr<orientation_2d> ori = boost::shared_ptr<orientation_2d>(new orientation_2d);
        if (m_layerControl->oriented()) {
            ++nb; //on affiche aussi les coord carto dans ce cas
            ori = m_layerControl->orientation();
        }

        m_parent->GetStatusBar()->SetFieldsCount(nb + 2); //+1 pour les coord en pixels

        unsigned int count = 0;
        bool affichageCartoDone = false;


        std::ostringstream coordGlob;
                    coordGlob << "Coord Glob : (";
                    coordGlob << p.x;
                    coordGlob << ",";
                    coordGlob << p.y;
                    coordGlob << ")";
                    m_parent->SetStatusText(wxString(coordGlob.str().c_str(), *wxConvCurrent), count);
                    ++count;
                    
        {
            std::ostringstream coordPixel;
            wxRealPoint q = m_ghostLayer->transform().to_local(p);
            coordPixel << "Global (" << q.x << "," << q.y << ")";
            m_parent->SetStatusText(wxString(coordPixel.str().c_str(), *wxConvCurrent), count);
            ++count;

            if (!affichageCartoDone && m_layerControl->oriented()) {
                // MB : coordonnées carto du ghost layer, a vérifier....
                affichageCartoDone = true;
                std::ostringstream coordCarto;
                coordCarto << "Carto : (";
                coordCarto << static_cast<int> (ori->origin_x() + ori->step() * (-m_ghostLayer->transform().translation_x() + p.x * m_ghostLayer->transform().zoom_factor()));
                coordCarto << ",";
                coordCarto << static_cast<int> (ori->origin_y() + ori->step() * (m_ghostLayer->transform().translation_y() - p.y * m_ghostLayer->transform().zoom_factor()));
                coordCarto << ")";
                m_parent->SetStatusText(wxString(coordCarto.str().c_str(), *wxConvCurrent), count);
                ++count;
            }
        }
        for (layer_control::iterator it = m_layerControl->begin(); it != m_layerControl->end(); ++it) {
            if ((*it)->visible())
            {
                wxRealPoint q = (*it)->transform().to_local(p);
                std::ostringstream affichage;
                affichage << boost::filesystem::basename((*it)->name());
                affichage << "-" << 100. / (*it)->transform().zoom_factor() << "% : ";
                affichage << "("<<q.x<<","<<q.y<<") ";
                affichage << (*it)->pixel_value(p);
                m_parent->SetStatusText(wxString(affichage.str().c_str(), *wxConvCurrent), count);
                ++count;
            }
        }
    }
}

void panel_viewer::zoom(double zoom_factor, wxMouseEvent &event) {
    for (layer_control::iterator it = m_layerControl->begin(); it != m_layerControl->end(); ++it) {
        if ((*it)->transformable()) {
            (*it)->transform().zoom(zoom_factor,event.GetPosition().x,event.GetPosition().y);
            (*it)->needs_update(true);
        }
    }
    m_ghostLayer->transform().zoom(zoom_factor,event.GetPosition().x,event.GetPosition().y);
    Refresh();
}

/*
 void PanelViewer::OnChar(wxKeyEvent& event)
 {
 if (event.GetKeyCode()==(int)'l')
 m_layerControl->Show( !m_layerControl->IsVisible() );
 }
 */


void panel_viewer::add_layer(const layer::ptrLayerType &layer, bool has_transform) {
    try {
        m_layerControl->add_layer(layer, has_transform);
    } catch (const std::exception &e) {
        GILVIEWER_LOG_EXCEPTION(e.what())
            }
}

void panel_viewer::delete_layer( unsigned int index){
    m_layerControl->delete_layer(index);
}

layer::ptrLayerType panel_viewer::get_layer_with_id(unsigned int id)const{
    return m_layerControl->get_layer_with_id(id);
}

void panel_viewer::show_layer_control(bool show) const {
    m_layerControl->Show(show);
}

//void PanelViewer::ShowPopUpMenu(const wxPoint& pos) {
//
//  PopupMenu(m_menuMain, pos.x, pos.y);
//}

void panel_viewer::on_quit(wxCommandEvent& event) {
    Close();
}

void panel_viewer::snap_shot(wxCommandEvent& event) {
    {
        // On recupere le DC, on le met dans un bitmap et on copie ce bitmap (pour le coller ensuite) ...
        wxBufferedPaintDC dc(this);
        int width, height;
        this->GetClientSize(&width, &height);
        if (!dc.IsOk())
            return;
        wxBitmap snap = dc.GetSelectedBitmap();
        snap.SetHeight(height);
        snap.SetWidth(width);

        wxBitmapDataObject *toto = new wxBitmapDataObject(snap);

        if (wxTheClipboard->Open()) {
            wxTheClipboard->SetData(toto);
            wxTheClipboard->Close();
        }
    }
}

void panel_viewer::execute_mode() {
    Refresh();

    switch (m_mode) {
    case MODE_NAVIGATION:
        execute_mode_navigation();
        break;
    case MODE_GEOMETRY_MOVING:
        execute_mode_geometry_moving();
        break;
    case MODE_CAPTURE:
        execute_mode_capture();
        break;
    case MODE_EDITION:
        execute_mode_edition();
        break;
    case MODE_SELECTION:
        execute_mode_selection();
        break;
    }
}

void panel_viewer::execute_mode_navigation     () {}
void panel_viewer::execute_mode_geometry_moving() {}
void panel_viewer::execute_mode_capture        () {}
void panel_viewer::execute_mode_edition        () {}
void panel_viewer::execute_mode_selection      () {}

wxRealPoint panel_viewer::snap(const wxRealPoint& p) const
{
    wxRealPoint q=p;
    if(m_snap)
    {
        double d2[SNAP_MAX_ID];
        if(m_snap&SNAP_GRID ) d2[SNAP_GRID ]=10*10; // squared snap tolerance in [grid steps squared]
        if(m_snap&SNAP_POINT) d2[SNAP_POINT]=10*10; // squared snap tolerance in [screen pixels squared]
        if(m_snap&SNAP_LINE ) d2[SNAP_LINE ]=10*10; // squared snap tolerance in [screen pixels squared]
        for (layer_control::iterator it = m_layerControl->begin(); it != m_layerControl->end(); ++it) {
            (*it)->snap(m_snap,d2,p,q);
        }
        m_ghostLayer->snap(m_snap,d2,p,q);
    }
    return q;
}

void panel_viewer::geometry_add_point(const wxRealPoint& p, bool final)
{
    if(vectorlayerghost()->complete())
        vectorlayerghost()->reset();
    if(m_ghostLayer->add_point(p,final))
        geometry_end();
    Refresh();
}
void panel_viewer::geometry_move_relative  (const wxRealPoint& p)
{
    m_ghostLayer->move_relative(p);
    execute_mode();
}
void panel_viewer::geometry_move_absolute  (const wxRealPoint& p)
{
    m_ghostLayer->move_absolute(p);
    execute_mode();
}
void panel_viewer::geometry_update_absolute(const wxRealPoint& p)
{
    m_ghostLayer->update_absolute(p);
    execute_mode();
}
void panel_viewer::geometry_update_relative(const wxRealPoint& p)
{
    m_ghostLayer->update_relative(p);
    execute_mode();
}

void panel_viewer::geometry_end()
{
    execute_mode();
}

panel_viewer* create_panel_viewer(wxFrame* parent, wxAuiManager *dockmanager) {
    return new panel_viewer(parent,dockmanager);
}

void panel_viewer::Register(wxFrame* parent, wxAuiManager *dockmanager) {
    panel_manager::instance()->Register("PanelViewer", boost::bind(create_panel_viewer, parent, dockmanager));
}

#if wxUSE_DRAG_AND_DROP
bool gilviewer_file_drop_target::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
    // Je ne suis pas certain que ça fonctionne comme ça ...
    m_panelViewer->layercontrol()->add_layers_from_files(filenames);
    return true;
}
#endif // wxUSE_DRAG_AND_DROP
void panel_viewer::crop() {
    // On recherche le calque selectionne
    unsigned int i = 0;
    std::vector<layer::ptrLayerType> selected_layers;
    for (layer_control::iterator it = m_layerControl->begin(); it != m_layerControl->end() && i < static_cast<unsigned int>(m_layerControl->rows().size()); ++it, ++i) {
        if (m_layerControl->rows()[i]->m_nameStaticText->selected()) {
            selected_layers.push_back(*it);
        }
    }
    const vector_layer_ghost::Rectangle *rect = m_ghostLayer->get<vector_layer_ghost::Rectangle>();
    if(!rect) return;
    wxRealPoint p0(m_ghostLayer->transform().from_local(rect->first ));
    wxRealPoint p1(m_ghostLayer->transform().from_local(rect->second));
    for(std::vector<layer::ptrLayerType>::const_iterator it=selected_layers.begin(); it!=selected_layers.end(); ++it) {
        try {
            layer::ptrLayerType layer = (*it)->crop(p0,p1);
            if(!layer) continue; // todo : warn the user ??
            m_layerControl->add_layer(layer,true);

        } catch (std::exception &e) {
            GILVIEWER_LOG_EXCEPTION(e.what())
                wxMessageBox(_("Exception: see log!"), _("Exception!"), wxICON_ERROR);
        }
    }
}
