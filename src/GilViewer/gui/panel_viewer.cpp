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
#	define NOMINMAX
#endif

#include <algorithm>
#include <numeric>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>

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
#include <wx/log.h>
#include <wx/statusbr.h>
#include <wx/filedlg.h>

#include "../layers/vector_layer_ghost.hpp"
#include "../layers/vector_layer.hpp"
#include "../gui/application_settings.hpp"
#include "../gui/layer_control_utils.hpp"
#include "../gui/layer_control.hpp"
#include "../gui/define_id.hpp"
#include "../gui/panel_manager.hpp"
#include "../tools/orientation_2d.hpp"

#ifdef _WINDOWS
#	include <wx/msw/winundef.h>
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

void panel_viewer::mode_navigation() {
    m_mode = MODE_NAVIGATION;
    //m_toolBar->ToggleTool(ID_MODE_NAVIGATION, true);
    //	m_menuMain->Check(ID_MODE_NAVIGATION, true);
}

void panel_viewer::mode_capture() {
    m_mode = MODE_CAPTURE;
    //m_toolBar->ToggleTool(ID_MODE_CAPTURE, true);
    //	m_menuMain->Check(ID_MODE_CAPTURE, true);
}

void panel_viewer::mode_geometry_moving() {
    m_mode = MODE_GEOMETRY_MOVING;
    //m_toolBar->ToggleTool(ID_MODE_GEOMETRY_MOVING, true);
    //	m_menuMain->Check(ID_MODE_GEOMETRY_MOVING, true);
}

void panel_viewer::mode_edition() {
    m_mode = MODE_EDITION;
    //m_toolBar->ToggleTool(ID_MODE_EDITION, true);
    //	m_menuMain->Check(ID_MODE_EDITION, true);
}

void panel_viewer::mode_selection() {
    m_mode = MODE_SELECTION;
    //m_toolBar->ToggleTool(ID_MODE_SELECTION, true);
    //	m_menuMain->Check(ID_MODE_SELECTION, true);
}

void panel_viewer::geometry_null() {
    m_ghostLayer->m_drawPointPosition = false;
    m_ghostLayer->m_drawCircle = false;
    m_ghostLayer->m_drawRectangleSelection = false;

    m_geometry = GEOMETRY_NULL;
    //m_toolBar->ToggleTool(ID_GEOMETRY_NULL, true);
    //	m_menuMain->Check(ID_GEOMETRY_NULL, true);
    Refresh();
}

void panel_viewer::geometry_point() {
    m_geometry = GEOMETRY_POINT;
    //m_toolBar->ToggleTool(ID_GEOMETRY_POINT, true);
    //	m_menuMain->Check(ID_GEOMETRY_POINT, true);
}

void panel_viewer::geometry_circle() {
    m_geometry = GEOMETRY_CIRCLE;
    //m_toolBar->ToggleTool(ID_GEOMETRY_CIRCLE, true);
    //	m_menuMain->Check(ID_GEOMETRY_CIRCLE, true);
}

void panel_viewer::geometry_rectangle() {
    m_geometry = GEOMETRY_RECTANGLE;
    //m_toolBar->ToggleTool(ID_GEOMETRY_RECTANGLE, true);
    //	m_menuMain->Check(ID_GEOMETRY_RECTANGLE, true);
}

void panel_viewer::geometry_line() {
    m_geometry = GEOMETRY_LINE;
    //m_toolBar->ToggleTool(ID_GEOMETRY_LINE, true);
    //	m_menuMain->Check(ID_GEOMETRY_LINE, true);
}

void panel_viewer::geometry_polygon() {
    m_geometry = GEOMETRY_POLYGONE;
    //m_toolBar->ToggleTool(ID_GEOMETRY_POLYGONE, true);
    //	m_menuMain->Check(ID_GEOMETRY_POLYGONE, true);
}

layer_control* panel_viewer::layercontrol() const {
    return m_layerControl;
}

application_settings* panel_viewer::applicationsettings() const {
    return m_applicationSettings;
}

