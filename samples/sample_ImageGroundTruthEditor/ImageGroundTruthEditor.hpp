/**
 * @author Jean-Pascal Burochin
 * @date 23/10/2012
 * @project e-PLU, WP3 : détection de façades aveugles
 *  (en vue du calcul d'enveloppes constructible)
 * @object Éditeur de rectangle dans des images
 */
#ifndef __ImageGroundTruthEditor__
#define __ImageGroundTruthEditor__

#include "ImageGroundTruthApplication.hpp"
#include "ImageGroundTruthManager.hpp"
#include "GilViewer/gui/basic_viewer_frame.hpp"
#include "GilViewer/convenient/macros_gilviewer.hpp"

class ImageGroundTruthPanelViewer;

class ImageGroundTruthEditor : public basic_viewer_frame
{
public:
    ImageGroundTruthEditor(ImageGroundTruthManager * p_manager, const wxString& title, const wxPoint& pos = wxPoint(50,50), const wxSize& size = wxSize(800,600), long style = wxDEFAULT_FRAME_STYLE, const wxString& name = wxT("frame") );
    virtual ~ImageGroundTruthEditor() { wxGetApp().ExitMainLoop(); }

private:
    ImageGroundTruthManager mp_manager;
    ImageGroundTruthPanelViewer * mp_panel_viewer;

    void InitIO(wxCommandEvent & );
    void LoadNextImage(wxCommandEvent & );
    void LoadPreviousImage(wxCommandEvent & );
    void Save(wxCommandEvent & );
    void Undo(wxCommandEvent & );
    void Redo(wxCommandEvent & );
    void MarkAsBlind(wxCommandEvent & );
    void MarkAsUnknown(wxCommandEvent & );
    void Reject(wxCommandEvent & );
    wxToolBar * CreateToolBar();

    DECLARE_EVENT_TABLE()
    DECLARE_GILVIEWER_METHODS_FOR_EVENTS_TABLE()
};

#endif
