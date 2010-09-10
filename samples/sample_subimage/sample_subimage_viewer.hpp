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

#ifndef __SAMPLE_SUBIMAGE_VIEWER_HPP__
#define __SAMPLE_SUBIMAGE_VIEWER_HPP__


#include "GilViewer/layers/layer.hpp"

#include "GilViewer/gui/basic_viewer_frame.hpp"
#include "GilViewer/convenient/macros_gilviewer.hpp"

#include "sample_subimage.hpp"

class panel_viewer;
class wxStatusBar;
class wxDialog;
class wxMenuBar;

class sample_subimage_viewer : public basic_viewer_frame
{
public:
        sample_subimage_viewer( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE, const wxString& name = wxT("frame") );
        virtual ~sample_subimage_viewer() { wxGetApp().ExitMainLoop(); };

        void add_layer( const layer::ptrLayerType &layer);
        void add_layers_from_files(const wxArrayString &names);

private:
	panel_viewer* m_drawPane;
	wxMenu* m_menuOptions;
	wxMenu* m_submenuLangage;

	DECLARE_GILVIEWER_METHODS_FOR_EVENTS_TABLE();
	DECLARE_EVENT_TABLE();
};

#endif // __SAMPLE_SUBIMAGE_VIEWER_HPP__
