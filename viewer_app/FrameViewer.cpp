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

#include "gui/bitmaps/LOGO_MATIS_small.xpm"
#include "gui/PanelManager.h"

BEGIN_EVENT_TABLE(FrameViewer,wxFrame)
	ADD_ITKVIEWER_EVENTS_TO_TABLE(FrameViewer)
END_EVENT_TABLE()

IMPLEMENTS_ITKVIEWER_METHODS_FOR_EVENTS_TABLE(FrameViewer,m_drawPane)

FrameViewer::FrameViewer(wxWindow* parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style, const wxString &name) :
	wxFrame(parent, id, title, pos, size, style, name)
{
#if defined(__WXMSW__)
	// Sous windows, on va chercher l'image dans les resources
	wxIcon icon("logo_matis_small");
	SetIcon (icon);
#else
	SetIcon(wxICON(LOGO_MATIS_small));
#endif

	//m_mgr.SetManagedWindow(this);

	PanelViewer::Register(this);
	m_drawPane = PanelManager::Instance()->createObject("PanelViewer");

	m_status = m_drawPane->GetStatusBar();
	this->SetStatusBar(m_status);
	wxConfigBase *pConfig = wxConfigBase::Get();
	double fontSize;
	if ( pConfig )
		wxConfigBase::Get()->Read(_T("/Options/FontSize"), &fontSize, 8);
	// On tente un setting de la font pour pouvoir afficher les infos dans la status bar qd il y a bcp d'images ...
	wxFont fontFrameViewer((unsigned int)fontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	this->GetStatusBar()->SetFont(fontFrameViewer);

	this->SetToolBar(m_drawPane->GetToolBar());
	//m_mgr.AddPane(m_drawPane->GetToolBar(),wxTOP);

	//m_mgr.Update();

	this->Show();
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
	this->SetMenuBar(m_drawPane->GetMenuBar());

//	//plugins
//	const ArrayOfPlugins &plugins = PluginManager::Instance()->GetPluginsList();
//
//	//const std::multimap<wxString,wxString> & loadedPlugins = PluginManager::Instance()->GetLoadedPlugins();
//
//	// Bah, c'est carrement degueulasse le subMenu ...
//
//	wxMenu *menu = new wxMenu;
//	GetMenuBar()->Insert(2,menu,_("Plugins"));
//	wxMenu *menuFilters = new wxMenu;
//	menu->AppendSubMenu( menuFilters , _("Filters") );
//
//	for (size_t i=0;i<plugins.GetCount();i++)
//	{
//		unsigned int pluginID = ID_FIRST_PLUGIN + i;
//		wxMenuItem *item = new wxMenuItem(menu, pluginID, plugins[i]->getPluginName(), plugins[i]->getPluginDescription() + _(" - ") + plugins[i]->getPluginAuthor());
//		std::cout << plugins[i]->getPluginName() << std::endl;
//		if ( wxString( plugins[i]->getPluginName().c_str() , *wxConvCurrent ).Contains(_("Filter")) || wxString( plugins[i]->getPluginName().c_str() , *wxConvCurrent ).Contains(_("filter")) )
//			menuFilters->Append( item );
//		else
//			menu->Append( item );
//		this->Connect(pluginID, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&PluginBase::execute, NULL, plugins[i]);
//		plugins[i]->setPluginIndex(i);
//	}
}
#endif // wxUSE_MENUS