panel_viewer::panel_viewer(wxFrame* parent) :
	wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS), m_parent(parent), m_mainToolbar(NULL), m_modeAndGeometryToolbar(NULL), m_menuBar(NULL),
	//m_menuMain(NULL),
        m_mouseMovementStarted(false), m_translationDrag(0, 0),
        // Construction des differentes fenetres liees au PanelViewer :
        //		- layer control
        m_layerControl(new layer_control(this, parent, wxID_ANY, _("Layers control"))),
        //		- applications settings
        //reference au ghostLayer du LayerControl
        m_ghostLayer(layercontrol()->m_ghostLayer),
        //Setting des modes d'interface :
        m_mode(MODE_NAVIGATION), m_geometry(GEOMETRY_POINT) {
    if (panel_manager::instance()->panels_list().size() == 0)
        m_applicationSettings = new application_settings(this, wxID_ANY);

#if wxUSE_DRAG_AND_DROP
    SetDropTarget(new gilviewer_file_drop_target(this));
#endif // wxUSE_DRAG_AND_DROP
    // Avoids flickering effect under windows !!!
    //	SetFocus();
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

    //	/////Popup menu
    //	m_menuMain = new wxMenu;
    //	//	wxMenu* menuRemove = new wxMenu;
    //	//	wxMenu* menuVisibility = new wxMenu;
    //	//	wxMenu* menuTransformability = new wxMenu;
    //	//
    //	//	for (unsigned int i = 0; i < m_layerControl->GetNumberOfLayers(); ++i)
    //	//	{
    //	//		unsigned int index_row = m_layerControl->GetRows()[i]->m_index;
    //	//		menuRemove->Append(ID_POPMENU_REMOVE + index_row, wxString(m_layerControl->Layers()[i]->Name().c_str(), *wxConvCurrent));
    //	//		menuVisibility->AppendCheckItem(ID_POPMENU_VISIBILITY + index_row, wxString(m_layerControl->Layers()[i]->Name().c_str(), *wxConvCurrent));
    //	//		menuTransformability->AppendCheckItem(ID_POPMENU_TRANSFORMABILITY + index_row, wxString(m_layerControl->Layers()[i]->Name().c_str(), *wxConvCurrent));
    //	//		menuVisibility->Check(ID_POPMENU_VISIBILITY + index_row, m_layerControl->Layers()[i]->IsVisible());
    //	//		menuTransformability->Check(ID_POPMENU_TRANSFORMABILITY + index_row, m_layerControl->Layers()[i]->IsTransformable());
    //	//	}
    //
    //	m_menuMain->Append(wxID_NEW, _("New"));
    //	m_menuMain->Append(wxID_OPEN, _("Open"));
    //	m_menuMain->AppendSeparator();
    //	//	m_menuMain->AppendSubMenu(menuRemove, _("Remove layer ...") );
    //	//	m_menuMain->AppendSubMenu(menuVisibility, _("Change layer visibility ...") );
    //	//	m_menuMain->AppendSubMenu(menuTransformability, _("Change layer transformability ...") );
    //	//	m_menuMain->AppendSeparator();
    //	m_menuMain->Append(ID_BASIC_SNAPSHOT, _("Screenshot\tCtrl-C"));
    //
    //	//	wxMenu* menuMode = new wxMenu;
    //	m_menuMain->AppendSeparator();
    //	//	m_menuMain->AppendSubMenu(menuMode, _("Mode"));
    //	m_menuMain->AppendRadioItem(ID_MODE_NAVIGATION, _("Navigation"));
    //	m_menuMain->AppendRadioItem(ID_MODE_CAPTURE, _("Saisie"));
    //	m_menuMain->AppendRadioItem(ID_MODE_GEOMETRY_MOVING, _("Move"));
    //	m_menuMain->AppendRadioItem(ID_MODE_EDITION, _("Edition"));
    //	m_menuMain->AppendRadioItem(ID_MODE_SELECTION, _("Selection"));
    //	m_menuMain->AppendRadioItem(ID_SINGLE_CROP, _("Crop"));
    //
    //	m_menuMain->AppendSeparator();
    //	m_menuMain->Append(wxID_ABOUT, _("About"));
    //
    //	//	wxMenu* menuGeometry = new wxMenu;
    //	//	m_menuMain->AppendSubMenu(menuGeometry, _("Geometry"));
    //	m_menuMain->AppendSeparator();
    //	m_menuMain->AppendRadioItem(ID_GEOMETRY_NULL, _("Nothing"));
    //	m_menuMain->AppendRadioItem(ID_GEOMETRY_POINT, _("Point"));
    //	m_menuMain->AppendRadioItem(ID_GEOMETRY_LINE, _("Line"));
    //	m_menuMain->AppendRadioItem(ID_GEOMETRY_CIRCLE, _("Circle"));
    //	m_menuMain->AppendRadioItem(ID_GEOMETRY_RECTANGLE, _("Rectangle"));
    //	m_menuMain->AppendRadioItem(ID_GEOMETRY_POLYGONE, _("Polygon"));

    ///Shortcuts
    wxAcceleratorEntry entries[3];
    entries[0].Set(wxACCEL_CTRL, (int) 'C', ID_BASIC_SNAPSHOT);
    entries[1].Set(wxACCEL_ALT, (int) 'N', ID_MODE_NAVIGATION);
    entries[2].Set(wxACCEL_ALT, (int) 'C', ID_MODE_CAPTURE);
    wxAcceleratorTable acceleratorTable(3, entries);
    this->SetAcceleratorTable(acceleratorTable);

}

