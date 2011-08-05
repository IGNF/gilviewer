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

#ifndef _GILVIEWER_H_
#define _GILVIEWER_H_

#include <wx/app.h>

class gilviewer_frame;

class gilviewer_app: public wxApp
{
    bool OnInit();

    // Langage
    void set_langage(unsigned int language_id);

    gilviewer_frame *m_frame;
};

DECLARE_APP(gilviewer_app);

/*! \page tutorial1 Tutorial: an application on top of libGilViewer
 *  This tutorial explains how to build an application on top of <i>libGilViewer</i>. It is a minimal sample to benefits from the raster
 *  and vector visualisation offered by <i>libGilViewer</i>. The code is available in <i>libGilViewer</i> repository (directory viewer_app)<br>
 *
 *  @section section_structure Application structure
 *  Our application (<b>GilViewer</b>) is made up of two classes:<br>
 *  <ul>
 *      <li>gilviewer_frame which provides the frame to display 2D raster or vector data</li>
 *      <li>gilviewer_app which implements the main application</li>
 *  </ul>
 *  Note that you should have a basic knowledge of <a href="http://www.wxwidgets.org/">wxWidgets</a>. See the
 *  <a href="http://www.wxwidgets.org/docs/">documentation</a> for more details.
 *
 *  @section section_gilviewer_frame The display frame (gilviewer_frame)
 *
 *  @subsection subsection_gilviewer_frame_hpp gilviewer_frame.hpp
 *
 *  \dontinclude gilviewer_frame.hpp
 *  First, you need to include all required headers and forward declare some classes:
 *  \skip #include "GilViewer/layers/Layer.hpp"
 *  \until class wxMenuBar;
 *  Note that the include path for <i>libGilViewer</i> are <b>relative to viewer_app directory</b>. If you have installed <i>libGilViewer</i>
 *  use the following include directive:
 *  @code
 *  #include <GilViewer/xxx/yyy.hpp>
 *  @endcode
 *  instead of:
 *  @code
 *  #include <../src/GilViewer/xxx/yyy.hpp>
 *  @endcode
 *
 *  To benefit from the GUI functionnalities of <i>libGilViewer</i>, you subclass basic_viewer_frame. You also declare the constructor and
 *  the destructor:
 *  \skip class gilviewer_frame : public basic_viewer_frame
 *  \until virtual ~gilviewer_frame() { wxGetApp().ExitMainLoop(); }
 *  Note that the destructor forces the application to close when the frame is destroyed.
 *
 *  You then declare a panel_viewer variable which handles the display functionnalities:
 *  \skip private:
 *  \until panel_viewer* m_panelviewer;
 *
 *  Finally, you use macros to handle events:
 *  \skip DECLARE_EVENT_TABLE();
 *  \until };
 *  DECLARE_GILVIEWER_METHODS_FOR_EVENTS_TABLE() is a convenient <i>libGilViewer</i> macro which declares event handling methods handled
 *  by basic_viewer_frame (open layers, snapshot, ...). You do not need it, but it is useful to deal with toolbar events and avoid painful
 *  code.
 *
 *  @subsection subsection_gilviewer_frame_cpp gilviewer_frame.cpp
 *  \dontinclude gilviewer_frame.cpp
 *
 *  gilviewer_frame is as follows. You first include all required headers:
 *  \skip #include <wx/menu.h>
 *  \until #include "gilviewer_frame.hpp"
 *
 *  You then declare and implement the events handling methods using classical wxWidgets macros and <i>libGilViewer</i> convenient macros:
 *  \skip BEGIN_EVENT_TABLE(gilviewer_frame,basic_viewer_frame)
 *  \until IMPLEMENTS_GILVIEWER_METHODS_FOR_EVENTS_TABLE(gilviewer_frame,m_panelviewer)
 *
 *  In the constructor, the first thing you need to do is to register the class in the factory:
 *  \skip gilviewer_frame::gilviewer_frame(wxWindow* parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style, const wxString &name) :
 *  \until panel_viewer::Register(this);
 *  You are then able to build a panel instance (variable m_panelviewer) using the panel_manager factory:
 *  \skip m_panelviewer = panel_manager::instance()->create_object("PanelViewer");
 *  \until m_panelviewer = panel_manager::instance()->create_object("PanelViewer");
 *
 *  Finally, you build the GUI elements using <a href="http://www.kirix.com/labs/wxaui/documentation.html">wxAui</a> panels. The panel:
 *  \skip wxAuiPaneInfo paneInfoDrawPane;
 *  \until m_dockManager.AddPane( m_panelviewer, paneInfoDrawPane );
 *
 *  The toolbars (not required but useful):
 *  \skip wxAuiPaneInfo toolbarInfo;
 *  \until m_dockManager.AddPane( m_panelviewer->mode_and_geometry_toolbar(this), modeAndGeometryToolbarInfo );
 *
 *  All changes then need to be commited:
 *  \skip m_dockManager.Update();
 *  \until m_dockManager.Update();
 *
 *  Decorations can also be added:
 *  \skip SetMenuBar( m_panelviewer->menubar() );
 *  \until m_statusBar->SetStatusText(wxT("GilViewer - Adrien Chauve & Olivier Tournaire"));
 *
 *  @code
 *  }
 *  @endcode
 *
 *  Now, we need to build the main application.
 *
 *  @section section_gilviewer_app The main application (gilviewer_app)
 *
 *  @subsection subsection_gilviewer_app_hpp gilviewer_app.hpp
 *
 *  The header is really short:
 *  @code
 *  #include <wx/app.h>
 *  class gilviewer_frame;
 *
 *  class gilviewer_app: public wxApp
 *  {
 *      bool OnInit();
 *      gilviewer_frame *m_frame;
 *  };
 *
 *  DECLARE_APP(gilviewer_app);
 *  @endcode
 *  Note that the gilviewer_app::OnInit() method can be private since it is only used within gilviewer_app.
 *
 *  @subsection subsection_gilviewer_app_cpp gilviewer_app.cpp
 *
 *  \dontinclude gilviewer_app.cpp
 *  The implementation file is also simple. First, include the headers:
 *  \skip #include <wx/filename.h>
 *  \until #endif
 *
 *  Then, use the wx macro to implement the application:
 *  @code
 *  IMPLEMENT_APP(gilviewer_app);
 *  @endcode
 *
 *  The gilviewer_app::OnInit() method then do the main job:
 *  @code
 *  bool gilviewer_app::OnInit()
 *  {
 *  #ifdef __LINUX__
 *      setlocale(LC_ALL, "POSIX");
 *  #endif
 *
 *      register_all_file_formats();
 *      try
 *      {
 *          m_frame = new gilviewer_frame((wxFrame *)NULL, wxID_ANY, _("GilViewer"), wxPoint(50,50), wxSize(800,600));
 *          m_frame->Show();
 *      }
 *      catch( std::exception &e )
 *      {
 *          GILVIEWER_LOG_EXCEPTION("Exception")
 *          wxString message(e.what(), *wxConvCurrent);
 *          ::wxMessageBox( message );
 *      }
 *      return true;
 *  }
 *  @endcode
 *
 *  The register_all_file_formats() methods registers all available file (raster and vector) formats in the factory.
 *
 *  @section section_cmakelists Compile the application
 *
 *  TODO...
 *
 *  @section section_cmakelists Run the application
 *
 *  If everything has gone weel, you can now run the application. You should see something like this:
 *  <img src="../img/gilviewer_app_screenshot.png">.
 */



#endif /* _GILVIEWER_H_ */
