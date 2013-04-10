/**
 * @author Jean-Pascal Burochin
 * @date 23/10/2012
 * @project e-PLU, WP3 : détection de façades aveugles
 *  (en vue du calcul d'enveloppes constructible)
 * @object Éditeur de rectangle dans des images
 */
#include "ImageGroundTruthPanelViewer.hpp"
#include "ImageGroundTruthEditor.hpp"
#include "GilViewer/gui/define_id.hpp"
#include "GilViewer/io/gilviewer_io_factory.hpp"
#include "GilViewer/gui/layer_control.hpp"
#include "GilViewer/layers/vector_layer_ghost.hpp"
#include <wx/toolbar.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/dirdlg.h>

#define JPB_wx_THROW(E) \
    std::ostringstream erreur; \
    erreur << "Exception" << std::endl; \
    erreur << "File : " << __FILE__ << std::endl; \
    erreur << "Function : " << __FUNCTION__ << std::endl; \
    erreur << "Line : " << __LINE__ << std::endl; \
    erreur << "Object : " << E << std::endl; \
    wxMessageBox(wxString(erreur.str().c_str(), *wxConvCurrent));

#define JPB_wx_CATCH(E) \
    catch(const std::runtime_error & e) \
    { \
        wxMessageBox(wxString(e.what(), *wxConvCurrent)); \
    } \
    catch(const std::exception & e) \
    { \
        wxMessageBox(wxString(e.what(), *wxConvCurrent)); \
    } \
    catch( ... ) \
    { \
        JPB_wx_THROW(E) \
    }

BEGIN_EVENT_TABLE(ImageGroundTruthPanelViewer, panel_viewer)
    EVT_MIDDLE_DOWN(ImageGroundTruthPanelViewer::on_middle_down)
    EVT_LEFT_DOWN(ImageGroundTruthPanelViewer::on_left_down)
    EVT_RIGHT_DOWN(ImageGroundTruthPanelViewer::on_right_down)
    EVT_KEY_DOWN(ImageGroundTruthPanelViewer::on_key_down)
    ADD_GILVIEWER_EVENTS_TO_TABLE(ImageGroundTruthPanelViewer)
END_EVENT_TABLE()

IMPLEMENTS_GILVIEWER_METHODS_FOR_EVENTS_TABLE(ImageGroundTruthPanelViewer,this)

ImageGroundTruthPanelViewer::ImageGroundTruthPanelViewer(ImageGroundTruthManager * p_ground_truth_manager, wxFrame * p_parent, wxAuiManager * p_dock_manager, wxToolBar * p_tool_bar): panel_viewer(p_parent, p_dock_manager), mp_ground_truth_manager(p_ground_truth_manager), mp_editor_bar(p_tool_bar)
{
    Initialize();
}

ImageGroundTruthPanelViewer::~ImageGroundTruthPanelViewer()
{
    CheckIfSaved();
}

void ImageGroundTruthPanelViewer::InitIO()
{
    std::vector < std::string > v_input_image_paths;
    std::string output_directory_path;
    wxArrayString v_path;
    wxFileDialog * p_file_dialog;
    std::string dummy_input_image_path;
    wxString dummy_output_directory_path;
    ImageGroundTruthManager::image_index_t path_index;

    try
    {
        CheckIfSaved();
        p_file_dialog = new wxFileDialog(NULL, wxT("Sélection des images"), wxT(""), wxT(""), wxT("*"), wxFD_OPEN | wxFD_CHANGE_DIR | wxFD_MULTIPLE | wxFD_FILE_MUST_EXIST);
        if(p_file_dialog->ShowModal() != wxID_OK)
        {
            p_file_dialog->Destroy();
            return ;
        }
        p_file_dialog->GetPaths(v_path);
        for(path_index = 0; path_index < (ImageGroundTruthManager::image_index_t) v_path.GetCount(); ++ path_index)
        {
            dummy_input_image_path = v_path[path_index].mb_str();
            v_input_image_paths.push_back(dummy_input_image_path);
        }
        p_file_dialog->Destroy();
        dummy_output_directory_path = wxDirSelector(wxT("Sélection du dossier où écrire les rectangles"));
        if(dummy_output_directory_path.empty())
        {
            if(mp_ground_truth_manager->IsEmpty())
            {
                exit(0);
            }
            mp_ground_truth_manager->Initialize(v_input_image_paths);
        }
        else
        {
            output_directory_path = dummy_output_directory_path.mb_str();
            mp_ground_truth_manager->Initialize(v_input_image_paths, output_directory_path);
        }
        Update();
    }
    JPB_wx_CATCH("échec de l'initialisation du gestionnaire de boîtes englobantes");
}

