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

#include "FrameViewer.hpp"

#include <iostream>
#include <sstream>

#include <wx/log.h>
#include <wx/menu.h>
#include <wx/sizer.h>
#include <wx/image.h>
#include <wx/statusbr.h>
#include <wx/toolbar.h>
#include <wx/config.h>
#if defined(__WXMSW__)
#include <wx/icon.h>
#endif

#include "gui/ApplicationSettings.hpp"
#include "gui/LayerControl.hpp"
#include "gui/PanelViewer.hpp"
#include "gui/define_id.hpp"

#include "gui/resources/LOGO_MATIS_small.xpm"
#include "gui/PanelManager.h"

BEGIN_EVENT_TABLE(FrameViewer,BasicViewerFrame)
	//ADD_ITKVIEWER_EVENTS_TO_TABLE(FrameViewer)
END_EVENT_TABLE()

//IMPLEMENTS_ITKVIEWER_METHODS_FOR_EVENTS_TABLE(FrameViewer,m_drawPane)

FrameViewer::FrameViewer(wxWindow* parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style, const wxString &name) :
	BasicViewerFrame(parent, id, title, pos, size, style, name)
{
#if defined(__WXMSW__)
	// Sous windows, on va chercher l'image dans les resources
	wxIcon icon("logo_matis_small");
	SetIcon (icon);
#else
	SetIcon(wxICON(LOGO_MATIS_small));
#endif


	PanelViewer::Register(this);
	m_drawPane = PanelManager::Instance()->createObject("PanelViewer");

	m_statusBar->SetStatusText(_("GilViewer - Adrien Chauve & Olivier Tournaire"));

	//this->SetToolBar(m_drawPane->GetToolBar());


	wxAuiPaneInfo paneInfoDrawPane;
	paneInfoDrawPane.Caption( _("viewer panel") );
	paneInfoDrawPane.Center();
	paneInfoDrawPane.CloseButton(false);
	paneInfoDrawPane.CaptionVisible(false);
	m_dockManager.AddPane( m_drawPane, paneInfoDrawPane );

	m_dockManager.Update();
}

void FrameViewer::AddLayer(const Layer::ptrLayerType &layer)
{
	m_drawPane->AddLayer(layer);
}

void FrameViewer::AddLayersFromFiles(const wxArrayString &names)
{
	m_drawPane->GetLayerControl()->AddLayersFromFiles(names);
}

#if wxUSE_MENUS
void FrameViewer::BuildPluginsMenu()
{
//	this->SetMenuBar(m_drawPane->GetMenuBar());

}
#endif // wxUSE_MENUS
