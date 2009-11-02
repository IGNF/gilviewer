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

    any_view_image_position_to_string_functor(const int i, const int j, std::ostringstream& oss):i_(i), j_(j), oss_(oss){}

    template <typename View>
    result_type operator()(const View& v)
    {
    	if(isInside(v, i_,j_))
		   oss_ << (int) boost::gil::at_c<0>( v(i_,j_) );
    	else
    		oss_ << "outside";
    }

    const int i_, j_;
    std::ostringstream& oss_;
};

#define OVERLOAD_IMAGE_POSITION_TO_STRING_PARENTHESIS_OPERATOR( r , n , data ) template <> \
any_view_image_position_to_string_functor::result_type any_view_image_position_to_string_functor::operator()<data::view_t>(const data::view_t& v) \
{ \
    if(isInside(v, i_,j_)) \
	oss_ << (int) boost::gil::at_c<0>(v(i_,j_)) << "," << (int) boost::gil::at_c<1>(v(i_,j_)) << "," << (int) boost::gil::at_c<2>(v(i_,j_)); \
    else \
        oss_ << "outside"; \
}

BOOST_PP_SEQ_FOR_EACH( OVERLOAD_IMAGE_POSITION_TO_STRING_PARENTHESIS_OPERATOR , ~ , RGB_IMAGE_TYPES )
BOOST_PP_SEQ_FOR_EACH( OVERLOAD_IMAGE_POSITION_TO_STRING_PARENTHESIS_OPERATOR , ~ , RGBA_IMAGE_TYPES )