wxToolBar* panel_viewer::main_toolbar(wxWindow* parent) {

    if (!m_mainToolbar) {
        m_mainToolbar = new wxToolBar(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTB_HORIZONTAL);

        m_mainToolbar->AddTool(wxID_OPEN, wxT("O"), wxXmlResource::Get()->LoadBitmap(wxT("DOCUMENT-OPEN_22x22")), wxNullBitmap, wxITEM_NORMAL, _("Open layer"));
        m_mainToolbar->AddTool(ID_SHOW_HIDE_LAYER_CONTROL, wxT("SHLC"), wxArtProvider::GetIcon(wxART_LIST_VIEW, wxART_TOOLBAR, wxSize(22, 22)), wxNullBitmap, wxITEM_NORMAL, _("Show / Hide layer control"));

        m_mainToolbar->AddTool(ID_BASIC_SNAPSHOT, wxT("S"), wxXmlResource::Get()->LoadBitmap(wxT("CAMERA_PHOTO_22x22")), wxNullBitmap, wxITEM_NORMAL, _("Snapshot"));

        m_mainToolbar->AddTool(wxID_PREFERENCES, wxT("AS"), wxXmlResource::Get()->LoadBitmap(wxT("APPLICATIONS-SYSTEM_22x22")), wxNullBitmap, wxITEM_NORMAL, _("Application settings"));

        m_mainToolbar->AddTool(wxID_HELP, wxT("AS"), wxXmlResource::Get()->LoadBitmap(wxT("HELP_22x22")), wxNullBitmap, wxITEM_NORMAL, _("Help"));

        //	m_toolBar->AddSeparator();
        //	m_toolBar->AddTool(ID_MODE_NAVIGATION, wxT("MN"), wxBitmap(icone_move16_16_xpm), wxNullBitmap, wxITEM_RADIO, _("Navigation"));
        //	m_toolBar->AddTool(ID_MODE_CAPTURE, wxT("MN"), wxBitmap(mActionToggleEditing_xpm), wxNullBitmap, wxITEM_RADIO, _("Saisie"));
        //	m_toolBar->AddTool(ID_MODE_EDITION, wxT("MN"), wxBitmap(mActionToggleEditing_xpm), wxNullBitmap, wxITEM_RADIO, _("Edition"));
        //	m_toolBar->AddTool(ID_MODE_GEOMETRY_MOVING, wxT("MN"), wxBitmap(geometry_moving_16x16_xpm), wxNullBitmap, wxITEM_RADIO, _("Geometry moving"));
        //	m_toolBar->AddTool(ID_MODE_SELECTION, wxT("MN"), wxBitmap(select_16x16_xpm), wxNullBitmap, wxITEM_RADIO, _("Selection"));

        //	m_toolBar->AddSeparator();
        //	m_toolBar->AddTool(ID_GEOMETRY_NULL, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("PROCESS-STOP_16x16")), wxNullBitmap, wxITEM_RADIO, _("None"));
        //	m_toolBar->AddTool(ID_GEOMETRY_POINT, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("POINTS_16x16")), wxNullBitmap, wxITEM_RADIO, _("Point"));
        //	m_toolBar->AddTool(ID_GEOMETRY_CIRCLE, wxT("MN"), wxBitmap(mActionToggleEditing_xpm), wxNullBitmap, wxITEM_RADIO, _("Circle"));
        //	m_toolBar->AddTool(ID_GEOMETRY_LINE, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("POLYLINES_16x16")), wxNullBitmap, wxITEM_RADIO, _("Line"));
        //	m_toolBar->AddTool(ID_GEOMETRY_RECTANGLE, wxT("MN"), wxBitmap(capture_rectangle_16x16_xpm), wxNullBitmap, wxITEM_RADIO, _("Rectangle"));
        //	m_toolBar->AddTool(ID_GEOMETRY_POLYGONE, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("POLYGONS_16x16")), wxNullBitmap, wxITEM_RADIO, _("Polygone"));

        //	m_toolBar->AddSeparator();
        //	m_toolBar->AddTool(ID_SINGLE_CROP, wxT("MN"), wxBitmap(geometry_moving_16x16_xpm), wxNullBitmap, wxITEM_NORMAL, _("Single crop"));
        //	m_toolBar->AddTool(ID_MULTI_CROP, wxT("MN"), wxBitmap(select_16x16_xpm), wxNullBitmap, wxITEM_NORMAL, _("Multi crop"));

        m_mainToolbar->Realize();
    }

    return m_mainToolbar;
}

