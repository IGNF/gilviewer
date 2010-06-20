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

#include <wx/statusbr.h>
#include <wx/log.h>
#include <wx/toolbar.h>
#include <wx/xrc/xmlres.h>
#include <wx/aboutdlg.h>
#include <wx/config.h>
#include <wx/image.h>
#include <wx/dialog.h>
#include <wx/html/htmlwin.h>

#include "../gui/define_id.hpp"

#include "BasicViewerFrame.h"

extern void InitXmlResource();


BEGIN_EVENT_TABLE(basic_viewer_frame,wxFrame)
        EVT_TOOL(wxID_ABOUT, basic_viewer_frame::on_about)
        EVT_TOOL(wxID_HELP, basic_viewer_frame::on_help)
        EVT_TOOL(ID_SHOW_HIDE_LOG_WINDOW, basic_viewer_frame::on_show_hide_log_window)
        //EVT_TOOL(wxID_PREFERENCES, basic_viewer_frame::OnApplicationSettings)
        END_EVENT_TABLE()

        basic_viewer_frame::basic_viewer_frame(wxWindow* parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style, const wxString &name) :
	wxFrame(parent, id, title, pos, size, style, name),
        m_isLogWindowVisible(true)
{
    wxInitAllImageHandlers();

    // Initialising resources ...
    wxXmlResource::Get()->InitAllHandlers();
    InitXmlResource();

    m_dockManager.SetManagedWindow(this);

    m_statusBar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP, wxT("statusBar"));
    SetStatusBar(m_statusBar);

    wxConfigBase *pConfig = wxConfigBase::Get();
    double fontSize;
    if ( pConfig )
        wxConfigBase::Get()->Read(wxT("/Options/FontSize"), &fontSize, 8);
    // On tente un setting de la font pour pouvoir afficher les infos dans la status bar qd il y a bcp d'images ...
    wxFont fontFrameViewer((unsigned int)fontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    m_statusBar->SetFont(fontFrameViewer);


    //Construction de la log window
    wxLog::SetActiveTarget(NULL);
    m_logWindow = new wxLogWindow(this, _("Log window"));
    m_logWindow->Show(m_isLogWindowVisible);

    //ToolBar
    m_baseToolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTB_HORIZONTAL);
    m_baseToolBar->AddTool(wxID_ABOUT, wxT("A"), wxXmlResource::Get()->LoadBitmap( wxT("DIALOG-INFORMATION_22x22") ), wxNullBitmap, wxITEM_NORMAL, _("About"));
    m_baseToolBar->AddTool(ID_SHOW_HIDE_LOG_WINDOW, wxT("SHLG"), wxXmlResource::Get()->LoadBitmap( wxT("X-OFFICE-ADDRESS-BOOK_22x22") ) , wxNullBitmap, wxITEM_NORMAL, _("Show / Hide Log Window"));
    m_baseToolBar->Realize();

    wxAuiPaneInfo paneInfoToolbar;
    paneInfoToolbar.ToolbarPane();
    paneInfoToolbar.Caption( _("Toolbar Info") );
    paneInfoToolbar.TopDockable();
    paneInfoToolbar.Top();
    paneInfoToolbar.Fixed();
    paneInfoToolbar.Resizable(false);
    paneInfoToolbar.CloseButton(false);
    paneInfoToolbar.CaptionVisible(false);
    m_dockManager.AddPane(m_baseToolBar, paneInfoToolbar);

    m_dockManager.Update();

    CenterOnScreen();

}

basic_viewer_frame::~basic_viewer_frame()
{
    m_dockManager.UnInit();
}

void basic_viewer_frame::on_show_hide_log_window(wxCommandEvent& event)
{
    m_isLogWindowVisible = !m_isLogWindowVisible;
    m_logWindow->Show(m_isLogWindowVisible);
}

void basic_viewer_frame::on_about(wxCommandEvent& event)
{
    wxAboutBox(about_info());
}

void basic_viewer_frame::on_help(wxCommandEvent& event)
{
    help()->Show(true);
}

//void BasicViewerFrame::OnApplicationSettings(wxCommandEvent& event)
//{
//	variablePanelViewer->GetApplicationSettings()->Show(true);
//}


wxAboutDialogInfo basic_viewer_frame::about_info() const
{
    wxAboutDialogInfo info;
    info.AddDeveloper(_("Authors:"));
    info.AddDeveloper(wxT("Olivier Tournaire"));
    info.AddDeveloper(wxT("Adrien Chauve"));
    info.AddDeveloper(wxT(""));
    info.AddDeveloper(_("Contributors:"));
    info.AddDeveloper(wxT("Nicolas David (CMake guru)"));
    info.SetName(wxT("GilViewer"));
    info.SetVersion(wxT("0.1.0"));
    info.SetWebSite(wxT("http://code.google.com/p/gilviewer"), _("Home page") );
    info.SetDescription(_("2D raster and vector viewer"));
    info.SetCopyright(wxT("olivier.tournaire@gmail.com\nadrien.chauve@gmail.com"));
    return info;
}

wxDialog* basic_viewer_frame::help() const
{
    wxDialog* helpDialog = new wxDialog(NULL, wxID_ANY, wxString(_("Help")));
    wxHtmlWindow* helpWindow = new wxHtmlWindow(helpDialog, wxID_ANY, wxDefaultPosition, wxSize(380, 400), wxHW_SCROLLBAR_AUTO);

    wxBoxSizer *topsizer;
    topsizer = new wxBoxSizer(wxVERTICAL);

    helpWindow->LoadPage(wxT("help/help.html"));
    topsizer->Add(helpWindow, 1, wxALL, 10);
    helpDialog->SetSizer(topsizer);
    topsizer->Fit(helpDialog);

    return helpDialog;
}




















