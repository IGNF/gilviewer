/**
 * @author Jean-Pascal Burochin
 * @date 29/10/2012
 * @project e-PLU, WP3 : détection de façades aveugles
 *  (en vue du calcul d'enveloppes constructible)
 * @object Gestionnaire de boîtes englobantes de fenêtre ou d'occlusion
 */
#include "ImageGroundTruthManager.hpp"
#include "STL_ExtensionJPB/STL_manager.hpp"
#include <boost/filesystem/operations.hpp>

ImageGroundTruthManager::ImageGroundTruthManager( ): m_is_saved(true), m_facade_type(default_facade_type), m_openings_number(0), m_undo_number(0), m_box_index(- 1), m_image_index(- 1), m_output_directory_path("")
{

}

ImageGroundTruthManager::ImageGroundTruthManager(const ImageGroundTruthManager * manager): m_is_saved(manager->m_is_saved), m_facade_type(manager->m_facade_type), m_openings_number(manager->m_openings_number), m_undo_number(manager->m_undo_number), m_box_index(manager->m_box_index), m_image_index(manager->m_image_index), m_output_directory_path(manager->m_output_directory_path), mv_boxes(manager->mv_boxes), mv_input_image_paths(manager->mv_input_image_paths)
{

}

void ImageGroundTruthManager::Initialize(const std::vector < std::string > & v_input_image_path)
{
    Initialize(v_input_image_path, m_output_directory_path);
}

void ImageGroundTruthManager::Initialize(const std::vector < std::string > & v_input_image_paths, const std::string & output_directory_path)
{
    std::vector < std::string >::const_iterator path_it;

    mv_input_image_paths.clear();
    for(path_it = v_input_image_paths.begin(); path_it < v_input_image_paths.end(); ++ path_it)
    {
        if(get_extension(* path_it) != ".txt")
        {
            mv_input_image_paths.push_back(* path_it);
        }
    }
    m_output_directory_path = output_directory_path;
    m_image_index = mv_input_image_paths.size() - 1;
    Reset();
}

void ImageGroundTruthManager::LoadNextImage()
{
    CheckInitialization();
    m_image_index = (m_image_index + 1) % mv_input_image_paths.size();
    Reset();
}

void ImageGroundTruthManager::LoadPreviousImage()
{
    CheckInitialization();
    m_image_index = (m_image_index + mv_input_image_paths.size() - 1) % mv_input_image_paths.size();
    Reset();
}

void ImageGroundTruthManager::AddBox(const GroundTruthBox & box)
{
    CheckInitialization();
    while(AreThereRemainingBoxesToPutBack())
    {
        mv_boxes.pop_back();
    }
    mv_boxes.push_back(box);
    ++ m_box_index;
    if(box.IsOpening())
    {
        ++ m_openings_number;
    }
    m_undo_number = 0;
    m_is_saved = false;
}

void ImageGroundTruthManager::RemoveLastBox()
{
    CheckInitialization();
    if(! mv_boxes.empty())
    {
        if(IsLastBoxOpening())
        {
            -- m_openings_number;
        }
        m_box_index = std::max(- 1, m_box_index - 1);
        ++ m_undo_number;
        m_is_saved = m_undo_number == 0;
    }
}

GroundTruthBox ImageGroundTruthManager::PutBackBox()
{
    CheckInitialization();
    CheckBoxes();
    if(AreThereRemainingBoxesToPutBack())
    {
        ++ m_box_index;
        -- m_undo_number;
        m_is_saved = m_undo_number == 0;
        if(IsLastBoxOpening())
        {
            ++ m_openings_number;
        }
    }
    return mv_boxes[m_box_index];
}

void ImageGroundTruthManager::SaveBoxes()
{
    std::vector < GroundTruthBox > v_savable_boxes;

    CheckInitialization();
    v_savable_boxes = std::vector < GroundTruthBox > (mv_boxes.begin(), mv_boxes.begin() + m_box_index + 1);
    if(HasOpening() || m_facade_type == blind_facade_type)
    {
        write_vector(v_savable_boxes, GetOutputBoxesPath());
    }
    else
    {
        if(m_facade_type == unknown_facade_type)
        {
            write_vector(std::vector < coordinate_t > (1, unknown_facade_type), GetOutputBoxesPath());
        }
        else
        {
            boost::filesystem::remove(boost::filesystem::path(GetOutputBoxesPath()));
        }
    }

    m_undo_number = 0;
    m_is_saved = true;
}

