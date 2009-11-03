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

#include "GilViewer/layers/VectorLayerGhost.h"
#include "GilViewer/layers/VectorLayer.hpp"
#include "GilViewer/gui/ApplicationSettings.hpp"
#include "GilViewer/gui/LayerControlUtils.hpp"
#include "GilViewer/gui/LayerControl.hpp"
#include "GilViewer/gui/define_id.hpp"
#include "GilViewer/gui/PanelManager.h"
#include "PanelViewer.hpp"

//#include "resources/geometry_moving_16x16.xpm"
//#include "resources/icone_move16_16.xpm"

#ifdef _WINDOWS
#	include <wx/msw/winundef.h>
#endif

BEGIN_EVENT_TABLE(PanelViewer, wxPanel)
EVT_MOTION(PanelViewer::OnMouseMove)
EVT_LEFT_DOWN(PanelViewer::OnLeftDown)
EVT_LEFT_UP(PanelViewer::OnLeftUp)
EVT_RIGHT_DOWN(PanelViewer::OnRightDown)
EVT_LEFT_DCLICK(PanelViewer::OnLeftDClick)
EVT_RIGHT_DCLICK(PanelViewer::OnRightDClick)
EVT_MIDDLE_DOWN(PanelViewer::OnMiddleDown)
EVT_MOUSEWHEEL(PanelViewer::OnMouseWheel)
EVT_KEY_DOWN(PanelViewer::OnKeydown)
EVT_PAINT(PanelViewer::OnPaint)
EVT_SIZE(PanelViewer::OnSize)
ADD_GILVIEWER_EVENTS_TO_TABLE(PanelViewer)
END_EVENT_TABLE()

IMPLEMENTS_GILVIEWER_METHODS_FOR_EVENTS_TABLE(PanelViewer,this)

using namespace std;

void PanelViewer::OnSize(wxSizeEvent &e) {
	for (LayerControl::iterator it = m_layerControl->begin(); it != m_layerControl->end(); ++it)
		(*it)->HasToBeUpdated(true);
}

void PanelViewer::SetModeNavigation() {
	m_mode = MODE_NAVIGATION;
	//m_toolBar->ToggleTool(ID_MODE_NAVIGATION, true);
	//	m_menuMain->Check(ID_MODE_NAVIGATION, true);
}

void PanelViewer::SetModeCapture() {
	m_mode = MODE_CAPTURE;
	//m_toolBar->ToggleTool(ID_MODE_CAPTURE, true);
	//	m_menuMain->Check(ID_MODE_CAPTURE, true);
}

void PanelViewer::SetModeGeometryMoving() {
	m_mode = MODE_GEOMETRY_MOVING;
	//m_toolBar->ToggleTool(ID_MODE_GEOMETRY_MOVING, true);
	//	m_menuMain->Check(ID_MODE_GEOMETRY_MOVING, true);
}

void PanelViewer::SetModeEdition() {
	m_mode = MODE_EDITION;
	//m_toolBar->ToggleTool(ID_MODE_EDITION, true);
	//	m_menuMain->Check(ID_MODE_EDITION, true);
}

void PanelViewer::SetModeSelection() {
	m_mode = MODE_SELECTION;
	//m_toolBar->ToggleTool(ID_MODE_SELECTION, true);
	//	m_menuMain->Check(ID_MODE_SELECTION, true);
}

void PanelViewer::SetGeometryNull() {
	m_ghostLayer.m_drawPointPosition = false;
	m_ghostLayer.m_drawCircle = false;
	m_ghostLayer.m_drawRectangleSelection = false;

	m_geometry = GEOMETRY_NULL;
	//m_toolBar->ToggleTool(ID_GEOMETRY_NULL, true);
	//	m_menuMain->Check(ID_GEOMETRY_NULL, true);
	Refresh();
}

void PanelViewer::SetGeometryPoint() {
	m_geometry = GEOMETRY_POINT;
	//m_toolBar->ToggleTool(ID_GEOMETRY_POINT, true);
	//	m_menuMain->Check(ID_GEOMETRY_POINT, true);
}

void PanelViewer::SetGeometryCircle() {
	m_geometry = GEOMETRY_CIRCLE;
	//m_toolBar->ToggleTool(ID_GEOMETRY_CIRCLE, true);
	//	m_menuMain->Check(ID_GEOMETRY_CIRCLE, true);
}

