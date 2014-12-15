/**
 * @author Jean-Pascal Burochin
 * @date 23/10/2012
 * @project e-PLU, WP3 : détection de façades aveugles
 *  (en vue du calcul d'enveloppes constructible)
 * @object Éditeur de rectangle dans des images
 */
#ifdef WIN32
	#pragma warning(disable : 4251)
	#pragma warning(disable : 4275)
#endif
#include "ImageGroundTruthEditor.hpp"
#include "ImageGroundTruthPanelViewer.hpp"
#include "GilViewer/gui/layer_control.hpp"
#include "GilViewer/gui/define_id.hpp"
#include <wx/log.h>
#include <wx/toolbar.h>
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/statusbr.h>

ImageGroundTruthEditor::ImageGroundTruthEditor(ImageGroundTruthManager * p_manager, const wxString & title, const wxPoint & pos, const wxSize & size, long style, const wxString & name): basic_viewer_frame((wxFrame *) NULL, wxID_ANY, title, pos, size, style, name), mp_manager(p_manager)
{
    wxToolBar * p_editor_bar;
    wxAuiPaneInfo panel_viewer_setting, toolbar_setting;

    try
    {
        p_editor_bar = CreateToolBar();
        mp_panel_viewer = new ImageGroundTruthPanelViewer(p_manager, this, & m_dockManager, p_editor_bar);

        toolbar_setting.ToolbarPane();
        toolbar_setting.Top();
        panel_viewer_setting.Center();
        panel_viewer_setting.CaptionVisible(false);

        m_dockManager.DetachPane(m_baseToolBar);
        m_dockManager.AddPane(p_editor_bar, toolbar_setting);
        m_dockManager.AddPane(mp_panel_viewer, panel_viewer_setting);
        m_dockManager.Update();

        m_isLogWindowVisible = false;
        m_logWindow->Show(m_isLogWindowVisible);
        m_statusBar->SetStatusText(wxT("e-PLU project 2012-2014 - Jean-Pascal Burochin (IGN MATIS) & Bruno Vallet (IGN MATIS) & Thomas Brosset (QUELLEVILLE?)"));
    }
    JPB_CATCH("Échec de l'initialisation de l'éditeur")
}

void ImageGroundTruthEditor::InitIO(wxCommandEvent & )
{
    mp_panel_viewer->InitIO();
}

void ImageGroundTruthEditor::LoadNextImage(wxCommandEvent & )
{
    mp_panel_viewer->LoadNextImage();
}

void ImageGroundTruthEditor::LoadPreviousImage(wxCommandEvent & )
{
    mp_panel_viewer->LoadPreviousImage();
}

void ImageGroundTruthEditor::Save(wxCommandEvent & )
{
    mp_panel_viewer->Save();
}

void ImageGroundTruthEditor::Undo(wxCommandEvent & )
{
    mp_panel_viewer->Undo();
}

void ImageGroundTruthEditor::Redo(wxCommandEvent & )
{
    mp_panel_viewer->Redo();
}

void ImageGroundTruthEditor::MarkAsBlind(wxCommandEvent & )
{
    mp_panel_viewer->MarkAsBlind();
}

void ImageGroundTruthEditor::MarkAsUnknown(wxCommandEvent & )
{
    mp_panel_viewer->MarkAsUnknown();
}

void ImageGroundTruthEditor::Reject(wxCommandEvent & )
{
    mp_panel_viewer->Reject();
}