void ImageGroundTruthManager::LoadBoxes()
{
    std::string path;
    std::vector < coordinate_t > v_dummy_data;
    std::vector < GroundTruthBox > v_openings, v_dummy_occlusions;

    CheckInitialization();
    path = GetOutputBoxesPath();

    try
    {
        v_dummy_data = read_vector < coordinate_t > (path);
    }
    catch(...)
    {
        Reset();
        m_facade_type = rejected_facade_type;
        return;
    }

    if(v_dummy_data.empty())
    {
        Reset();
        m_facade_type = blind_facade_type;
        return;
    }

    if(v_dummy_data.front() == unknown_facade_type)
    {
        Reset();
        m_facade_type = unknown_facade_type;
        return;
    }

    try
    {
        mv_boxes = read_vector < GroundTruthBox > (path);
        GroundTruthBox::SortBoxes(mv_boxes, v_openings, v_dummy_occlusions);
        m_openings_number = v_openings.size();
        if(m_openings_number == 0)
        {
            m_facade_type = default_facade_type;
        }
        m_box_index = mv_boxes.size() - 1;
        m_undo_number = 0;
        m_is_saved = true;
    }
    JPB_CATCH("échec du chargement des boîtes à partir du fichier \"" + path + "\"")
}

bool ImageGroundTruthManager::IsLastBoxOpening() const
{
    CheckInitialization();
    CheckBoxes();

    return mv_boxes[m_box_index].IsOpening();
}

std::string ImageGroundTruthManager::GetInputImagePath() const
{
    CheckInitialization();
    return mv_input_image_paths[m_image_index];
}

std::string ImageGroundTruthManager::GetOutputBoxesPath() const
{
    CheckInitialization();
    boost::filesystem::path path(mv_input_image_paths[m_image_index]);
    //path = m_output_directory_path / path.filename().replace_extension(ext);
    //path = m_output_directory_path + "/" + path.replace_extension(ext).filename();
    path = m_output_directory_path / boost::filesystem::path(path.replace_extension("txt").filename());

    return path.string();
}

ImageGroundTruthManager::image_index_t ImageGroundTruthManager::GetImagesNumber() const
{
    return mv_input_image_paths.size();
}

void ImageGroundTruthManager::GetBoxes(std::vector < GroundTruthBox > & v_openings, std::vector < GroundTruthBox > & v_occlusions)
{
    GroundTruthBox::SortBoxes(mv_boxes, v_openings, v_occlusions);
}

bool ImageGroundTruthManager::IsEmpty() const
{
    return mv_input_image_paths.empty();
}

bool ImageGroundTruthManager::IsSaved() const
{
    return m_is_saved;
}

bool ImageGroundTruthManager::IsBlind() const
{
    return ! HasOpening() && m_facade_type == blind_facade_type;
}

bool ImageGroundTruthManager::HasOpening() const
{
    return m_openings_number > 0;
}

bool ImageGroundTruthManager::IsUnknown() const
{
    return ! HasOpening() && m_facade_type == unknown_facade_type;
}

bool ImageGroundTruthManager::IsRejected() const
{
    return ! HasOpening() && m_facade_type == rejected_facade_type;
}

void ImageGroundTruthManager::MarkFacadeType(const unsigned char facade_type)
{
    if(HasOpening())
    {
        JPB_THROW("la façade possède des fenêtres")
    }

    if(m_facade_type != facade_type)
    {
        m_facade_type = facade_type;
        m_is_saved = false;
    }
}

void ImageGroundTruthManager::MarkAsBlind()
{
    MarkFacadeType(blind_facade_type);
}

void ImageGroundTruthManager::MarkAsUnknown()
{
    MarkFacadeType(unknown_facade_type);
}

void ImageGroundTruthManager::Reject()
{
    MarkFacadeType(rejected_facade_type);
}

bool ImageGroundTruthManager::AreThereRemainingBoxesToPutBack() const
{
    CheckInitialization();

    return m_box_index + 1 < (int) mv_boxes.size();
}

bool ImageGroundTruthManager::AreThereRemainingBoxesToRemove() const
{
    return m_box_index >= 0;
}

void ImageGroundTruthManager::CheckInitialization() const
{
    if(IsEmpty())
    {
        JPB_THROW("aucune image n'est chargée")
    }
}

void ImageGroundTruthManager::CheckBoxes() const
{
    if(mv_boxes.empty())
    {
        JPB_THROW("aucune boîte englobante n'a été saisie précédemment")
    }
}

void ImageGroundTruthManager::Reset()
{
    m_box_index = - 1;
    m_undo_number = 0;
    m_is_saved = true;
    m_facade_type = default_facade_type;
    m_openings_number = 0;
    mv_boxes.clear();
}