wxToolBar* panel_viewer::mode_and_geometry_toolbar(wxWindow* parent) {
    if (!m_modeAndGeometryToolbar) {
        m_modeAndGeometryToolbar = new wxToolBar(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTB_HORIZONTAL);

        m_modeAndGeometryToolbar->AddTool(ID_MODE_NAVIGATION, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("HAND_22x22")), wxNullBitmap, wxITEM_RADIO, _("Navigation"));
        m_modeAndGeometryToolbar->AddTool(ID_MODE_CAPTURE, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("PEN_22x22")), wxNullBitmap, wxITEM_RADIO, _("Saisie"));
        //m_modeAndGeometryToolbar->AddTool(ID_MODE_EDITION, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("APPLICATIONS_OFFICE_22x22")), wxNullBitmap, wxITEM_RADIO, _("Edition"));
        m_modeAndGeometryToolbar->AddTool(ID_MODE_GEOMETRY_MOVING, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("GEOMETRY_MOVING_22x22")), wxNullBitmap, wxITEM_RADIO, _("Geometry moving"));
        //m_modeAndGeometryToolbar->AddTool(ID_MODE_SELECTION, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("SELECTION_22x22")), wxNullBitmap, wxITEM_RADIO, _("Selection"));

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
    // on commence un cliquer-glisser pour bouger
    m_mouseMovementStarted = true;
    SetCursor(wxCursor(wxCURSOR_HAND));

    m_mouseMovementInitX = event.m_x;
    m_mouseMovementInitY = event.m_y;

    ///saisie d'un point
    if (m_mode == MODE_NAVIGATION || event.m_shiftDown) {
        //rien
    } else if (m_mode == MODE_CAPTURE) {
        geometry_add_point(wxPoint(event.m_x, event.m_y));
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

    if (m_mode == MODE_NAVIGATION || event.m_shiftDown) {
        //rien
    }
    ///si capture (et qu'on ne recommence pas à naviguer donc que shift est n'est pas enfoncé) : fin de la géométrie
    else if (m_mode == MODE_CAPTURE) {
        geometry_end();
    }

}

void panel_viewer::on_right_down(wxMouseEvent &event) {
    //	/////Clique droit popup menu
    //	if (event.m_shiftDown) //pour ne pas interférer avec le double-click !
    //	{
    //		wxPoint point = event.GetPosition();
    //		// If from keyboard
    //		if (point.x == -1 && point.y == -1) {
    //			wxSize size = GetSize();
    //			point.x = size.x / 2;
    //			point.y = size.y / 2;
    //		}
    //		ShowPopUpMenu(point);
    //	} else

    if (m_mode == MODE_CAPTURE) {
        switch (m_geometry) {
        case GEOMETRY_NULL:
        case GEOMETRY_POINT:
        case GEOMETRY_CIRCLE:
        case GEOMETRY_RECTANGLE:
            break;
        case GEOMETRY_LINE:
            m_ghostLayer->m_lineEndCapture = true;
            geometry_add_point(wxPoint(event.m_x, event.m_y));
            break;
        case GEOMETRY_POLYGONE:
            break;
        }
    }

}

void panel_viewer::on_left_double_click(wxMouseEvent &event) {
    ///si on est en mode navigation (ou qu'on appuie sur shift -> force le mode navigation)
    if (m_mode == MODE_NAVIGATION || event.m_shiftDown) {
        wxConfigBase *pConfig = wxConfigBase::Get();
        double zoom_;
        pConfig->Read(wxT("/Options/Zoom"), &zoom_, 0.5);
        zoom(zoom_, event);
    }
    update_statusbar(event.m_x, event.m_y);
}

void panel_viewer::on_right_double_click(wxMouseEvent &event) {
    ///si on est en mode navigation (ou qu'on appuie sur shift -> force le mode navigation)
    if (m_mode == MODE_NAVIGATION || event.m_shiftDown) {
        wxConfigBase *pConfig = wxConfigBase::Get();
        double dezoom;
        pConfig->Read(wxT("/Options/Dezoom"), &dezoom, 2.);
        zoom(dezoom, event);
    }
    update_statusbar(event.m_x, event.m_y);
}

void panel_viewer::on_middle_down(wxMouseEvent & event) {
    ///Affichage du layer control
    wxPoint pos = event.GetPosition();
    pos += GetScreenPosition();
    m_layerControl->SetPosition(pos);

    show_layer_control(!m_layerControl->IsVisible());

    event.Skip();
    update_statusbar(event.m_x, event.m_y);
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
    update_statusbar(event.m_x, event.m_y);
}

void panel_viewer::on_mouse_move(wxMouseEvent &event) {
    //déplacement (de l'image ou de la géométrie) si on a bouton gauche enfoncé (m_mouseMovementStarted)
    if (m_mouseMovementStarted) {
        wxPoint translation((int) (event.m_x - m_mouseMovementInitX), (int) (event.m_y - m_mouseMovementInitY));

        m_mouseMovementInitX = event.m_x;
        m_mouseMovementInitY = event.m_y;

        ///si on est en mode navigation (ou qu'on appuie sur shift -> force le mode navigation) : déplacement de l'image
        if (m_mode == MODE_NAVIGATION || event.m_shiftDown) {
            m_translationDrag += translation;
            scene_move(translation);
        }
        ///si en mode geometry moving : c'est la géométrie qui bouge !!
        else if (m_mode == MODE_GEOMETRY_MOVING) {
            geometry_move_absolute(wxPoint(event.m_x, event.m_y));
        }
    }

    if (m_mode == MODE_CAPTURE) ///si en mode capture : c'est le point à ajouter qui bouge !!
    {
        geometry_update_absolute(wxPoint(event.m_x, event.m_y));
    }

    update_statusbar(event.m_x, event.m_y);
    //	SetFocus();
}

