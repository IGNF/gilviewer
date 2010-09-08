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

#include <iostream>
#include <fstream>
#include <sstream>

#include <wx/log.h>
#include <wx/menu.h>
#include <wx/sizer.h>
#include <wx/image.h>
#include <wx/statusbr.h>
#include <wx/toolbar.h>
#include <wx/config.h>

#if defined(__WXMSW__)
#	include <wx/icon.h>
#endif

#include "GilViewer/gui/resources/LOGO_MATIS_small.xpm"
#include "GilViewer/gui/application_settings.hpp"
#include "GilViewer/gui/layer_control.hpp"
#include "GilViewer/gui/panel_viewer.hpp"
#include "GilViewer/gui/define_id.hpp"
#include "GilViewer/gui/panel_manager.hpp"
#include "sample_subimage_viewer.hpp"

BEGIN_EVENT_TABLE(sample_subimage_viewer,basic_viewer_frame)
ADD_GILVIEWER_EVENTS_TO_TABLE(sample_subimage_viewer)
EVT_TOOL(wxID_HELP, sample_subimage_viewer::on_help)
END_EVENT_TABLE()

IMPLEMENTS_GILVIEWER_METHODS_FOR_EVENTS_TABLE(sample_subimage_viewer,m_drawPane)

sample_subimage_viewer::sample_subimage_viewer(wxWindow* parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style, const wxString &name) :
basic_viewer_frame(parent, id, title, pos, size, style, name)
{
#if defined(__WXMSW__)
	// Sous windows, on va chercher l'image dans les resources
	wxIcon icon("logo_matis_small");
	SetIcon (icon);
#else
	SetIcon(wxICON(LOGO_MATIS_small));
#endif

	panel_viewer::Register(this);
        m_drawPane = panel_manager::instance()->create_object("PanelViewer");

	m_statusBar->SetStatusText(wxT("GilViewer - Adrien Chauve & Olivier Tournaire"));

	wxAuiPaneInfo paneInfoDrawPane;
	paneInfoDrawPane.Name( wxT("Viewer panel name") );
	paneInfoDrawPane.Caption( wxT("Viewer panel") );
	paneInfoDrawPane.TopDockable();
	paneInfoDrawPane.Center();
	paneInfoDrawPane.CloseButton(false);
	paneInfoDrawPane.CaptionVisible(false);
	m_dockManager.AddPane( m_drawPane, paneInfoDrawPane );

	wxAuiPaneInfo toolbarInfo;
	toolbarInfo.ToolbarPane();
	toolbarInfo.Caption( _("Main Toolbar") );
	toolbarInfo.TopDockable();
	toolbarInfo.Top();
	toolbarInfo.Fixed();
	toolbarInfo.Resizable(false);
	toolbarInfo.CloseButton(false);
	toolbarInfo.CaptionVisible(false);
        m_dockManager.AddPane( m_drawPane->main_toolbar(this), toolbarInfo );

	wxAuiPaneInfo modeAndGeometryToolbarInfo;
	modeAndGeometryToolbarInfo.ToolbarPane();
	modeAndGeometryToolbarInfo.Caption( _("Mode and geometry Toolbar") );
	modeAndGeometryToolbarInfo.TopDockable();
	modeAndGeometryToolbarInfo.Top();
	modeAndGeometryToolbarInfo.Fixed();
	modeAndGeometryToolbarInfo.Resizable(false);
	modeAndGeometryToolbarInfo.CloseButton(false);
	modeAndGeometryToolbarInfo.CaptionVisible(false);
        m_dockManager.AddPane( m_drawPane->mode_and_geometry_toolbar(this), modeAndGeometryToolbarInfo );

	m_dockManager.Update();

	// Menu bar
        SetMenuBar( m_drawPane->menubar() );
}

void sample_subimage_viewer::AddLayer(const layer::ptrLayerType &layer) {
        m_drawPane->add_layer(layer);
}

void sample_subimage_viewer::AddLayersFromFiles(const wxArrayString &names) {
        m_drawPane->layercontrol()->add_layers_from_files(names);
}