void ImageGroundTruthPanelViewer::LoadNextImage()
{
    if(mp_ground_truth_manager->IsEmpty())
    {
        return ;
    }
    try
    {
        CheckIfSaved();
        mp_ground_truth_manager->LoadNextImage();
        Update();
    }
    JPB_wx_CATCH("échec du chargement de l'image suivante");
}

void ImageGroundTruthPanelViewer::LoadPreviousImage()
{
    if(mp_ground_truth_manager->IsEmpty())
    {
        return ;
    }
    try
    {
        CheckIfSaved();
        mp_ground_truth_manager->LoadPreviousImage();
        Update();
    }
    JPB_wx_CATCH("échec du chargement de l'image précédente");
}

void ImageGroundTruthPanelViewer::Save()
{
    try
    {
        mp_ground_truth_manager->SaveBoxes();
        UpdateSaveStatus();
    }
    JPB_wx_CATCH("échec de l'enregistrement des boîtes englobantes");
}

void ImageGroundTruthPanelViewer::on_left_down(wxMouseEvent & event)
{
    if(mp_ground_truth_manager->IsEmpty())
    {
        return ;
    }

    try
    {
        m_is_last_event_related_to_openings = ! event.ControlDown();
    }
    JPB_wx_CATCH("échec de l'ajout d'une boîtes englobante");
    event.Skip();
}

void ImageGroundTruthPanelViewer::on_right_down(wxMouseEvent & event)
{
    if(mp_ground_truth_manager->IsEmpty())
    {
        return ;
    }

    try
    {
        m_is_last_event_related_to_openings = false;
    }
    JPB_wx_CATCH("échec de l'ajout d'une boîtes englobante d'occlusion");
    event.Skip();
}

void ImageGroundTruthPanelViewer::on_key_down(wxKeyEvent & event)
{
    if(event.ControlDown() && event.GetUnicodeKey() == 'O')
    {
        InitIO();
        return;
    }

    if(mp_ground_truth_manager->IsEmpty())
    {
        return ;
    }

    try
    {
        switch(event.GetKeyCode())
        {
            case WXK_PAGEUP: LoadNextImage(); break;
            case WXK_PAGEDOWN: LoadPreviousImage(); break;
        }
        if(event.ControlDown())
        {
            switch(event.GetUnicodeKey())
            {
                case 'S': Save(); break;
                case 'Z': Undo(); break;
                case 'Y': Redo(); break;
                case 'A': MarkAsBlind(); break;
                case 'I': MarkAsUnknown(); break;
                case 'R': Reject(); break;
            }
        }
    }
    JPB_wx_CATCH("échec de l'ajout d'une boîtes englobante d'occlusion");
}

void ImageGroundTruthPanelViewer::Undo()
{
    bool is_opening;

    try
    {
        if(mp_ground_truth_manager->IsEmpty())
        {
            return ;
        }
        if(! mp_ground_truth_manager->AreThereRemainingBoxesToRemove())
        {
            return ;
        }

        is_opening = mp_ground_truth_manager->IsLastBoxOpening();
        mp_ground_truth_manager->RemoveLastBox();
        if(is_opening)
        {
            RemoveLastBox(mp_opening_layer);
        }
        else
        {
            RemoveLastBox(mp_occlusion_layer);
        }
        UpdateSaveStatus();
        UpdateFacadeTypeStatus();
        mp_editor_bar->EnableTool(wxID_UNDO, mp_ground_truth_manager->AreThereRemainingBoxesToRemove());
        mp_editor_bar->EnableTool(wxID_REDO, true);
    }
    JPB_wx_CATCH("échec de l'annulation de la dernière saisie")
}