void panel_viewer::on_keydown(wxKeyEvent& event) {
    int step;
    if (event.m_controlDown)
        step = 50;
    else
        step = 1;

    wxPoint deplacement(0, 0);
    //déplacement effectif
    if (event.m_keyCode == WXK_RIGHT)
        deplacement = wxPoint(step, 0);
    else if (event.m_keyCode == WXK_LEFT)
        deplacement = wxPoint(-step, 0);
    else if (event.m_keyCode == WXK_DOWN)
        deplacement = wxPoint(0, step);
    else if (event.m_keyCode == WXK_UP)
        deplacement = wxPoint(0, -step);

    if (event.m_keyCode == WXK_RIGHT || event.m_keyCode == WXK_LEFT || event.m_keyCode == WXK_UP || event.m_keyCode == WXK_DOWN) {
        ///si on est en mode navigation (ou qu'on appuie sur shift -> force le mode navigation) : déplacement de l'image
        if (m_mode == MODE_NAVIGATION || event.m_shiftDown) {
            update_if_transformable();
            scene_move(deplacement);
        }
        ///si en mode capture : c'est le point à ajouter qui bouge !!
        else if (m_mode == MODE_CAPTURE) {
            geometry_update_relative(deplacement);
        }
        ///si en mode geometry moving : c'est la géométrie qui bouge !!
        else if (m_mode == MODE_GEOMETRY_MOVING) {
            geometry_move_relative(deplacement);
        }
    }

    event.Skip();
    update_statusbar(event.m_x, event.m_y);

}

void panel_viewer::update_if_transformable() {
    for (layer_control::iterator it = m_layerControl->begin(); it != m_layerControl->end(); ++it) {
        if ((*it)->transformable()) {
            (*it)->needs_update(true);
        }
    }
}

void panel_viewer::scene_move(const wxPoint& translation) {
    for (layer_control::iterator it = m_layerControl->begin(); it != m_layerControl->end(); ++it) {
        if ((*it)->transformable()) {
            (*it)->translation_x((*it)->translation_x() + translation.x * (*it)->zoom_factor());
            (*it)->translation_y((*it)->translation_y() + translation.y * (*it)->zoom_factor());
        }
    }
    m_ghostLayer->translation_x(m_ghostLayer->translation_x() + translation.x * m_ghostLayer->zoom_factor());
    m_ghostLayer->translation_y(m_ghostLayer->translation_y() + translation.y * m_ghostLayer->zoom_factor());
    Refresh();
}

bool panel_viewer::coord_image(const int mouseX, const int mouseY, int &i, int &j) const {
    bool coordOK = false;
    for (layer_control::iterator it = m_layerControl->begin(); it != m_layerControl->end() && !coordOK; ++it) {
        if ((*it)->visible() && ((*it)->pixel_value(i, j).length() != 0)) {
            coordOK = true;
            double zoom = (*it)->zoom_factor();
            i = static_cast<int> (zoom*mouseX-(*it)->translation_x());
            j = static_cast<int> (zoom*mouseY-(*it)->translation_y());
        }
    }
    return coordOK;
}

bool panel_viewer::subpix_coord_image(const int mouseX, const int mouseY, double &i, double &j) const {
    bool coordOK = false;
    for (layer_control::iterator it = m_layerControl->begin(); it != m_layerControl->end() && !coordOK; ++it) {
        if ((*it)->visible()) {
            coordOK = true;
            double zoom = (*it)->zoom_factor();
            i = static_cast<double> (zoom*mouseX-(*it)->translation_x()- 0.5);
            j = static_cast<double> (zoom*mouseY-(*it)->translation_y()- 0.5);
        }
    }
    return coordOK;
}

void panel_viewer::open_custom_format(const std::string &filename) {
    std::ostringstream oss;
    oss << "File : " << __FILE__ << "\n";
    oss << "Function : " << __FUNCTION__ << "\n";
    oss << "Line : " << __LINE__ << "\n";
    oss << "Format non supporté !\n";
    ::wxMessageBox(wxString(oss.str().c_str(), *wxConvCurrent));
}

