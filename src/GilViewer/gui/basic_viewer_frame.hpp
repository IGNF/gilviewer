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

#ifndef BASICVIEWERFRAME_H_
#define BASICVIEWERFRAME_H_

#include <wx/aui/aui.h>
#include <wx/frame.h>
#include <wx/aboutdlg.h>

//#include "../gui/simple_text_window_impl.h"
class application_settings;

class wxLogWindow;
class wxStatusBar;
class wxToolBar;
class wxDialog;

class basic_viewer_frame : public wxFrame
{
public:
    basic_viewer_frame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE, const wxString& name = _("frame"));
    virtual ~basic_viewer_frame();

    wxAuiManager *dockManager() { return &m_dockManager; }
    application_settings* settings() const { return m_settings; }

    //const simple_text_window_impl* log_window() { return m_logWindow; }

protected:
    wxStatusBar* m_statusBar;

    bool m_isLogWindowVisible;
    wxLogWindow* m_logWindow;

    wxAuiManager m_dockManager;

    wxToolBar* m_baseToolBar;
    application_settings* m_settings;

    virtual wxAboutDialogInfo about_info() const;
    virtual wxDialog* help() const;

    void on_about(wxCommandEvent& event);
    void on_help(wxCommandEvent& event);
    void on_show_hide_log_window(wxCommandEvent& event);
    void on_application_settings(wxCommandEvent& event);

    DECLARE_EVENT_TABLE();
};

#endif /* BASICVIEWERFRAME_H_ */