void PanelViewer::SetGeometryRectangle() {
	m_geometry = GEOMETRY_RECTANGLE;
	//m_toolBar->ToggleTool(ID_GEOMETRY_RECTANGLE, true);
	//	m_menuMain->Check(ID_GEOMETRY_RECTANGLE, true);
}

void PanelViewer::SetGeometryLine() {
	m_geometry = GEOMETRY_LINE;
	//m_toolBar->ToggleTool(ID_GEOMETRY_LINE, true);
	//	m_menuMain->Check(ID_GEOMETRY_LINE, true);
}

void PanelViewer::SetGeometryPolygone() {
	m_geometry = GEOMETRY_POLYGONE;
	//m_toolBar->ToggleTool(ID_GEOMETRY_POLYGONE, true);
	//	m_menuMain->Check(ID_GEOMETRY_POLYGONE, true);
}

LayerControl* PanelViewer::GetLayerControl() const {
	return m_layerControl;
}

ApplicationSettings* PanelViewer::GetApplicationSettings() const {
	return m_applicationSettings;
}

PanelViewer::PanelViewer(wxFrame* parent) :
	wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS), m_parent(parent), m_mainToolbar(NULL), m_modeAndGeometryToolbar(NULL), m_menuBar(NULL),
	//m_menuMain(NULL),
			m_mouseMovementStarted(false), m_translationDrag(0, 0),
			// Construction des differentes fenetres liees au PanelViewer :
			//		- layer control
			m_layerControl(new LayerControl(this, parent, wxID_ANY, _("Layers control"))),
			//		- applications settings
			//reference au ghostLayer du LayerControl
			m_ghostLayer(*GetLayerControl()->m_ghostLayer),
			//Setting des modes d'interface :
			m_mode(MODE_NAVIGATION), m_geometry(GEOMETRY_POINT) {
	if (PanelManager::Instance()->GetPanelsList().size() == 0)
		m_applicationSettings = new ApplicationSettings(this, wxID_ANY);

#if wxUSE_DRAG_AND_DROP
	SetDropTarget(new GilViewerFileDropTarget(this));
#endif // wxUSE_DRAG_AND_DROP
	// Avoids flickering effect under windows !!!
	//	SetFocus();
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	this->InitToolbar();

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

wxToolBar* PanelViewer::GetMainToolBar(wxWindow* parent) {

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

wxToolBar* PanelViewer::GetModeAndGeometryToolBar(wxWindow* parent) {
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

wxMenuBar* PanelViewer::GetMenuBar() {
	return m_menuBar;
}

bool PanelViewer::InitToolbar() {

	return true;
}

void PanelViewer::OnPaint(wxPaintEvent& evt) {
	wxBufferedPaintDC dc(this);
	if (!dc.Ok())
		return;
	dc.Clear();

	wxSize tailleImage(this->GetSize());

	for (LayerControl::iterator it = m_layerControl->begin(); it != m_layerControl->end(); ++it) {
		if ((*it)->IsVisible()) {
			if ((*it)->HasToBeUpdated()) {
				try {
					(*it)->Update(tailleImage.GetX(), tailleImage.GetY());
				} catch (const std::exception &e) {
					std::ostringstream oss;
					oss << "File : " << __FILE__ << "\n";
					oss << "Function : " << __FUNCTION__ << "\n";
					oss << "Line : " << __LINE__ << "\n";
					oss << e.what();
					wxMessageBox(wxString(oss.str().c_str(), *wxConvCurrent), _("Error!"), wxICON_ERROR);
					return;
				}
				(*it)->HasToBeUpdated(false);
			}

			if ((*it)->IsTransformable()) {
				(*it)->Draw(dc, static_cast<int> (m_translationDrag.x), static_cast<int> (m_translationDrag.y), true);
			} else {
				(*it)->Draw(dc, 0, 0, true);
			}
		}
	}

	//si on a des layers, on ajoute le ghost
	if (m_layerControl->GetRows().size()) {
		//alias
		Layer::ptrLayerType firstLayer = *m_layerControl->begin();
		GetLayerControl()->m_ghostLayer->Draw(dc, static_cast<int> (m_translationDrag.x), static_cast<int> (m_translationDrag.y), false, firstLayer->ZoomFactor(), firstLayer->TranslationX(), firstLayer->TranslationY(), firstLayer->Resolution());
	}
}

void PanelViewer::OnLeftDown(wxMouseEvent &event) {
	// on commence un cliquer-glisser pour bouger
	m_mouseMovementStarted = true;
	SetCursor(wxCursor(wxCURSOR_HAND));

	m_mouseMovementInitX = event.m_x;
	m_mouseMovementInitY = event.m_y;

	///saisie d'un point
	if (m_mode == MODE_NAVIGATION || event.m_shiftDown) {
		//rien
	} else if (m_mode == MODE_CAPTURE) {
		GeometryAddPoint(wxPoint(event.m_x, event.m_y));
	}

}

void PanelViewer::OnLeftUp(wxMouseEvent &event) {
	///si on était en mode navigation
	if (m_mouseMovementStarted) {
		SetCursor(wxCursor(wxCURSOR_ARROW));
		m_mouseMovementStarted = false;

		m_translationDrag.x = 0;
		m_translationDrag.y = 0;

		UpdateIfTransformable();

		Refresh();
	}

	if (m_mode == MODE_NAVIGATION || event.m_shiftDown) {
		//rien
	}
	///si capture (et qu'on ne recommence pas à naviguer donc que shift est n'est pas enfoncé) : fin de la géométrie
	else if (m_mode == MODE_CAPTURE) {
		GeometryEnd();
	}

}

void PanelViewer::OnRightDown(wxMouseEvent &event) {
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
			m_ghostLayer.m_lineEndCapture = true;
			GeometryAddPoint(wxPoint(event.m_x, event.m_y));
			break;
		case GEOMETRY_POLYGONE:
			break;
		}
	}

}

void PanelViewer::OnLeftDClick(wxMouseEvent &event) {
	///si on est en mode navigation (ou qu'on appuie sur shift -> force le mode navigation)
	if (m_mode == MODE_NAVIGATION || event.m_shiftDown) {
		wxConfigBase *pConfig = wxConfigBase::Get();
		double zoom;
		pConfig->Read(wxT("/Options/Zoom"), &zoom, 0.5);
		Zoom(zoom, event);
	}
	UpdateStatusBar(event.m_x, event.m_y);
}

void PanelViewer::OnRightDClick(wxMouseEvent &event) {
	///si on est en mode navigation (ou qu'on appuie sur shift -> force le mode navigation)
	if (m_mode == MODE_NAVIGATION || event.m_shiftDown) {
		wxConfigBase *pConfig = wxConfigBase::Get();
		double dezoom;
		pConfig->Read(wxT("/Options/Dezoom"), &dezoom, 2.);
		Zoom(dezoom, event);
	}
	UpdateStatusBar(event.m_x, event.m_y);
}

void PanelViewer::OnMiddleDown(wxMouseEvent & event) {
	///Affichage du layer control
	wxPoint pos = event.GetPosition();
	pos += GetScreenPosition();
	m_layerControl->SetPosition(pos);

	ShowLayerControl(!m_layerControl->IsVisible());

	event.Skip();
	UpdateStatusBar(event.m_x, event.m_y);
}

void PanelViewer::OnMouseWheel(wxMouseEvent& event) {
	////Zoom
	wxConfigBase *pConfig = wxConfigBase::Get();
	double zoom, dezoom;
	pConfig->Read(wxT("/Options/Zoom"), &zoom, 0.5);
	pConfig->Read(wxT("/Options/Dezoom"), &dezoom, 2.);
	int deltaMouseWheel = event.GetWheelRotation();
	double zoomFactor;
	if (deltaMouseWheel < 0)
		zoomFactor = dezoom;
	else
		zoomFactor = zoom;
	Zoom(zoomFactor, event);
	UpdateStatusBar(event.m_x, event.m_y);
}

void PanelViewer::OnMouseMove(wxMouseEvent &event) {
	//déplacement (de l'image ou de la géométrie) si on a bouton gauche enfoncé (m_mouseMovementStarted)
	if (m_mouseMovementStarted) {
		wxPoint translation(event.m_x - m_mouseMovementInitX, event.m_y - m_mouseMovementInitY);

		m_mouseMovementInitX = event.m_x;
		m_mouseMovementInitY = event.m_y;

		///si on est en mode navigation (ou qu'on appuie sur shift -> force le mode navigation) : déplacement de l'image
		if (m_mode == MODE_NAVIGATION || event.m_shiftDown) {
			m_translationDrag += translation;
			SceneMove(translation);
		}
		///si en mode geometry moving : c'est la géométrie qui bouge !!
		else if (m_mode == MODE_GEOMETRY_MOVING) {
			GeometryMoveAbsolute(wxPoint(event.m_x, event.m_y));
		}
	}

	if (m_mode == MODE_CAPTURE) ///si en mode capture : c'est le point à ajouter qui bouge !!
	{
		GeometryUpdateAbsolute(wxPoint(event.m_x, event.m_y));
	}

	UpdateStatusBar(event.m_x, event.m_y);
	//	SetFocus();
}

void PanelViewer::OnKeydown(wxKeyEvent& event) {
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
			UpdateIfTransformable();
			SceneMove(deplacement);
		}
		///si en mode capture : c'est le point à ajouter qui bouge !!
		else if (m_mode == MODE_CAPTURE) {
			GeometryUpdateRelative(deplacement);
		}
		///si en mode geometry moving : c'est la géométrie qui bouge !!
		else if (m_mode == MODE_GEOMETRY_MOVING) {
			GeometryMoveRelative(deplacement);
		}
	}

	event.Skip();
	UpdateStatusBar(event.m_x, event.m_y);

}

