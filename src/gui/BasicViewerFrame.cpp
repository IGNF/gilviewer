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

#include <wx/statusbr.h>
#include <wx/log.h>
#include <wx/toolbar.h>
#include <wx/xrc/xmlres.h>
#include <wx/aboutdlg.h>
#include <wx/config.h>

#include "gui/define_id.hpp"

#include "BasicViewerFrame.h"

extern void InitXmlResource();


BEGIN_EVENT_TABLE(BasicViewerFrame,wxFrame)
EVT_TOOL(wxID_ABOUT, BasicViewerFrame::OnAbout)
EVT_TOOL(ID_SHOW_HIDE_LOG_WINDOW, BasicViewerFrame::OnShowHideLogWindow)
//EVT_TOOL(wxID_PREFERENCES, BasicViewerFrame::OnApplicationSettings)
END_EVENT_TABLE()

BasicViewerFrame::BasicViewerFrame(wxWindow* parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style, const wxString &name) :
	wxFrame(parent, id, title, pos, size, style, name),
	m_isLogWindowVisible(false)
{
    // Initialising resources ...
    wxXmlResource::Get()->InitAllHandlers();
    InitXmlResource();


	m_dockManager.SetManagedWindow(this);

	m_statusBar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP, _("statusBar"));
	SetStatusBar(m_statusBar);

	wxConfigBase *pConfig = wxConfigBase::Get();
	double fontSize;
	if ( pConfig )
		wxConfigBase::Get()->Read(_T("/Options/FontSize"), &fontSize, 8);
	// On tente un setting de la font pour pouvoir afficher les infos dans la status bar qd il y a bcp d'images ...
	wxFont fontFrameViewer((unsigned int)fontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	m_statusBar->SetFont(fontFrameViewer);


	//Construction de la log window
	wxLog::SetActiveTarget(NULL);
	m_logWindow = new wxLogWindow(this, _("Log window"));
	m_logWindow->Show(m_isLogWindowVisible);

	//ToolBar
	m_baseToolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTB_HORIZONTAL);
	m_baseToolBar->AddTool(wxID_ABOUT, _("A"), wxXmlResource::Get()->LoadBitmap( wxT("DIALOG-INFORMATION_16x16") ), wxNullBitmap, wxITEM_NORMAL, _("About"));
	m_baseToolBar->AddTool(ID_SHOW_HIDE_LOG_WINDOW, _("SHLG"), wxXmlResource::Get()->LoadBitmap( wxT("X-OFFICE-ADDRESS-BOOK_16x16") ) , wxNullBitmap, wxITEM_NORMAL, _("Show / Hide Log Window"));
//	m_baseToolBar->AddTool(wxID_PREFERENCES, _("AS"), wxXmlResource::Get()->LoadBitmap( wxT("APPLICATIONS-SYSTEM_16x16") ) , wxNullBitmap, wxITEM_NORMAL, _("Application settings"));
	m_baseToolBar->Realize();

	wxAuiPaneInfo paneInfoToolbar;
	paneInfoToolbar.ToolbarPane();
	paneInfoToolbar.Caption( _("Base Toolbar") );
	paneInfoToolbar.Top();
	paneInfoToolbar.Fixed();
	m_dockManager.AddPane(m_baseToolBar, paneInfoToolbar);



	m_dockManager.Update();

	CenterOnScreen();

}

BasicViewerFrame::~BasicViewerFrame()
{
	m_dockManager.UnInit();
}


void BasicViewerFrame::OnShowHideLogWindow(wxCommandEvent& event)
{
	m_isLogWindowVisible = !m_isLogWindowVisible;
	m_logWindow->Show(m_isLogWindowVisible);
}

void BasicViewerFrame::OnAbout(wxCommandEvent& event)
{
	wxAboutBox(getAboutInfo());
}

//void BasicViewerFrame::OnApplicationSettings(wxCommandEvent& event)
//{
//	variablePanelViewer->GetApplicationSettings()->Show(true);
//}


wxAboutDialogInfo BasicViewerFrame::getAboutInfo() const
{
	wxAboutDialogInfo info;
	info.AddDeveloper(_("Olivier Tournaire"));
	info.AddDeveloper(_("Adrien Chauve"));
	info.SetName(_("GilViewer"));
	info.SetVersion(_("0.0.5"));
	info.SetWebSite(_("http://launchpad.net/gilviewer"), _("Home page") );
	info.SetDescription(_("2D raster and vector viewer."));
	info.SetCopyright(_T("olivier.tournaire@gmail.com\nadrien.chauve@gmail.com"));
	return info;
}























