/**
 * @author Jean-Pascal Burochin
 * @date 29/10/2012
 * @project e-PLU, WP3 : détection de façades aveugles
 *  (en vue du calcul d'enveloppes constructible)
 * @object Boîte englobante de fenêtre ou d'occlusion
 */
#ifndef __GroundTruthBox__
#define __GroundTruthBox__
#include "Box/Box.hpp"

class GroundTruthBox: public Box < short >
{
    public:
        typedef short coordinate_t;

        GroundTruthBox();
        GroundTruthBox(const coordinate_t x_min, const coordinate_t y_min, const coordinate_t x_max, const coordinate_t y_max, const bool is_opening = true);
        GroundTruthBox(const Box < coordinate_t > & box, const bool is_opening = true);
        GroundTruthBox(const GroundTruthBox & box);
        void SetStatus(const bool is_opening);
        bool IsOpening() const;
        static void SortBoxes
        (
            const std::vector < GroundTruthBox > & v_boxes,
            std::vector < GroundTruthBox > & v_openings,
            std::vector < GroundTruthBox > & v_occlusions
        );
        inline double ComputeIntersectionArea(const GroundTruthBox & box) const { return Box < coordinate_t >::ComputeIntersectionArea(box); }
        double ComputeIntersectionArea(const std::vector < GroundTruthBox > & v_boxes) const;

    private:
        bool m_is_opening;
};

std::ostream & operator << (std::ostream & os, const GroundTruthBox & box);
std::istream & operator >> (std::istream & is, GroundTruthBox & box);

#endif