void PanelViewer::UpdateIfTransformable() {
	for (LayerControl::iterator it = m_layerControl->begin(); it != m_layerControl->end(); ++it) {
		if ((*it)->IsTransformable()) {
			(*it)->HasToBeUpdated(true);
		}
	}
}

void PanelViewer::SceneMove(const wxPoint& translation) {
	for (LayerControl::iterator it = m_layerControl->begin(); it != m_layerControl->end(); ++it) {
		if ((*it)->IsTransformable()) {
			(*it)->TranslationX((*it)->TranslationX() + translation.x * (*it)->ZoomFactor());
			(*it)->TranslationY((*it)->TranslationY() + translation.y * (*it)->ZoomFactor());
		}
	}
	Refresh();
}

bool PanelViewer::GetCoordImage(const int mouseX, const int mouseY, int &i, int &j) const {
	bool coordOK = false;
	for (LayerControl::iterator it = m_layerControl->begin(); it != m_layerControl->end() && !coordOK; ++it) {
		if ((*it)->IsVisible() && ((*it)->GetPixelValue(i, j).length() != 0)) {
			coordOK = true;
			i = static_cast<int> (-(*it)->TranslationX() + mouseX * (*it)->ZoomFactor());
			j = static_cast<int> (-(*it)->TranslationY() + mouseY * (*it)->ZoomFactor());
		}
	}
	return coordOK;
}