void ImageGroundTruthPanelViewer::Redo()
{
    GroundTruthBox box;

    try
    {
        if(mp_ground_truth_manager->IsEmpty())
        {
            return ;
        }
        if(! mp_ground_truth_manager->AreThereRemainingBoxesToPutBack())
        {
            return ;
        }

        box = mp_ground_truth_manager->PutBackBox();
        if(box.IsOpening())
        {
            AddBox(mp_opening_layer, box);
        }
        else
        {
            AddBox(mp_occlusion_layer, box);
        }
        UpdateSaveStatus();
        UpdateFacadeTypeStatus();
        mp_editor_bar->EnableTool(wxID_UNDO, true);
        mp_editor_bar->EnableTool(wxID_REDO, mp_ground_truth_manager->AreThereRemainingBoxesToPutBack());
    }
    JPB_wx_CATCH("échec du rétablissement de la dernière saisie")
}

void ImageGroundTruthPanelViewer::MarkAsBlind()
{
    try
    {
        if(mp_ground_truth_manager->IsEmpty())
        {
            return ;
        }
        if(! mp_ground_truth_manager->HasOpening())
        {
            mp_ground_truth_manager->MarkAsBlind();
            UpdateSaveStatus();
            UpdateFacadeTypeStatus();
        }
    }
    JPB_wx_CATCH("échec du marquage de la façade aveugle")
}

void ImageGroundTruthPanelViewer::MarkAsUnknown()
{
    bool was_saved;

    try
    {
        if(mp_ground_truth_manager->IsEmpty())
        {
            return ;
        }
        if(! mp_ground_truth_manager->HasOpening())
        {
            was_saved = mp_ground_truth_manager->IsSaved();
            mp_ground_truth_manager->MarkAsUnknown();
            UpdateFacadeTypeStatus();
            if(was_saved)
            {
                mp_ground_truth_manager->SaveBoxes();
                LoadNextImage();
            }
        }
    }
    JPB_wx_CATCH("échec du marquage de la façade de type inconnu")
}

void ImageGroundTruthPanelViewer::Reject()
{
    bool was_saved;

    try
    {
        if(mp_ground_truth_manager->IsEmpty())
        {
            return ;
        }
        if(! mp_ground_truth_manager->HasOpening())
        {
            was_saved = mp_ground_truth_manager->IsSaved();
            mp_ground_truth_manager->Reject();
            UpdateFacadeTypeStatus();
            if(was_saved)
            {
                mp_ground_truth_manager->SaveBoxes();
                LoadNextImage();
            }
        }
    }
    JPB_wx_CATCH("échec du rejet de la façade")
}

void ImageGroundTruthPanelViewer::execute_mode_capture()
{
    GroundTruthBox box;

    if(! vectorlayerghost()->complete())
    {
        return ;
    }

    if(m_is_last_event_related_to_openings)
    {
        box = AddGhostRectangle(mp_opening_layer);
        box.SetStatus(true);
    }
    else
    {
        box = AddGhostRectangle(mp_occlusion_layer);
        box.SetStatus(false);
    }
    mp_ground_truth_manager->AddBox(box);
    vectorlayerghost()->reset();
    UpdateSaveStatus();
    UpdateFacadeTypeStatus();
    mp_editor_bar->EnableTool(wxID_UNDO, true);
    mp_editor_bar->EnableTool(wxID_REDO, false);
}

