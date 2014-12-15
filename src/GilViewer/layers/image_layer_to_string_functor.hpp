/***********************************************************************

This file is part of the GilViewer project source files.

GilViewer is an open source 2D viewer (raster and vector) based on Boost
GIL and wxWidgets.


Homepage:

	http://code.google.com/p/gilviewer

Copyright:

	Institut Geographique National (2009)

Authors:

	Olivier Tournaire, Adrien Chauve




    GilViewer is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GilViewer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with GilViewer.  If not, see <http://www.gnu.org/licenses/>.

***********************************************************************/

#ifndef TOSTRING_FUNCTOR
#define TOSTRING_FUNCTOR

#include <boost/gil/pixel.hpp>
#include <boost/gil/extension/io_new/detail/gil_extensions.hpp>

template <typename Type>
struct get_right_type_for_display
{
    typedef Type type;
};

template <>
struct get_right_type_for_display<unsigned char>
{
    typedef unsigned int type;
};

template <>
struct get_right_type_for_display<char>
{
    typedef int type;
};

template<typename ViewType, typename CoordType>
inline bool isInside(const ViewType& v, const CoordType i, const CoordType j)
{
    return i>=0 && j>=0 && i<v.width() && j<v.height();
}

struct any_view_image_position_to_string_functor
{
    typedef void result_type;

    any_view_image_position_to_string_functor(const int i, const int j, std::ostringstream& oss):i_(i), j_(j), oss_(oss) {}

    template <typename ViewType>
    typename boost::enable_if_c<
      boost::gil::num_channels<typename ViewType::value_type>::value == 1,
      result_type >::type
    operator()(const ViewType& v)
    {
        typedef typename boost::gil::get_pixel_type<ViewType>::type g_pixel_t;
        typedef typename boost::gil::kth_element_type<g_pixel_t,0>::type element_0_t;
        if (isInside(v, i_,j_))
            oss_ << (typename get_right_type_for_display<element_0_t>::type) boost::gil::at_c<0>( v(i_,j_) );
        else
            oss_ << "outside";
    }

    
    template <typename ViewType>
    typename boost::enable_if_c<
      boost::gil::num_channels<typename ViewType::value_type>::value >= 3,
      result_type >::type
    operator()(const ViewType& v)
    {
        using namespace boost::gil;
        typedef typename get_pixel_type<ViewType>::type g_pixel_t;
        typedef typename kth_element_type<g_pixel_t,0>::type element_0_t;
        typedef typename kth_element_type<g_pixel_t,1>::type element_1_t;
        typedef typename kth_element_type<g_pixel_t,2>::type element_2_t;

        if (isInside(v, i_,j_))
            oss_ << (typename get_right_type_for_display<element_0_t>::type)boost::gil::at_c<0>(v(i_,j_)) << ","
                 << (typename get_right_type_for_display<element_1_t>::type)boost::gil::at_c<1>(v(i_,j_)) << ","
                 << (typename get_right_type_for_display<element_2_t>::type)boost::gil::at_c<2>(v(i_,j_));
        else
            oss_ << "outside";
    }

    const int i_, j_;
    std::ostringstream& oss_;
};

#endif // TOSTRING_FUNCTOR
