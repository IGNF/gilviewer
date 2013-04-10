/**
 * @author Jean-Pascal Burochin
 * @date 29/10/2012
 * @project e-PLU, WP3 : détection de façades aveugles
 *  (en vue du calcul d'enveloppes constructible)
 * @object Gestionnaire de boîtes englobantes de fenêtre ou d'occlusion
 */
#ifndef __ImageGroundTruthManager__
#define __ImageGroundTruthManager__
#include "GroundTruthBox.hpp"
#include <vector>

class ImageGroundTruthManager
{
    public:
        typedef GroundTruthBox::coordinate_t coordinate_t;
        typedef long image_index_t;
        typedef short box_index_t;

        ImageGroundTruthManager();
        ImageGroundTruthManager(const ImageGroundTruthManager * manager);
        void Initialize(const std::vector < std::string > & v_input_image_path);
        void Initialize(const std::vector < std::string > & v_input_image_paths, const std::string & output_directory_path);
        std::string GetInputImagePath() const;
        std::string GetOutputBoxesPath() const;
        bool IsLastBoxOpening() const;
        bool IsEmpty() const;
        bool IsSaved() const;
        bool IsBlind() const;
        bool HasOpening() const;
        bool IsUnknown() const;
        bool IsRejected() const;
        bool AreThereRemainingBoxesToPutBack() const;
        bool AreThereRemainingBoxesToRemove() const;
        image_index_t GetImagesNumber() const;
        void LoadNextImage();
        void LoadPreviousImage();
        void AddBox(const GroundTruthBox & box);
        void RemoveLastBox();
        GroundTruthBox PutBackBox();
        void SaveBoxes();
        void LoadBoxes();
        void MarkAsBlind();
        void MarkAsUnknown();
        void Reject();
        void MarkFacadeType(const unsigned char facade_type);
        void GetBoxes(std::vector < GroundTruthBox > & v_openings, std::vector < GroundTruthBox > & v_occlusions);

    private:
        void CheckInitialization() const;
        void CheckBoxes() const;
        void Reset();
        enum
        {
            blind_facade_type,
            opening_facade_type,
            unknown_facade_type,
            rejected_facade_type
        };

        bool m_is_saved;
        unsigned char m_facade_type;
        static const unsigned char default_facade_type = blind_facade_type;
        box_index_t m_openings_number;
        short m_undo_number;
        box_index_t m_box_index;
        image_index_t m_image_index;
        std::string m_output_directory_path;
        std::vector < GroundTruthBox > mv_boxes;
        std::vector < std::string > mv_input_image_paths;
};

#endif