void ImageGroundTruthPanelViewer::Initialize()
{
    mp_editor_bar->ToggleTool(ID_MODE_NAVIGATION, false);
    mp_editor_bar->ToggleTool(ID_MODE_CAPTURE, true);
    mp_editor_bar->EnableTool(wxID_SAVE, false);
    mp_editor_bar->EnableTool(wxID_FORWARD, false);
    mp_editor_bar->EnableTool(wxID_BACKWARD, false);
    mp_editor_bar->EnableTool(wxID_UNDO, false);
    mp_editor_bar->EnableTool(wxID_REDO, false);

    m_is_last_event_related_to_openings = true;
    mp_opening_layer = vector_layer_ptr_t(new simple_vector_layer("ouvertures"));
    mp_occlusion_layer = vector_layer_ptr_t(new simple_vector_layer("occlusions"));
    add_layer(mp_opening_layer);
    add_layer(mp_occlusion_layer);
    mp_opening_layer->polygon_border_color(* wxRED);
    mp_occlusion_layer->polygon_border_color(wxColor(255, 255, 0));
    mp_opening_layer->polygon_inner_style(wxTRANSPARENT);
    mp_occlusion_layer->polygon_inner_style(wxTRANSPARENT);
    Refresh();
    geometry_rectangle();
    mode_capture();
    SetFocus();
}

void ImageGroundTruthPanelViewer::Update()
{
    std::vector < GroundTruthBox > v_openings, v_occlusions;
    wxString imagePath;
    bool are_there_many_images;

    if(! mp_ground_truth_manager->IsEmpty())
    {
        if(mp_image_layer != image_layer::ptrLayerType())
        {
            layercontrol()->delete_layer(0);
        }
        imagePath = wxString::FromUTF8(mp_ground_truth_manager->GetInputImagePath().c_str());
        mp_image_layer = layercontrol()->add_layer_from_file(imagePath);
        try
        {
            mp_ground_truth_manager->LoadBoxes();
        }
        catch(const std::runtime_error &) { }

        mp_ground_truth_manager->GetBoxes(v_openings, v_occlusions);
        LoadBoxes(mp_opening_layer, v_openings);
        LoadBoxes(mp_occlusion_layer, v_occlusions);
        layercontrol()->swap_rows(0, 2);
        are_there_many_images = mp_ground_truth_manager->GetImagesNumber() > 1;
        UpdateSaveStatus();
        UpdateFacadeTypeStatus();
        mp_editor_bar->EnableTool(wxID_FORWARD, are_there_many_images);
        mp_editor_bar->EnableTool(wxID_BACKWARD, are_there_many_images);
        mp_editor_bar->EnableTool(wxID_UNDO, mp_ground_truth_manager->AreThereRemainingBoxesToRemove());
        mp_editor_bar->EnableTool(wxID_REDO, false);
    }
    else
    {
        UpdateFacadeTypeStatus(false);
    }
    Refresh();
}

void ImageGroundTruthPanelViewer::UpdateSaveStatus()
{
    std::string title;
    bool is_not_saved;

    if(! mp_ground_truth_manager->IsEmpty())
    {
        title = mp_ground_truth_manager->GetInputImagePath();
        is_not_saved = ! mp_ground_truth_manager->IsSaved();
        if(is_not_saved)
        {
            title += " *";
        }
        m_parent->SetTitle(wxString::FromUTF8(title.c_str()));
        mp_editor_bar->EnableTool(wxID_SAVE, is_not_saved);
    }
}

void ImageGroundTruthPanelViewer::UpdateFacadeTypeStatus()
{
    if(! mp_ground_truth_manager->IsEmpty())
    {
        mp_editor_bar->ToggleTool(wxID_FILE, mp_ground_truth_manager->IsBlind());
        mp_editor_bar->ToggleTool(wxID_FILE2, mp_ground_truth_manager->IsUnknown());
        mp_editor_bar->ToggleTool(wxID_FILE3, mp_ground_truth_manager->IsRejected());
        UpdateFacadeTypeStatus(! mp_ground_truth_manager->HasOpening());
    }
    else
    {
        UpdateFacadeTypeStatus(false);
    }
}

void ImageGroundTruthPanelViewer::UpdateFacadeTypeStatus(const bool enable)
{
    mp_editor_bar->EnableTool(wxID_FILE, enable);
    mp_editor_bar->EnableTool(wxID_FILE2, enable);
    mp_editor_bar->EnableTool(wxID_FILE3, enable);
}

