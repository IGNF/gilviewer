/**
 * @author Jean-Pascal Burochin
 * @date 23/10/2012
 * @project e-PLU, WP3 : détection de façades aveugles
 *  (en vue du calcul d'enveloppes constructible)
 * @object Éditeur de rectangle dans des images
 */
#ifndef __ImageGroundTruthPanelViewer__
#define __ImageGroundTruthPanelViewer__
#include "ImageGroundTruthManager.hpp"
#include "GilViewer/gui/panel_viewer.hpp"
#include "GilViewer/layers/image_layer.hpp"
#include "GilViewer/layers/simple_vector_layer.hpp"

class ImageGroundTruthPanelViewer : public panel_viewer
{
public:
    ImageGroundTruthPanelViewer(ImageGroundTruthManager * p_ground_truth_manager, wxFrame * p_parent, wxAuiManager * p_dock_manager, wxToolBar * p_tool_bar);
    ~ImageGroundTruthPanelViewer();
    virtual void execute_mode_capture();
    void on_middle_down(wxMouseEvent &) { } // désactivation de la fenêtre d'accès aux calques
    void on_left_down(wxMouseEvent & event);
    void on_right_down(wxMouseEvent & event);
    void on_key_down(wxKeyEvent & event);
    void InitIO();
    void LoadNextImage();
    void LoadPreviousImage();
    void Save();
    void Undo();
    void Redo();
    void MarkAsBlind();
    void MarkAsUnknown();
    void Reject();

private :
    typedef boost::shared_ptr < simple_vector_layer > vector_layer_ptr_t;

    void Initialize();
    void LoadBoxes(vector_layer_ptr_t & p_layer, const std::vector < GroundTruthBox > & v_boxes);
    void AddBox(vector_layer_ptr_t & p_layer, const GroundTruthBox & box);
    GroundTruthBox AddGhostRectangle(vector_layer_ptr_t & p_layer);
    void RemoveLastBox(vector_layer_ptr_t & p_layer);
    void Update();
    void SetOpeningLayerSelection(const bool selection);
    void SetOcclusionLayerSelection(const bool selection);
    void UpdateSaveStatus();
    void UpdateFacadeTypeStatus();
    void UpdateFacadeTypeStatus(const bool enable);
    void CheckIfSaved();

    ImageGroundTruthManager * mp_ground_truth_manager;
    image_layer::ptrLayerType mp_image_layer;
    vector_layer_ptr_t mp_opening_layer;
    vector_layer_ptr_t mp_occlusion_layer;
    wxToolBar * mp_editor_bar;
    bool m_is_last_event_related_to_openings;

    DECLARE_GILVIEWER_METHODS_FOR_EVENTS_TABLE()
    DECLARE_EVENT_TABLE()
};

#endif