bool PanelViewer::GetSubPixCoordImage(const int mouseX, const int mouseY, double &i, double &j) const {
	bool coordOK = false;
	for (LayerControl::iterator it = m_layerControl->begin(); it != m_layerControl->end() && !coordOK; ++it) {
		if ((*it)->IsVisible()) {
			coordOK = true;
			i = static_cast<double> (-(*it)->TranslationX() + mouseX * (*it)->ZoomFactor()) - 0.5;
			j = static_cast<double> (-(*it)->TranslationY() + mouseY * (*it)->ZoomFactor()) - 0.5;
		}
	}
	return coordOK;
}

void PanelViewer::OpenCustomFormat(const std::string &filename) {
	std::ostringstream oss;
	oss << "File : " << __FILE__ << "\n";
	oss << "Function : " << __FUNCTION__ << "\n";
	oss << "Line : " << __LINE__ << "\n";
	oss << "Format non supporté !\n";
	::wxMessageBox(wxString(oss.str().c_str(), *wxConvCurrent));
}

void PanelViewer::UpdateStatusBar(const int i, const int j) {
	// On n'update que lorsque la status bar existe chez le parent ...
	if (m_parent->GetStatusBar()) {
		unsigned int nb = 0;
		for (LayerControl::iterator it = m_layerControl->begin(); it != m_layerControl->end(); ++it) {
			if ((*it)->IsVisible()) // && ((*it)->GetPixelValue(i, j).length() != 0))
				nb++;
		}

		if (nb == 0)
			return;

		Orientation2D ori;
		if (m_layerControl->IsOriented()) {
			++nb; //on affiche aussi les coord carto dans ce cas
			ori = m_layerControl->GetOrientation();
		}

		m_parent->GetStatusBar()->SetFieldsCount(nb + 1); //+1 pour les coord en pixels

		unsigned int count = 0;
		bool affichagePixelDone = false;
		bool affichageCartoDone = false;

		for (LayerControl::iterator it = m_layerControl->begin(); it != m_layerControl->end(); ++it) {
			std::string affichage;

			if ((*it)->IsVisible()) // && ((*it)->GetPixelValue(i, j).length() != 0))
			{
				// TODO : Nettoyer !!!
				if (!affichagePixelDone) {
					affichagePixelDone = true;
					std::ostringstream coordPixel;
					coordPixel << "Coord image : (";
					coordPixel << static_cast<int> (-(*it)->TranslationX() + i * (*it)->ZoomFactor());
					coordPixel << ",";
					coordPixel << static_cast<int> (-(*it)->TranslationY() + j * (*it)->ZoomFactor());
					coordPixel << ")";
					double dx, dy;
					GetSubPixCoordImage(i, j, dx, dy);
					coordPixel << (" - ( ");
					coordPixel << static_cast<double> (-(*it)->TranslationX() + i * (*it)->ZoomFactor()) - 0.5;
					coordPixel << ",";
					coordPixel << static_cast<double> (-(*it)->TranslationY() + j * (*it)->ZoomFactor()) - 0.5;
					coordPixel << ")";
					m_parent->SetStatusText(wxString(coordPixel.str().c_str(), *wxConvCurrent), count);
					++count;
				}

				if (!affichageCartoDone && m_layerControl->IsOriented()) {
					affichageCartoDone = true;
					std::ostringstream coordCarto;
					coordCarto << "Coord carto : (";
					coordCarto << static_cast<int> (ori.OriginX() + ori.Step() * (-(*it)->TranslationX() + i * (*it)->ZoomFactor()));
					coordCarto << ",";
					coordCarto << static_cast<int> (ori.OriginY() + ori.Step() * ((*it)->TranslationY() - j * (*it)->ZoomFactor()));
					coordCarto << ")";
					m_parent->SetStatusText(wxString(coordCarto.str().c_str(), *wxConvCurrent), count);
					++count;
				}

				affichage += boost::filesystem::basename((*it)->Name()) + " : ";
				affichage += (*it)->GetPixelValue(i, j);
				std::ostringstream oss;
				oss << 100. / (*it)->ZoomFactor();
				affichage += "-" + oss.str() + "%";
				m_parent->SetStatusText(wxString(affichage.c_str(), *wxConvCurrent), count);
				++count;
			}
		}
	}
}

