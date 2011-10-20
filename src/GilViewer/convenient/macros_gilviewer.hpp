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

#ifndef MACROSGILVIEWER_HPP_
#define MACROSGILVIEWER_HPP_

#include "../tools/error_logger.hpp"

/**
 * Un ensemble de macros pour faciliter la vie de l'utilisateur ...
 */

/**
 * Comme son nom l'indique, cette macro permet d'ajouter les evenements "classiques" de la barre d'outils a la table d'evenements.
 * Voir FrameViewer.cpp pour un exemple d'utilisation
 */
#define ADD_GILVIEWER_EVENTS_TO_TABLE(classname) \
        EVT_TOOL(wxID_OPEN, classname::on_open_layer) \
        EVT_TOOL(ID_BASIC_SNAPSHOT, classname::on_snap_shot) \
        EVT_TOOL(ID_SHOW_HIDE_LAYER_CONTROL, classname::on_show_hide_layer_control) \
        EVT_TOOL(ID_MODE_NAVIGATION, classname::on_mode_navigation) \
        EVT_TOOL(ID_MODE_GEOMETRY_MOVING, classname::on_mode_geometry_moving) \
        EVT_TOOL(ID_MODE_EDITION, classname::on_mode_edition) \
        EVT_TOOL(ID_MODE_CAPTURE, classname::on_mode_capture) \
        EVT_TOOL(ID_MODE_SELECTION, classname::on_mode_selection) \
        EVT_TOOL(ID_GEOMETRY_NULL, classname::on_geometry_null) \
        EVT_TOOL(ID_GEOMETRY_POINT, classname::on_geometry_point) \
        EVT_TOOL(ID_GEOMETRY_CIRCLE, classname::on_geometry_circle) \
        EVT_TOOL(ID_GEOMETRY_RECTANGLE, classname::on_geometry_rectangle) \
        EVT_TOOL(ID_GEOMETRY_LINE, classname::on_geometry_line) \
        EVT_TOOL(ID_GEOMETRY_POLYGONE, classname::on_geometry_polygone) \
        EVT_TOOL(ID_CROP, classname::on_crop)



/**
 * Comme son nom l'indique, cette macro permet de declarer les evenements "classiques" de la table d'evenements.
 * Voir FrameViewer.hpp pour un exemple d'utilisation
 */
#define DECLARE_GILVIEWER_METHODS_FOR_EVENTS_TABLE() \
        void on_open_layer(wxCommandEvent& event); \
        void on_snap_shot(wxCommandEvent& event); \
        void on_show_hide_layer_control(wxCommandEvent& event); \
        void on_mode_navigation(wxCommandEvent& event); \
        void on_mode_geometry_moving(wxCommandEvent& event); \
        void on_mode_edition(wxCommandEvent& event); \
        void on_mode_capture(wxCommandEvent& event); \
        void on_mode_selection(wxCommandEvent& event); \
        void on_geometry_null(wxCommandEvent& event); \
        void on_geometry_point(wxCommandEvent& event); \
        void on_geometry_circle(wxCommandEvent& event); \
        void on_geometry_rectangle(wxCommandEvent& event); \
        void on_geometry_line(wxCommandEvent& event); \
        void on_geometry_polygone(wxCommandEvent& event); \
        void on_crop(wxCommandEvent& event);

/**
 * Comme son nom l'indique, cette macro permet d'implementer les evenements "classiques" de la table d'evenements.
 * Voir FrameViewer.cpp pour un exemple d'utilisation
 */
#define IMPLEMENTS_GILVIEWER_METHODS_FOR_EVENTS_TABLE(classname,variablePanelViewer) \
        void classname::on_open_layer(wxCommandEvent& event) \
	{ \
                variablePanelViewer->layercontrol()->on_open_layer(event); \
        } \
        void classname::on_snap_shot(wxCommandEvent& event) \
	{ \
                variablePanelViewer->snap_shot(event); \
	} \
        void classname::on_show_hide_layer_control(wxCommandEvent& event) \
	{ \
                variablePanelViewer->layercontrol()->Show(!variablePanelViewer->layercontrol()->IsVisible()); \
	} \
        void classname::on_mode_navigation(wxCommandEvent& event) \
	{ \
                variablePanelViewer->mode_navigation(); \
		wxLogMessage( _("Navigation mode!") ); \
	} \
        void classname::on_mode_geometry_moving(wxCommandEvent& event) \
	{ \
                variablePanelViewer->mode_geometry_moving(); \
		wxLogMessage( _("Geometry Moving mode!") ); \
	} \
        void classname::on_mode_edition(wxCommandEvent& event) \
	{ \
                variablePanelViewer->mode_edition(); \
		wxLogMessage( _("Editing mode!") ); \
	} \
        void classname::on_mode_capture(wxCommandEvent& event) \
	{ \
                variablePanelViewer->mode_capture(); \
		wxLogMessage( _("Capture mode!") ); \
	} \
        void classname::on_mode_selection(wxCommandEvent& event) \
	{ \
                variablePanelViewer->mode_selection(); \
		wxLogMessage( _("Selection mode!") ); \
	} \
        void classname::on_geometry_null(wxCommandEvent& event) \
	{ \
                variablePanelViewer->geometry_null(); \
		wxLogMessage( _("NULL Geometry!") ); \
	} \
        void classname::on_geometry_point(wxCommandEvent& event) \
	{ \
                variablePanelViewer->geometry_point(); \
		wxLogMessage( _("Geometry Point!") ); \
	} \
        void classname::on_geometry_circle(wxCommandEvent& event) \
	{ \
                variablePanelViewer->geometry_circle(); \
		wxLogMessage( _("Geometry Circle!") ); \
	} \
        void classname::on_geometry_rectangle(wxCommandEvent& event) \
	{ \
                variablePanelViewer->geometry_rectangle(); \
		wxLogMessage( _("Geometry Rectangle!") ); \
	} \
        void classname::on_geometry_line(wxCommandEvent& event) \
	{ \
                variablePanelViewer->geometry_line(); \
		wxLogMessage( _("Geometry Line!") ); \
	} \
        void classname::on_geometry_polygone(wxCommandEvent& event) \
	{ \
                variablePanelViewer->geometry_polygon(); \
		wxLogMessage( _("Geometry Polygon!") ); \
	} \
        void classname::on_crop(wxCommandEvent& event) \
	{ \
                variablePanelViewer->crop(); \
	}

#define INTERNAL_LOG_INFOS() \
        oss << "File: " << __FILE__ << endl; \
        oss << "Line: " << __LINE__ << endl; \
        oss << "Function: " << __FUNCTION__ << endl;

#define GILVIEWER_LOG_EXCEPTION(txt) \
    { \
    ostringstream oss; \
    oss << "[EXCEPTION]: " << txt; \
    oss << e.what() << endl; \
    INTERNAL_LOG_INFOS() \
    gilviewer_wx_error_logger::log_message(oss.str()); \
    }

#define GILVIEWER_LOG_ERROR(txt) \
    { \
    ostringstream oss; \
    oss << "[ERROR]: " << txt; \
    gilviewer_wx_error_logger::log_message(oss.str()); \
    }

#define GILVIEWER_LOG_MESSAGE(txt) \
    { \
    ostringstream oss; \
    oss << "[MESSAGE]: " << txt; \
    gilviewer_wx_error_logger::log_message(oss.str()); \
    }

#endif /* MACROSGILVIEWER_HPP_ */