BEGIN_EVENT_TABLE(ImageGroundTruthEditor, basic_viewer_frame)
    EVT_TOOL(wxID_NEW, ImageGroundTruthEditor::InitIO)
    EVT_TOOL(wxID_FORWARD, ImageGroundTruthEditor::LoadNextImage)
    EVT_TOOL(wxID_BACKWARD, ImageGroundTruthEditor::LoadPreviousImage)
    EVT_TOOL(wxID_SAVE, ImageGroundTruthEditor::Save)
    EVT_TOOL(wxID_UNDO, ImageGroundTruthEditor::Undo)
    EVT_TOOL(wxID_REDO, ImageGroundTruthEditor::Redo)
    EVT_TOOL(wxID_FILE, ImageGroundTruthEditor::MarkAsBlind)
    EVT_TOOL(wxID_FILE2, ImageGroundTruthEditor::MarkAsUnknown)
    EVT_TOOL(wxID_FILE3, ImageGroundTruthEditor::Reject)
    ADD_GILVIEWER_EVENTS_TO_TABLE(ImageGroundTruthEditor)
END_EVENT_TABLE()

IMPLEMENTS_GILVIEWER_METHODS_FOR_EVENTS_TABLE(ImageGroundTruthEditor, mp_panel_viewer)

wxToolBar * ImageGroundTruthEditor::CreateToolBar()
{
    wxToolBar * p_editor_bar;

    p_editor_bar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTB_HORIZONTAL);
    p_editor_bar->AddTool(wxID_NEW, wxT("A"), wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_BUTTON, wxSize(22,22)), wxNullBitmap, wxITEM_NORMAL, _("entrées/sorties"));
    p_editor_bar->AddTool(wxID_SAVE, wxT("A"), wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_BUTTON, wxSize(22,22)) , wxNullBitmap, wxITEM_NORMAL, _("enregistrement saisie"));
    p_editor_bar->AddTool(wxID_FORWARD, wxT("A"), wxArtProvider::GetBitmap(wxART_GO_FORWARD, wxART_BUTTON, wxSize(22,22)) , wxNullBitmap, wxITEM_NORMAL, _("image suivante"));
    p_editor_bar->AddTool(wxID_BACKWARD, wxT("A"), wxArtProvider::GetBitmap(wxART_GO_BACK, wxART_BUTTON, wxSize(22,22)) , wxNullBitmap, wxITEM_NORMAL, _("image précédente"));
    p_editor_bar->AddSeparator();
    p_editor_bar->AddTool(ID_MODE_NAVIGATION, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("HAND_22x22")), wxNullBitmap, wxITEM_RADIO, _("navigation"));
    p_editor_bar->AddTool(ID_MODE_CAPTURE, wxT("MN"), wxXmlResource::Get()->LoadBitmap(wxT("RECTANGLE_16x16")), wxNullBitmap, wxITEM_RADIO, _("saisie"));
    p_editor_bar->AddSeparator();
    p_editor_bar->AddTool(wxID_UNDO, wxT("Z"), wxArtProvider::GetBitmap(wxART_UNDO, wxART_BUTTON, wxSize(22,22)) , wxNullBitmap, wxITEM_NORMAL, _("annulation dernière saisie"));
    p_editor_bar->AddTool(wxID_REDO, wxT("Z"), wxArtProvider::GetBitmap(wxART_REDO, wxART_BUTTON, wxSize(22,22)) , wxNullBitmap, wxITEM_NORMAL, _("rétablissement dernière saisie"));
    p_editor_bar->AddSeparator();
    p_editor_bar->AddTool(wxID_FILE, wxT("T"), wxArtProvider::GetBitmap(wxART_TICK_MARK, wxART_BUTTON, wxSize(22,22)) , wxNullBitmap, wxITEM_RADIO, _("façade aveugle"));
    p_editor_bar->AddTool(wxID_FILE2, wxT("T"), wxArtProvider::GetBitmap(wxART_QUESTION, wxART_BUTTON, wxSize(22,22)) , wxNullBitmap, wxITEM_RADIO, _("façade de type inconnu"));
    p_editor_bar->AddTool(wxID_FILE3, wxT("T"), wxArtProvider::GetBitmap(wxART_ERROR, wxART_BUTTON, wxSize(22,22)) , wxNullBitmap, wxITEM_RADIO, _("façade rejetée"));
    p_editor_bar->Realize();

    return p_editor_bar;
}
