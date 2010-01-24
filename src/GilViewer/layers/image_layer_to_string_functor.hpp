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
#include <boost/preprocessor/seq/for_each.hpp>

#include "GilViewer/layers/image_types.hpp"

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
    typename boost::enable_if< boost::mpl::contains< boost::mpl::transform<gray_image_types,add_view_type<boost::mpl::_1> >::type,
    ViewType>,
    result_type>::type operator()(const ViewType& v)
    {
        if (isInside(v, i_,j_))
            oss_ << (int) boost::gil::at_c<0>( v(i_,j_) );
        else
            oss_ << "outside";
    }

    template<class ViewType>
    typename boost::enable_if< boost::mpl::or_< boost::mpl::contains< boost::mpl::transform< rgb_image_types,
    add_view_type<boost::mpl::_1 > >::type,
    ViewType>,
    boost::mpl::contains< boost::mpl::transform< rgba_image_types,
    add_view_type<boost::mpl::_1 > >::type,
    ViewType>
    >,
    result_type>::type operator()(const ViewType& v)
    {
        using namespace boost::gil;

        if (isInside(v, i_,j_))
            oss_ << (int)at_c<0>(v(i_,j_)) << "," << (int)at_c<1>(v(i_,j_)) << "," << (int)at_c<2>(v(i_,j_));
        else
            oss_ << "outside";
    }

    const int i_, j_;
    std::ostringstream& oss_;
};
