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


#include "layers/Layer.hpp"
#include "gui/BasicViewerFrame.h"

#include "GilViewer.h"

class PanelViewer;
class wxStatusBar;


class FrameViewer : public BasicViewerFrame
{
public:
	FrameViewer( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE, const wxString& name = _("frame") );
	virtual ~FrameViewer() { /*m_mgr.UnInit(); */wxGetApp().ExitMainLoop(); };

	void AddLayer( const Layer::ptrLayerType &layer);
	void AddLayersFromFiles(const wxArrayString &names);
#if wxUSE_MENUS
	void BuildPluginsMenu();
#endif // wxUSE_MENUS

private:
	PanelViewer* m_drawPane;

	DECLARE_EVENT_TABLE();
};

#endif // __FRAME_VIEWER_HPP__
