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

#ifndef __FRAME_VIEWER_HPP__
#define __FRAME_VIEWER_HPP__


#include "../src/GilViewer/layers/Layer.hpp"

#include "../src/GilViewer/gui/BasicViewerFrame.h"
#include "../src/GilViewer/convenient/MacrosGilViewer.hpp"

#include "GilViewer.h"

class panel_viewer;
class wxStatusBar;
class wxDialog;
class wxMenuBar;

class FrameViewer : public BasicViewerFrame
{
public:
	FrameViewer( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE, const wxString& name = wxT("frame") );
	virtual ~FrameViewer() { wxGetApp().ExitMainLoop(); };

	void AddLayer( const Layer::ptrLayerType &layer);
	void AddLayersFromFiles(const wxArrayString &names);

private:
	panel_viewer* m_drawPane;
	wxMenu* m_menuOptions;
	wxMenu* m_submenuLangage;

	DECLARE_GILVIEWER_METHODS_FOR_EVENTS_TABLE();
	DECLARE_EVENT_TABLE();
};

#endif // __FRAME_VIEWER_HPP__