void panel_viewer::update_statusbar(const int i, const int j) {
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

        m_parent->GetStatusBar()->SetFieldsCount(nb + 1); //+1 pour les coord en pixels

        unsigned int count = 0;
        bool affichagePixelDone = false;
        bool affichageCartoDone = false;

        for (layer_control::iterator it = m_layerControl->begin(); it != m_layerControl->end(); ++it) {
            std::string affichage;

            if ((*it)->visible()) // && ((*it)->GetPixelValue(i, j).length() != 0))
            {
                // TODO : Nettoyer !!!
                if (!affichagePixelDone) {
                    affichagePixelDone = true;
                    std::ostringstream coordPixel;
                    coordPixel << "Coord image : (";
                    coordPixel << static_cast<int> (-(*it)->translation_x() + i * (*it)->zoom_factor());
                    coordPixel << ",";
                    coordPixel << static_cast<int> (-(*it)->translation_y() + j * (*it)->zoom_factor());
                    coordPixel << ")";
                    double dx, dy;
                    subpix_coord_image(i, j, dx, dy);
                    coordPixel << (" - ( ");
                    coordPixel << static_cast<double> (-(*it)->translation_x() + i * (*it)->zoom_factor()) - 0.5;
                    coordPixel << ",";
                    coordPixel << static_cast<double> (-(*it)->translation_y() + j * (*it)->zoom_factor()) - 0.5;
                    coordPixel << ")";
                    m_parent->SetStatusText(wxString(coordPixel.str().c_str(), *wxConvCurrent), count);
                    ++count;
                }

                if (!affichageCartoDone && m_layerControl->oriented()) {
                    affichageCartoDone = true;
                    std::ostringstream coordCarto;
                    coordCarto << "Coord carto : (";
                    coordCarto << static_cast<int> (ori->origin_x() + ori->step() * (-(*it)->translation_x() + i * (*it)->zoom_factor()));
                    coordCarto << ",";
                    coordCarto << static_cast<int> (ori->origin_y() + ori->step() * ((*it)->translation_y() - j * (*it)->zoom_factor()));
                    coordCarto << ")";
                    m_parent->SetStatusText(wxString(coordCarto.str().c_str(), *wxConvCurrent), count);
                    ++count;
                }

                affichage += boost::filesystem::basename((*it)->name()) + " : ";
                affichage += (*it)->pixel_value(i, j);
                std::ostringstream oss;
                oss << 100. / (*it)->zoom_factor();
                affichage += "-" + oss.str() + "%";
                m_parent->SetStatusText(wxString(affichage.c_str(), *wxConvCurrent), count);
                ++count;
            }
        }
    }
}

void panel_viewer::zoom(double zoom_factor, wxMouseEvent &event) {
    for (layer_control::iterator it = m_layerControl->begin(); it != m_layerControl->end(); ++it) {
        if ((*it)->transformable()) {
            (*it)->translation_x((*it)->translation_x() + event.m_x * (*it)->zoom_factor() * (zoom_factor - 1));
            (*it)->translation_y((*it)->translation_y() + event.m_y * (*it)->zoom_factor() * (zoom_factor - 1));
            (*it)->zoom_factor((*it)->zoom_factor() * zoom_factor);
            (*it)->needs_update(true);
        }
    }
    m_ghostLayer->translation_x(m_ghostLayer->translation_x() + event.m_x * m_ghostLayer->zoom_factor() * (zoom_factor - 1));
    m_ghostLayer->translation_y(m_ghostLayer->translation_y() + event.m_y * m_ghostLayer->zoom_factor() * (zoom_factor - 1));
    m_ghostLayer->zoom_factor(m_ghostLayer->zoom_factor() * zoom_factor);
    Refresh();
}

/*
 void PanelViewer::OnChar(wxKeyEvent& event)
 {
 if (event.GetKeyCode()==(int)'l')
 m_layerControl->Show( !m_layerControl->IsVisible() );
 }
 */

void panel_viewer::add_layer(const layer::ptrLayerType &layer) {
    try {
        m_layerControl->add_layer(layer);
    } catch (const std::exception &e) {
        GILVIEWER_LOG_EXCEPTION("")
    }
}

void panel_viewer::show_layer_control(bool show) const {
    m_layerControl->Show(show);
}

//void PanelViewer::ShowPopUpMenu(const wxPoint& pos) {
//
//	PopupMenu(m_menuMain, pos.x, pos.y);
//}

void panel_viewer::on_quit(wxCommandEvent& event) {
    Close();
}

