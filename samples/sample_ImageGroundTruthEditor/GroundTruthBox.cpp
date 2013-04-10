/**
 * @author Jean-Pascal Burochin
 * @date 29/10/2012
 * @project e-PLU, WP3 : détection de façades aveugles
 *  (en vue du calcul d'enveloppes constructible)
 * @object Boîte englobante de fenêtre ou d'occlusion
 */
#include "GroundTruthBox.hpp"
#include "STL_ExtensionJPB/STL_manager.hpp"

GroundTruthBox::GroundTruthBox(): Box < coordinate_t > () { }

GroundTruthBox::GroundTruthBox(const coordinate_t x_min, const coordinate_t y_min, const coordinate_t x_max, const coordinate_t y_max, const bool is_opening): Box < coordinate_t > (x_min, y_min, x_max, y_max), m_is_opening(is_opening) { }

GroundTruthBox::GroundTruthBox(const GroundTruthBox & box): Box < coordinate_t > (box), m_is_opening(box.m_is_opening) { }

GroundTruthBox::GroundTruthBox(const Box < coordinate_t > & box, const bool is_opening): Box < coordinate_t > (box), m_is_opening (is_opening) { }

bool GroundTruthBox::IsOpening() const
{
    return m_is_opening;
}

void GroundTruthBox::SetStatus(const bool is_opening)
{
    m_is_opening = is_opening;
}

void GroundTruthBox::SortBoxes
(
    const std::vector < GroundTruthBox > & v_boxes,
    std::vector < GroundTruthBox > & v_openings,
    std::vector < GroundTruthBox > & v_occlusions
)
{
    std::vector < GroundTruthBox >::const_iterator box_it;
    GroundTruthBox box;

    v_openings.clear();
    v_occlusions.clear();
    for(box_it = v_boxes.begin(); box_it < v_boxes.end(); ++ box_it)
    {
        box = * box_it;
        box.Regularize();
        if(box.IsOpening())
        {
            v_openings.push_back(box);
        }
        else
        {
            v_occlusions.push_back(box);
        }
    }
}

double GroundTruthBox::ComputeIntersectionArea(const std::vector < GroundTruthBox > & v_boxes) const
{
    double intersection_area;
    std::vector < GroundTruthBox >::const_iterator box_it;

    intersection_area = .0;
    for(box_it = v_boxes.begin(); box_it < v_boxes.end(); ++ box_it)
    {
        intersection_area += ComputeIntersectionArea(* box_it);
    }

    return intersection_area;
}

std::ostream & operator << (std::ostream & os, const GroundTruthBox & box)
{
    os << box.IsOpening() << COORDINATE_SEPARATOR << (Box < GroundTruthBox::coordinate_t > ) box;

    return os;
}

std::istream & operator >> (std::istream & is, GroundTruthBox & box)
{
    Box < GroundTruthBox::coordinate_t > base_box;
    bool is_opening;

    is >> is_opening;
    is >> base_box;

    box = GroundTruthBox(base_box, is_opening);

    return is;
}
