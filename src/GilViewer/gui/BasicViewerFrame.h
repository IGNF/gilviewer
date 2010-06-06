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

class wxLogWindow;
class wxStatusBar;
class wxToolBar;

class basic_viewer_frame : public wxFrame
{
public:
    basic_viewer_frame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE, const wxString& name = _("frame"));
    virtual ~basic_viewer_frame();

protected:
    wxStatusBar* m_statusBar;

    bool m_isLogWindowVisible;
    wxLogWindow* m_logWindow;

    wxAuiManager m_dockManager;

    wxToolBar* m_baseToolBar;

    virtual wxAboutDialogInfo getAboutInfo() const;
    virtual wxDialog* getHelp() const;

    void OnAbout(wxCommandEvent& event);
    void OnHelp(wxCommandEvent& event);
    void OnShowHideLogWindow(wxCommandEvent& event);
    void OnApplicationSettings(wxCommandEvent& event);

    DECLARE_EVENT_TABLE();
};

typedef basic_viewer_frame BasicViewerFrame;

#endif /* BASICVIEWERFRAME_H_ */
