/**
  * @date 14/09/2012
  * @author Jean-Pascal Burochin
  * @object Boîte englobante
  */
#include "Box.hpp"
#include "../constantes.h"
#include <sstream>
#include <fstream>
#include <stdexcept>

#define COORDINATE_SEPARATOR " "

template < class coordinate_t >
Box < coordinate_t >::Box(): m_x_min(0), m_x_max(0), m_y_min(0), m_y_max(0)
{
}

template < class coordinate_t >
Box < coordinate_t >::Box(coordinate_t x_min, coordinate_t y_min, coordinate_t x_max, coordinate_t y_max):
    m_x_min(x_min),
    m_x_max(x_max),
    m_y_min(y_min),
    m_y_max(y_max)
{
}

template < class coordinate_t >
Box < coordinate_t >::Box(const Box < coordinate_t > & box):
    m_x_min(box.m_x_min),
    m_x_max(box.m_x_max),
    m_y_min(box.m_y_min),
    m_y_max(box.m_y_max)
{
}

template < class coordinate_t >
coordinate_t Box < coordinate_t >::GetXMin() const
{
    return m_x_min;
}

template < class coordinate_t >
coordinate_t Box < coordinate_t >::GetXMax() const
{
    return m_x_max;
}

template < class coordinate_t >
coordinate_t Box < coordinate_t >::GetYMin() const
{
    return m_y_min;
}

template < class coordinate_t >
coordinate_t Box < coordinate_t >::GetYMax() const
{
    return m_y_max;
}

template < class coordinate_t >
coordinate_t Box < coordinate_t >::GetWidth() const
{
    if(m_x_min > m_x_max)
    {
        return m_x_min - m_x_max;
    }
    return m_x_max - m_x_min;
}

template < class coordinate_t >
coordinate_t Box < coordinate_t >::GetHeight() const
{
    if(m_y_min > m_y_max)
    {
        return m_y_min - m_y_max;
    }
    return m_y_max - m_y_min;
}

template < class coordinate_t >
void Box < coordinate_t >::GetCenter(coordinate_t & x, coordinate_t & y) const
{
    x = .5 * (m_x_max + m_x_min);
    y = .5 * (m_y_max + m_y_min);
}

template < class coordinate_t >
Box < coordinate_t > Box < coordinate_t >::GetIntersection(const Box < coordinate_t > & box) const
{
    Box < coordinate_t > intersection;

    intersection.m_x_min = std::max(m_x_min, box.m_x_min);
    intersection.m_y_min = std::max(m_y_min, box.m_y_min);
    intersection.m_x_max = std::max(intersection.m_x_min, std::min(m_x_max, box.m_x_max));
    intersection.m_y_max = std::max(intersection.m_y_min, std::min(m_y_max, box.m_y_max));

    return intersection;
}

template < class coordinate_t >
bool Box < coordinate_t >::IsContaining(const coordinate_t x, const coordinate_t y) const
{
    return (x >= m_x_min) && (x <= m_x_max) && (y >= m_y_min) && (y <= m_y_max);
}

template < class coordinate_t >
bool Box < coordinate_t >::IsDegenerated() const
{
    return (m_x_min > m_x_max) || (m_y_min > m_y_max);
}

template < class coordinate_t >
void Box < coordinate_t >::Regularize()
{
    if(m_x_min > m_x_max)
    {
        std::swap(m_x_min, m_x_max);
    }
    if(m_y_min > m_y_max)
    {
        std::swap(m_y_min, m_y_max);
    }
}

template < class coordinate_t >
void Box < coordinate_t >::Regularize(coordinate_t limit_x_min, coordinate_t limit_y_min, coordinate_t limit_x_max, coordinate_t limit_y_max)
{
    Regularize(Box<coordinate_t>(limit_x_min, limit_y_min, limit_x_max, limit_y_max));
}

template < class coordinate_t >
void Box < coordinate_t >::Regularize(const Box<coordinate_t> & limit_box)
{
    m_x_min = std::max(m_x_min, limit_box.m_x_min);
    m_y_min = std::max(m_y_min, limit_box.m_y_min);
    m_x_max = std::min(m_x_max, limit_box.m_x_max);
    m_y_max = std::min(m_y_max, limit_box.m_y_max);
}

template < class coordinate_t >
double Box < coordinate_t >::ComputeOverlapRatio(const Box < coordinate_t > & box) const
{
    double overlap_ratio;
    double inter_area, total_area;

    overlap_ratio = .0;
    inter_area = ComputeIntersectionArea(box);
    total_area = ComputeArea() + box.ComputeArea() - inter_area;
    if(total_area > EPS)
    {
        overlap_ratio = inter_area / total_area;
    }

    return overlap_ratio;
}

template < class coordinate_t >
double Box < coordinate_t >::ComputeArea() const
{
    double area;

    area = (m_y_max - m_y_min) * (m_x_max - m_x_min);

    return area;
}

template < class coordinate_t >
double Box < coordinate_t >::ComputeIntersectionArea(const Box < coordinate_t > box) const
{
    return GetIntersection(box).ComputeArea();
}

template < class coordinate_t >
std::ostream & operator << (std::ostream & os, const Box < coordinate_t > & box)
{
    os << box.GetXMin() << COORDINATE_SEPARATOR;
    os << box.GetYMin() << COORDINATE_SEPARATOR;
    os << box.GetXMax() << COORDINATE_SEPARATOR;
    os << box.GetYMax() << COORDINATE_SEPARATOR;

    return os;
}

template < class coordinate_t >
std::istream & operator >> (std::istream & is, Box < coordinate_t > & box)
{
    coordinate_t x_min, y_min, x_max, y_max;

    is >> x_min ;
    is >> y_min ;
    is >> x_max ;
    is >> y_max ;
    box = Box < coordinate_t > (x_min, y_min, x_max, y_max);

    return is;
}
