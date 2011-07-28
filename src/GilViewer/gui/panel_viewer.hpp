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

#ifndef PANELVIEWER_HPP_
#define PANELVIEWER_HPP_

#include <wx/dnd.h>
#include <wx/panel.h>

#include "../layers/layer.hpp"

#include "../convenient/macros_gilviewer.hpp"

class application_settings;
class layer_control;
class wxLogWindow;
class wxToolBar;
class wxMenuBar;
class vector_layer_ghost;

#if wxUSE_DRAG_AND_DROP
class gilviewer_file_drop_target;
#endif // wxUSE_DRAG_AND_DROP

class panel_viewer : public wxPanel
{
public:
#if wxUSE_DRAG_AND_DROP
    friend class gilviewer_file_drop_target;
#endif // wxUSE_DRAG_AND_DROP

    static void Register(wxFrame* parent);


    virtual ~panel_viewer() {}

    void add_layer( const layer::ptrLayerType &layer );

    layer_control* layercontrol() const;
    application_settings* applicationsettings() const;

    // On la met en public pour pouvoir y acceder depuis le FrameViewer (salete de windows, il faut bien le reconnaitre ...)
    DECLARE_GILVIEWER_METHODS_FOR_EVENTS_TABLE();

    wxToolBar* main_toolbar(wxWindow* parent);
    wxToolBar* mode_and_geometry_toolbar(wxWindow* parent);
    wxMenuBar* menubar();
    bool init_toolbar();

    virtual void snap_shot(wxCommandEvent& event);

    virtual void open_custom_format(const std::string &filename);

    void mode_navigation();
    void mode_geometry_moving();
    void mode_edition();
    void mode_capture();
    void mode_selection();

    void geometry_null();
    void geometry_point();
    void geometry_circle();
    void geometry_rectangle();
    void geometry_line();
    void geometry_polygon();

    void on_snapshot(wxCommandEvent& event);

    void crop();

    // TODO : tout passer en minuscule (ou en majuscule)
    enum eMode //mode de gestion des événements
    {
        MODE_NAVIGATION, //déplacement dans la scène 2D (translation/zoom des images)
        MODE_GEOMETRY_MOVING, //déplacement de la primitive active
        MODE_EDITION, //modification de primitives existantes
        MODE_CAPTURE, //saisie de nouvelles primitives
        MODE_SELECTION //sélection raster
    };

    enum eGEOMETRY //choix de primitives
    {
        GEOMETRY_NULL,
        GEOMETRY_POINT,
        GEOMETRY_CIRCLE,
        GEOMETRY_RECTANGLE,
        GEOMETRY_LINE,
        GEOMETRY_POLYGONE
    };


    inline eMode mode() { return m_mode; }
    inline eGEOMETRY geometry() { return m_geometry; }
    inline layer::eSNAP snap() { return m_snap; }
    inline boost::shared_ptr<vector_layer_ghost> vectorlayerghost() { return m_ghostLayer; }

    DECLARE_EVENT_TABLE();

protected:
    wxFrame* m_parent;

    void show_layer_control(bool show) const;

    ///Recuperer les coord images du pointeur souris par rapport a la premiere couche image
    bool coord_image(const int mouseX, const int mouseY, int &i, int &j) const;
    bool subpix_coord_image(const int mouseX, const int mouseY, double &i, double&j) const;

    wxToolBar* m_mainToolbar;
    wxToolBar* m_modeAndGeometryToolbar;
    /// The main menu bar
    wxMenuBar* m_menuBar;
    /// The menu 'File'
    wxMenu *m_menuFile;
    /// The menu 'About'
    wxMenu *m_menuAbout;

    bool m_mouseMovementStarted;
    float m_mouseMovementInitX;
    float m_mouseMovementInitY;

    wxPoint m_translationDrag;

    // Le controle des couches
    layer_control* m_layerControl;
    application_settings* m_applicationSettings;

    //ref sur le ghostLayer du LayerControl
    boost::shared_ptr<vector_layer_ghost> m_ghostLayer;

    void execute_mode();
    virtual void execute_mode_navigation();
    virtual void execute_mode_geometry_moving();
    virtual void execute_mode_capture();
    virtual void execute_mode_edition();
    virtual void execute_mode_selection();

    /// Flag indiquant le mode navigation, saisie, ...)
    eMode m_mode;
    ///Flag indiquant la primitive géométrique en cours quand on n'est pas en mode navigation (point, rectangle, ...)
    eGEOMETRY m_geometry;
    ///Flag indiquant le mode de snap courant
    layer::eSNAP m_snap;

    ///Ajoute un point à la géométrie courante
    void geometry_add_point(const wxPoint& pt);
    ///Mets à jour la géométrie avec le point sous la souris lors d'un MouseMove, sans le sélectionner définitivement
    void geometry_update_absolute(const wxPoint& pt);
    void geometry_update_relative(const wxPoint& translation);
    ///Fin du clic, on ferme (ou pas ?) la géométrie et on exécute les traitements
    void geometry_end();

    ///Déplacement de la géométrie
    void geometry_move_absolute(const wxPoint& pt);
    void geometry_move_relative(const wxPoint& translation);

    ///Déplacement de la scene
    void scene_move(const wxPoint& translation);

    void on_paint(wxPaintEvent& evt);
    void update_statusbar(const int i, const int j);
    void on_size( wxSizeEvent &e );

    // Mouse events
    void on_mouse_move(wxMouseEvent &event);
    void on_left_down(wxMouseEvent &event);
    void on_left_up(wxMouseEvent &event);
    void on_middle_down(wxMouseEvent &event);
    void on_right_down(wxMouseEvent &event);
    void on_left_double_click(wxMouseEvent &event);
    void on_right_double_click(wxMouseEvent &event);
    void on_keydown(wxKeyEvent& event);
    void on_mouse_wheel(wxMouseEvent& event);

    void on_quit(wxCommandEvent& event);

    void zoom(double zoomFactor, wxMouseEvent &event);

    void update_if_transformable();

    ///pour ne créer des panels qu'à partir de la factory (PanelManager)
    panel_viewer(wxFrame* parent);
    friend panel_viewer* create_panel_viewer(wxFrame* parent);

};

#if wxUSE_DRAG_AND_DROP
class gilviewer_file_drop_target : public wxFileDropTarget
{
public:
    gilviewer_file_drop_target( panel_viewer* panelviewer ) : m_panelViewer(panelviewer){}
    virtual ~gilviewer_file_drop_target() {}
    bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);
private:
    panel_viewer* m_panelViewer;
};

#endif // wxUSE_DRAG_AND_DROP

#endif /*PANELVIEWER_HPP_*/