void ImageGroundTruthPanelViewer::CheckIfSaved()
{
    if(! mp_ground_truth_manager->IsEmpty())
    {
        if(! mp_ground_truth_manager->IsSaved())
        {
            wxMessageDialog dialog(this, wxT("Voulez-vous enregistrer votre saisie ?"), wxT("Energistrement des boîtes englobantes"), wxYES_NO);
            if(dialog.ShowModal() == wxID_YES)
            {
                mp_ground_truth_manager->SaveBoxes();
            }
        }
    }
}

void ImageGroundTruthPanelViewer::LoadBoxes(vector_layer_ptr_t & p_layer, const std::vector < GroundTruthBox > & v_boxes)
{
    std::vector < GroundTruthBox >::const_iterator box_it;

    try
    {
        p_layer->clear();
        for(box_it = v_boxes.begin(); box_it < v_boxes.end(); ++ box_it)
        {
            AddBox(p_layer, * box_it);
        }
    }
    catch( ... )
    {
        JPB_wx_THROW("échec du chargement des polygones dans le calque")
    }
}

GroundTruthBox ImageGroundTruthPanelViewer::AddGhostRectangle(vector_layer_ptr_t & p_layer)
{
    GroundTruthBox box;
    vector_layer_ghost::Rectangle ghost_rectangle;
    std::vector < double > v_x, v_y;
    wxRealPoint ghost_pt_1, ghost_pt_2, local_pt_1, local_pt_2;

    ghost_rectangle = * vectorlayerghost()->get < vector_layer_ghost::Rectangle > ();
    ghost_pt_1 = vectorlayerghost()->transform().from_local(ghost_rectangle.first);
    ghost_pt_2 = vectorlayerghost()->transform().from_local(ghost_rectangle.second);
    local_pt_1 = wxRealPoint(p_layer->transform().to_local(ghost_pt_1));
    local_pt_2 = wxRealPoint(p_layer->transform().to_local(ghost_pt_2));
    v_x.push_back(local_pt_1.x);
    v_y.push_back(local_pt_1.y);
    v_x.push_back(local_pt_2.x);
    v_y.push_back(local_pt_1.y);
    v_x.push_back(local_pt_2.x);
    v_y.push_back(local_pt_2.y);
    v_x.push_back(local_pt_1.x);
    v_y.push_back(local_pt_2.y);
    p_layer->add_polygon(v_x, v_y);
    box = GroundTruthBox(local_pt_1.x, local_pt_1.y, local_pt_2.x, local_pt_2.y);

    return box;
}

void ImageGroundTruthPanelViewer::AddBox(vector_layer_ptr_t & p_layer, const GroundTruthBox & box)
{
    std::vector < double > v_x, v_y;

    v_x.clear();
    v_y.clear();
    v_x.push_back(box.GetXMin());
    v_x.push_back(box.GetXMax());
    v_x.push_back(box.GetXMax());
    v_x.push_back(box.GetXMin());
    v_y.push_back(box.GetYMin());
    v_y.push_back(box.GetYMin());
    v_y.push_back(box.GetYMax());
    v_y.push_back(box.GetYMax());
    p_layer->add_polygon(v_x, v_y);
    Refresh();
}

void ImageGroundTruthPanelViewer::RemoveLastBox(vector_layer_ptr_t & p_layer)
{
    std::vector < double > v_x, v_y;
    std::vector < std::vector < double > > v_polygon_x, v_polygon_y;
    std::vector < std::vector < double > >::iterator polygon_x_it, polygon_y_it;

    for(int i = 0; i < ((int) p_layer->num_polygons()) - 1; ++ i)
    {
        v_x.clear();
        v_y.clear();
        p_layer->get_polygon(i, v_x, v_y);
        v_polygon_x.push_back(v_x);
        v_polygon_y.push_back(v_y);
    }
    p_layer->clear();
    polygon_x_it = v_polygon_x.begin();
    polygon_y_it = v_polygon_y.begin();
    for(; polygon_x_it < v_polygon_x.end(); ++ polygon_x_it, ++ polygon_y_it)
    {
        p_layer->add_polygon(* polygon_x_it, * polygon_y_it);
    }
    Refresh();
}