void PanelViewer::Zoom(double zoomFactor, wxMouseEvent &event) {
	for (LayerControl::iterator it = m_layerControl->begin(); it != m_layerControl->end(); ++it) {
		if ((*it)->IsTransformable()) {
			(*it)->TranslationX((*it)->TranslationX() + event.m_x * (*it)->ZoomFactor() * (zoomFactor - 1));
			(*it)->TranslationY((*it)->TranslationY() + event.m_y * (*it)->ZoomFactor() * (zoomFactor - 1));
			(*it)->ZoomFactor((*it)->ZoomFactor() * zoomFactor);
			(*it)->HasToBeUpdated(true);
		}
	}

	Refresh();
}

/*
 void PanelViewer::OnChar(wxKeyEvent& event)
 {
 if (event.GetKeyCode()==(int)'l')
 m_layerControl->Show( !m_layerControl->IsVisible() );
 }
 */

void PanelViewer::AddLayer(const Layer::ptrLayerType &layer) {
	try {
		m_layerControl->AddLayer(layer);
	} catch (const std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}

void PanelViewer::ShowLayerControl(bool show) const {
	m_layerControl->Show(show);
}

//void PanelViewer::ShowPopUpMenu(const wxPoint& pos) {
//
//	PopupMenu(m_menuMain, pos.x, pos.y);
//}

void PanelViewer::OnQuit(wxCommandEvent& event) {
	Close();
}

void PanelViewer::OnSnapshot(wxCommandEvent& event) {
	wxString wildcard;
	wildcard << _("All supported files ");
	wildcard << wxT("(*.tif;*.tiff;*.png*.jpg;*.jpeg;*.bmp)|*.tif;*.tiff;*.png*.jpg;*.jpeg;*.bmp|");
	wildcard << wxT("TIFF (*.tif;*.tiff)|*.tif;*.tiff|");
	wildcard << wxT("PNG (*.png)|*.png|");
	wildcard << wxT("JPEG (*.jpg;*.jpeg)|*.jpg;*.jpeg|");
	wildcard << wxT("BMP (*.bmp)|*.bmp|");

	//	wxFileDialog *fileDialog = new wxFileDialog(this, _("Sauvegarde de la fenetre en  l'etat"), _(""), _(""), wildcard, wxFD_SAVE|wxFD_CHANGE_DIR);
	//
	//	if (fileDialog->ShowModal() == wxID_OK)
	{
		// On recupere le DC, on le met dans un bitmap et on sauve ce bitmap ...
		wxBufferedPaintDC dc(this);
		int width, height;
		this->GetClientSize(&width, &height);
		if (!dc.Ok())
			return;
		wxBitmap snap = dc.GetSelectedBitmap();
		snap.SetHeight(height);
		snap.SetWidth(width);
		wxImage im = snap.ConvertToImage();

		wxBitmapDataObject *toto = new wxBitmapDataObject(snap);

		//wxSVGFileDC SVG(wxString("test.svg",*wxConvCurrent),  width, height, 72);
		//SVG.Blit(0,0,width,height,&dc,0,0);

		if (wxTheClipboard->Open()) {
			wxTheClipboard->SetData(toto);
			wxTheClipboard->Close();
		}

		// Si on n'a pas mis d'extension, on en rajoute une ...
		//		fileDialog->AppendExtension( fileDialog->GetFilename() , _(".tif;.tiff;.png;.jpg;.jpeg;.bmp") );
		//		im.SaveFile( fileDialog->GetFilename() );
	}
}

void PanelViewer::executeMode() {
	Refresh();

	switch (m_mode) {
	case MODE_NAVIGATION:
		executeModeNavigation();
		break;
	case MODE_GEOMETRY_MOVING:
		executeModeGeometryMoving();
		break;
	case MODE_CAPTURE:
		executeModeCapture();
		break;
	case MODE_EDITION:
		executeModeEdition();
		break;
	case MODE_SELECTION:
		executeModeSelection();
		break;
	}
}

void PanelViewer::executeModeNavigation() {

}
void PanelViewer::executeModeGeometryMoving() {

}
void PanelViewer::executeModeCapture() {

}
void PanelViewer::executeModeEdition() {

}
void PanelViewer::executeModeSelection() {

}

inline double DistancePoints(const wxPoint& pt1, const wxPoint &pt2) {
	wxPoint diff(pt2 - pt1);
	return std::sqrt((double) (diff.x * diff.x + diff.y * diff.y));
}

void PanelViewer::GeometryAddPoint(const wxPoint & pt)
//coord filées par l'event (pas encore image)
{
	//	double xi,yi;
	//	GetSubPixCoordImage( x, y, xi, yi );
	int xi, yi;
	GetCoordImage(pt.x, pt.y, xi, yi);

	m_ghostLayer.m_drawPointPosition = false;
	m_ghostLayer.m_drawCircle = false;
	m_ghostLayer.m_drawLine = false;
	m_ghostLayer.m_drawRectangleSelection = false;

	switch (m_geometry) {
	case GEOMETRY_NULL:

		break;
	case GEOMETRY_CIRCLE:
		m_ghostLayer.m_drawCircle = true;
		m_ghostLayer.m_penCircle = wxPen(*wxBLUE, 2);
		m_ghostLayer.m_brushCircle = wxBrush(*wxBLUE, wxTRANSPARENT);
		if (!m_ghostLayer.m_CircleFirstPointSet) {
			m_ghostLayer.m_circle.first = wxPoint(xi, yi);
			m_ghostLayer.m_circle.second = 1;
			m_ghostLayer.m_CircleFirstPointSet = true;
		} else {
			m_ghostLayer.m_circle.second = DistancePoints(m_ghostLayer.m_circle.first, wxPoint(xi, yi));
			m_ghostLayer.m_CircleFirstPointSet = false;
			GeometryEnd();
		}
		break;
	case GEOMETRY_LINE:
		m_ghostLayer.m_drawLine = true;
		m_ghostLayer.m_penLine = wxPen(*wxBLUE, 1);

		if (!m_ghostLayer.m_lineHasBegun) {
			m_ghostLayer.m_linePoints.clear();
			m_ghostLayer.m_linePoints.push_back(wxPoint(xi, yi));
		}

		m_ghostLayer.m_lineHasBegun = true;
		m_ghostLayer.m_linePoints.push_back(wxPoint(xi, yi));
		if (m_ghostLayer.m_lineEndCapture) {
			m_ghostLayer.m_lineEndCapture = false;
			m_ghostLayer.m_lineHasBegun = false;
			GeometryEnd();
		}
		break;
	case GEOMETRY_POINT:
		m_ghostLayer.m_penPoint = wxPen(*wxGREEN, 3);
		m_ghostLayer.m_pointPosition = wxPoint(xi, yi);
		m_ghostLayer.m_drawPointPosition = true;
		//			std::cout << "point ajouté : " << xi << "," << yi << std::endl;
		GeometryEnd();
		break;
	case GEOMETRY_POLYGONE:

		break;
	case GEOMETRY_RECTANGLE:
		m_ghostLayer.m_penRectangle = wxPen(*wxRED, 2, wxDOT);
		m_ghostLayer.m_drawRectangleSelection = true;
		if (!m_ghostLayer.m_rectangleSelectionFirstPointSet) {
			m_ghostLayer.m_rectangleSelection.first = wxPoint(xi, yi);
			m_ghostLayer.m_rectangleSelection.second = wxPoint(xi, yi);
			m_ghostLayer.m_rectangleSelectionFirstPointSet = true;
		} else {
			m_ghostLayer.m_rectangleSelection.second = wxPoint(xi, yi);
			m_ghostLayer.m_rectangleSelectionFirstPointSet = false;
			GeometryEnd();
		}
		break;
	}

	executeMode();
}

void PanelViewer::GeometryMoveRelative(const wxPoint& translation) {
	switch (m_geometry) {
	case GEOMETRY_NULL:

		break;
	case GEOMETRY_CIRCLE:
		m_ghostLayer.m_circle.first += translation;
		break;
	case GEOMETRY_LINE:
		std::for_each(m_ghostLayer.m_linePoints.begin(), m_ghostLayer.m_linePoints.end(), boost::lambda::_1 += translation);
		break;
	case GEOMETRY_POINT:
		m_ghostLayer.m_pointPosition += translation;
		break;
	case GEOMETRY_POLYGONE:

		break;
	case GEOMETRY_RECTANGLE:
		m_ghostLayer.m_rectangleSelection.first += translation;
		m_ghostLayer.m_rectangleSelection.second += translation;

		break;
	}

	executeMode();
}

void PanelViewer::GeometryMoveAbsolute(const wxPoint& pt) {
	int xi, yi;
	GetCoordImage(pt.x, pt.y, xi, yi);

	switch (m_geometry) {
	case GEOMETRY_NULL:

		break;
	case GEOMETRY_CIRCLE:
		m_ghostLayer.m_circle.first = wxPoint(xi, yi);
		break;
	case GEOMETRY_LINE: {
		wxPoint bary(std::accumulate(m_ghostLayer.m_linePoints.begin(), m_ghostLayer.m_linePoints.end(), wxPoint(0, 0)));
		bary.x = bary.x / float(m_ghostLayer.m_linePoints.size());
		bary.y = bary.y / float(m_ghostLayer.m_linePoints.size());
		std::for_each(m_ghostLayer.m_linePoints.begin(), m_ghostLayer.m_linePoints.end(), boost::lambda::_1 += -bary + wxPoint(xi, yi));
	}
		break;
	case GEOMETRY_POINT:
		m_ghostLayer.m_pointPosition = wxPoint(xi, yi);
		break;
	case GEOMETRY_POLYGONE:

		break;
	case GEOMETRY_RECTANGLE:
		wxPoint rectangleDiagonale = m_ghostLayer.m_rectangleSelection.second - m_ghostLayer.m_rectangleSelection.first;
		m_ghostLayer.m_rectangleSelection.first = wxPoint(xi - float(rectangleDiagonale.x) / 2, yi - float(rectangleDiagonale.y) / 2);
		m_ghostLayer.m_rectangleSelection.second = m_ghostLayer.m_rectangleSelection.first + rectangleDiagonale;

		break;
	}

	executeMode();
}

void PanelViewer::GeometryUpdateAbsolute(const wxPoint & pt)
//coord filées par l'event (pas encore image)
{

	int xi, yi;
	GetCoordImage(pt.x, pt.y, xi, yi);

	switch (m_geometry) {
	case GEOMETRY_NULL:

		break;
	case GEOMETRY_CIRCLE:
		if (m_ghostLayer.m_CircleFirstPointSet) {
			m_ghostLayer.m_circle.second = DistancePoints(m_ghostLayer.m_circle.first, wxPoint(xi, yi));
		}
		break;
	case GEOMETRY_LINE:
		if (m_ghostLayer.m_lineHasBegun) {
			if (m_ghostLayer.m_linePoints.size() > 1)
				m_ghostLayer.m_linePoints.back() = wxPoint(xi, yi);
		}
		break;
	case GEOMETRY_POINT:
		//rien à faire pour un point
		break;
	case GEOMETRY_POLYGONE:

		break;
	case GEOMETRY_RECTANGLE:
		//si le rectangle a déjà un premier point, on met à jour le deuxième
		if (m_ghostLayer.m_rectangleSelectionFirstPointSet) {
			m_ghostLayer.m_rectangleSelection.second = wxPoint(xi, yi);
		}
		break;
	}

	executeMode();

}

void PanelViewer::GeometryUpdateRelative(const wxPoint & translation)
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
		if (m_ghostLayer.m_rectangleSelectionFirstPointSet) {
			m_ghostLayer.m_rectangleSelection.second += translation;
		}
		break;
	}

	executeMode();

}

