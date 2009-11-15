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

#include <stdexcept>

#include "GilViewer/layers/image_types.hpp"
#include "GilViewer/layers/ImageLayer.hpp"

struct get_any_image_functor
{
    typedef boost::shared_ptr< ImageLayer::usable_images_t > result_type;
    
    template <typename ViewT>
    result_type operator()(const ViewT& src) const
    {
        std::ostringstream oss;
	oss << "Not implemented ...\n";
        oss << "File : " << __FILE__ << "\n";
        oss << "Function : " << __FUNCTION__ << "\n";
        oss << "Line : " << __LINE__ << "\n";
        throw std::logic_error(oss.str());
    }    
};

/*
#define OVERLOAD_GET_PARENTHESIS_OPERATOR( r , n , data ) template <> \
get_any_image_functor::result_type get_any_image_functor::operator()<data::view_t>(const data::view_t& v) const \
{ \
    data im( v.dimensions() ); \
    boost::gil::copy_pixels( v , view(im) ); \
    return result_type(new ImageLayer::usable_images_t(im)); \
}
*/

//BOOST_PP_SEQ_FOR_EACH( OVERLOAD_GET_PARENTHESIS_OPERATOR , ~ , GRAY_IMAGE_TYPES )
//BOOST_PP_SEQ_FOR_EACH( OVERLOAD_GET_PARENTHESIS_OPERATOR , ~ , RGB_IMAGE_TYPES )
//BOOST_PP_SEQ_FOR_EACH( OVERLOAD_GET_PARENTHESIS_OPERATOR , ~ , RGBA_IMAGE_TYPES )