void panel_viewer::snap_shot(wxCommandEvent& event) {
    wxString wildcard;
    wildcard << _("All supported files ");
    wildcard << wxT("(*.tif;*.tiff;*.png*.jpg;*.jpeg;*.bmp)|*.tif;*.tiff;*.png*.jpg;*.jpeg;*.bmp|");
    wildcard << wxT("TIFF (*.tif;*.tiff)|*.tif;*.tiff|");
    wildcard << wxT("PNG (*.png)|*.png|");
    wildcard << wxT("JPEG (*.jpg;*.jpeg)|*.jpg;*.jpeg|");
    wildcard << wxT("BMP (*.bmp)|*.bmp|");

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
        wxImage im = snap.ConvertToImage();

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

void panel_viewer::execute_mode_navigation() {

}
void panel_viewer::execute_mode_geometry_moving() {

}
void panel_viewer::execute_mode_capture() {

}
void panel_viewer::execute_mode_edition() {

}
void panel_viewer::execute_mode_selection() {

}

inline double DistancePoints(const wxPoint& pt1, const wxPoint &pt2) {
    wxPoint diff(pt2 - pt1);
    return std::sqrt((double) (diff.x * diff.x + diff.y * diff.y));
}

void panel_viewer::geometry_add_point(const wxPoint & p)
        //coord filées par l'event (pas encore image)
{
    //	double xi,yi;
    //	GetSubPixCoordImage( x, y, xi, yi );
    wxPoint pt = m_ghostLayer->to_local(p);

    m_ghostLayer->m_drawPointPosition = false;
    m_ghostLayer->m_drawCircle = false;
    m_ghostLayer->m_drawLine = false;
    m_ghostLayer->m_drawRectangleSelection = false;

    switch (m_geometry) {
    case GEOMETRY_NULL:

        break;
    case GEOMETRY_CIRCLE:
        m_ghostLayer->m_drawCircle = true;
        m_ghostLayer->m_penCircle = wxPen(*wxBLUE, 2);
        m_ghostLayer->m_brushCircle = wxBrush(*wxBLUE, wxTRANSPARENT);
        if (!m_ghostLayer->m_CircleFirstPointSet) {
            m_ghostLayer->m_circle.first = pt;
            m_ghostLayer->m_circle.second = 1;
            m_ghostLayer->m_CircleFirstPointSet = true;
        } else {
            m_ghostLayer->m_circle.second = DistancePoints(m_ghostLayer->m_circle.first, pt);
            m_ghostLayer->m_CircleFirstPointSet = false;
            geometry_end();
        }
        break;
	case GEOMETRY_LINE:
        m_ghostLayer->m_drawLine = true;
        m_ghostLayer->m_penLine = wxPen(*wxBLUE, 1);

        if (!m_ghostLayer->m_lineHasBegun) {
            m_ghostLayer->m_linePoints.clear();
            m_ghostLayer->m_linePoints.push_back(pt);
        }

        m_ghostLayer->m_lineHasBegun = true;
        m_ghostLayer->m_linePoints.push_back(pt);
        if (m_ghostLayer->m_lineEndCapture) {
            m_ghostLayer->m_lineEndCapture = false;
            m_ghostLayer->m_lineHasBegun = false;
            geometry_end();
        }
        break;
	case GEOMETRY_POINT:
        m_ghostLayer->m_penPoint = wxPen(*wxGREEN, 3);
        m_ghostLayer->m_pointPosition = pt;
        m_ghostLayer->m_drawPointPosition = true;
        geometry_end();
        break;
	case GEOMETRY_POLYGONE:

        break;
	case GEOMETRY_RECTANGLE:
        m_ghostLayer->m_penRectangle = wxPen(*wxRED, 2, wxDOT);
        m_ghostLayer->m_drawRectangleSelection = true;
        m_ghostLayer->m_rectangleSelection.second = pt;
        m_ghostLayer->m_brushRectangle = wxBrush(*wxRED, wxTRANSPARENT);
        if (!m_ghostLayer->m_rectangleSelectionFirstPointSet) {
            m_ghostLayer->m_rectangleSelection.first  = pt;
            m_ghostLayer->m_rectangleSelectionFirstPointSet = true;
        } else {
            m_ghostLayer->m_rectangleSelectionFirstPointSet = false;
            geometry_end();
        }
        break;
    }

    execute_mode();
}

void panel_viewer::geometry_move_relative(const wxPoint& translation) {
    switch (m_geometry) {
    case GEOMETRY_NULL:

        break;
    case GEOMETRY_CIRCLE:
        m_ghostLayer->m_circle.first += translation;
        break;
    case GEOMETRY_LINE:
        std::for_each(m_ghostLayer->m_linePoints.begin(), m_ghostLayer->m_linePoints.end(), boost::lambda::_1 += translation);
        break;
    case GEOMETRY_POINT:
        m_ghostLayer->m_pointPosition += translation;
        break;
    case GEOMETRY_POLYGONE:

        break;
    case GEOMETRY_RECTANGLE:
        m_ghostLayer->m_rectangleSelection.first += translation;
        m_ghostLayer->m_rectangleSelection.second += translation;

        break;
    }

    execute_mode();
}

void panel_viewer::geometry_move_absolute(const wxPoint& p) {
    wxPoint pt = m_ghostLayer->to_local(p);

    switch (m_geometry) {
    case GEOMETRY_NULL:

        break;
    case GEOMETRY_CIRCLE:
        m_ghostLayer->m_circle.first = pt;
        break;
    case GEOMETRY_LINE: {
            wxPoint bary(std::accumulate(m_ghostLayer->m_linePoints.begin(), m_ghostLayer->m_linePoints.end(), wxPoint(0, 0)));
            bary.x = (int) (bary.x / float(m_ghostLayer->m_linePoints.size()));
            bary.y = (int) (bary.y / float(m_ghostLayer->m_linePoints.size()));
            std::for_each(m_ghostLayer->m_linePoints.begin(), m_ghostLayer->m_linePoints.end(), boost::lambda::_1 += -bary + pt);
	}
        break;
    case GEOMETRY_POINT:
        m_ghostLayer->m_pointPosition = pt;
        break;
    case GEOMETRY_POLYGONE:

        break;
    case GEOMETRY_RECTANGLE:
        wxPoint rectangleDiagonale = m_ghostLayer->m_rectangleSelection.second - m_ghostLayer->m_rectangleSelection.first;
        m_ghostLayer->m_rectangleSelection.first = wxPoint((int) (pt.x - float(rectangleDiagonale.x) / 2), (int) (pt.y - float(rectangleDiagonale.y) / 2));
        m_ghostLayer->m_rectangleSelection.second = m_ghostLayer->m_rectangleSelection.first + rectangleDiagonale;

        break;
    }

    execute_mode();
}

void panel_viewer::geometry_update_absolute(const wxPoint & p)
        //coord filées par l'event (pas encore image)
{
    wxPoint pt = m_ghostLayer->to_local(p);

    switch (m_geometry) {
    case GEOMETRY_NULL:

        break;
    case GEOMETRY_CIRCLE:
        if (m_ghostLayer->m_CircleFirstPointSet) {
            m_ghostLayer->m_circle.second = DistancePoints(m_ghostLayer->m_circle.first, pt);
        }
        break;
	case GEOMETRY_LINE:
        if (m_ghostLayer->m_lineHasBegun) {
            if (m_ghostLayer->m_linePoints.size() > 1)
                m_ghostLayer->m_linePoints.back() = pt;
        }
        break;
	case GEOMETRY_POINT:
        //rien à faire pour un point
        break;
	case GEOMETRY_POLYGONE:

        break;
	case GEOMETRY_RECTANGLE:
        //si le rectangle a déjà un premier point, on met à jour le deuxième
        if (m_ghostLayer->m_rectangleSelectionFirstPointSet) {
            m_ghostLayer->m_rectangleSelection.second = pt;
        }
        break;
    }

    execute_mode();

}

void panel_viewer::geometry_update_relative(const wxPoint & translation)
        //coord filées par l'event (pas encore image)
{
    switch (m_geometry) {
    case GEOMETRY_NULL:

        break;
    case GEOMETRY_CIRCLE:

        break;
    case GEOMETRY_LINE:

        break;
    case GEOMETRY_POINT:
        //rien à faire pour un point
        break;
    case GEOMETRY_POLYGONE:

        break;
    case GEOMETRY_RECTANGLE:
        //si le rectangle a déjà un premier point, on met à jour le deuxième
        if (m_ghostLayer->m_rectangleSelectionFirstPointSet) {
            m_ghostLayer->m_rectangleSelection.second += translation;
        }
        break;
    }

    execute_mode();

}

void panel_viewer::geometry_end() {
    execute_mode();
}

panel_viewer* create_panel_viewer(wxFrame* parent) {
    return new panel_viewer(parent);
}

void panel_viewer::Register(wxFrame* parent) {
    panel_manager::instance()->Register("PanelViewer", boost::bind(create_panel_viewer, parent));
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
    // On se met en mode capture et on active la selection par rectangle
    mode_capture();
    geometry_rectangle();
    // On recherche le calque selectionne
    std::vector<boost::shared_ptr<layer_control_row> >::iterator itr = m_layerControl->rows().begin();
    unsigned int i = 0;
    std::vector<layer::ptrLayerType> selected_layers;
    for (layer_control::iterator it = m_layerControl->begin(); it != m_layerControl->end() && i < static_cast<unsigned int>(m_layerControl->rows().size()); ++it, ++i) {
        if (m_layerControl->rows()[i]->m_nameStaticText->selected()) {
            selected_layers.push_back(*it);
        }
    }

    for(std::vector<layer::ptrLayerType>::const_iterator it=selected_layers.begin(); it!=selected_layers.end(); ++it) {
        wxRect  r  = m_ghostLayer->rectangle();
        wxPoint p0 = (*it)->to_local(r.GetTopLeft());
        wxPoint p1 = (*it)->to_local(r.GetBottomRight());
        try {
            layer::ptrLayerType layer = (*it)->crop(p0.x,p0.y,p1.x,p1.y);
            if(!layer) continue; // todo : warn the user ??
            m_layerControl->add_layer(layer);
            // now that the layer is added, we can set its geometry
            layer->translation_x(p0.x+(*it)->translation_x());
            layer->translation_y(p0.y+(*it)->translation_y());
            layer->zoom_factor  (     (*it)->zoom_factor  ());
            // todo : handle Orientation2D of *it if it exists ... ??

        } catch (std::exception &e) {
            GILVIEWER_LOG_EXCEPTION(e.what())
                wxMessageBox(_("Exception: see log!"), _("Exception!"), wxICON_ERROR);
        }
    }
}