void PanelViewer::GeometryEnd() {
	executeMode();
}

PanelViewer* createPanelViewer(wxFrame* parent) {
	return new PanelViewer(parent);
}

void PanelViewer::Register(wxFrame* parent) {
	PanelManager::Instance()->Register("PanelViewer", boost::bind(createPanelViewer, parent));
}

#if wxUSE_DRAG_AND_DROP
bool GilViewerFileDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
	// Je ne suis pas certain que ça fonctionne comme ça ...
	m_panelViewer->GetLayerControl()->AddLayersFromFiles(filenames);
	return true;
}
#endif // wxUSE_DRAG_AND_DROP
void PanelViewer::Crop() {
	// On se met en mode capture et on active la selection par rectangle
	SetModeCapture();
	SetGeometryRectangle();
	// On recherche le calque selectionne
	std::vector<boost::shared_ptr<LayerControlRow> >::iterator itr = m_layerControl->GetRows().begin();
	unsigned int i = 0, size = m_layerControl->GetRows().size();
	for (LayerControl::iterator it = m_layerControl->begin(); it != m_layerControl->end() && i < size; ++it, ++i) {
		if (m_layerControl->GetRows()[i]->m_nameStaticText->IsSelected()) {
			const VectorLayerGhost &ghost = PanelManager::Instance()->GetPanelsList()[0]->GetVectorLayerGhost();
			wxPoint p1 = ghost.m_rectangleSelection.first;
			wxPoint p2 = ghost.m_rectangleSelection.second;
			std::string layer_filename = (*it)->Filename();
			std::string ext = boost::filesystem::extension(layer_filename);
			std::string layer_base_filename = boost::filesystem::basename(layer_filename) + ext;

			boost::shared_ptr<VectorLayer> vl = boost::dynamic_pointer_cast<VectorLayer>(*it);
			wxString wildcard;
			if (vl) // calque vectoriel
			{
				wildcard << wxT("Shapefile (*.shp)|*.shp");
			} else // calque image
			{
				wildcard << _("All supported files ");
				wildcard << wxT("(*.tif;*.tiff;*.png;*.jpg;*.jpeg)|*.tif;*.tiff;*.png;*.jpg;*.jpeg|");
				wildcard << wxT("TIFF (*.tif;*.tiff)|*.tif;*.tiff|");
				wildcard << wxT("PNG (*.png)|*.png|");
				wildcard << wxT("JPEG (*.jpg;*.jpeg)|*.jpg;*.jpeg|");
			}
			wxString default_crop_filename;
			default_crop_filename << wxT("crop_");
			default_crop_filename = default_crop_filename + wxString(layer_base_filename.c_str(), *wxConvCurrent);
			wxFileDialog *fileDialog = new wxFileDialog(NULL, _("Save layer"), wxT(""), default_crop_filename, wildcard, wxFD_SAVE | wxFD_CHANGE_DIR);
			if (fileDialog->ShowModal() == wxID_OK) {
				try {
					Layer::ptrLayerType l = (*it)->crop(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::abs(p2.x - p1.x), std::abs(p2.y - p1.y), string(fileDialog->GetFilename().To8BitData()));
				} catch (std::exception &err) {
					std::ostringstream oss;
					oss << "File : " << __FILE__ << "\n";
					oss << "Function : " << __FUNCTION__ << "\n";
					oss << "Line : " << __LINE__ << "\n";
					oss << err.what();
					wxMessageBox(wxString(oss.str().c_str(), *wxConvCurrent));
				}
			}

			break;
		}
	}
}
