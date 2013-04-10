/**
  * @date 14/09/2012
  * @author Jean-Pascal Burochin
  * @object Boîte englobante
  */
#ifndef __Box__
#define __Box__
#include <sstream>
#include <vector>

template < class coordinate_t >
class Box
{
public:
    Box();
    Box(coordinate_t x_min, coordinate_t y_min, coordinate_t x_max, coordinate_t y_max);
    Box(const Box < coordinate_t > & box);
    coordinate_t GetXMin() const;
    coordinate_t GetYMin() const;
    coordinate_t GetXMax() const;
    coordinate_t GetYMax() const;
    coordinate_t GetWidth() const;
    coordinate_t GetHeight() const;
    void GetCenter(coordinate_t & x, coordinate_t & y) const;
    Box < coordinate_t > GetIntersection(const Box < coordinate_t > & box) const;
    bool IsDegenerated() const;
    bool IsContaining(const coordinate_t x, const coordinate_t y) const;
    void Regularize();
    void Regularize(coordinate_t limit_x_min, coordinate_t limit_y_min, coordinate_t limit_x_max, coordinate_t limit_y_max);
    void Regularize(const Box < coordinate_t > & limit_box);
    double ComputeArea() const;
    double ComputeOverlapRatio(const Box < coordinate_t > & box) const;
    double ComputeIntersectionArea(const Box < coordinate_t > box) const;

protected:
    coordinate_t m_x_min;
    coordinate_t m_x_max;
    coordinate_t m_y_min;
    coordinate_t m_y_max;
};

template < class coordinate_t >
std::ostream & operator << (std::ostream & os, const Box < coordinate_t > & box);

template < class coordinate_t >
std::istream & operator >> (std::istream & is, Box < coordinate_t > & box);

#include "Box.inl"
#endif
