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

#ifdef _WINDOWS
#	undef min
#	undef max
#endif // _WINDOWS

#include <boost/algorithm/minmax.hpp>
#include <boost/algorithm/minmax_element.hpp>

#include <boost/preprocessor/seq/for_each.hpp>

#include "GilViewer/layers/image_types.hpp"

struct pixel_compare_less
{
	template <typename PixelType>
	bool operator()( const PixelType& p1 , const PixelType& p2 ) const
	{
		return boost::gil::at_c<0>(p1) < boost::gil::at_c<0>(p2);
	}
};

struct any_view_min_max
{
    typedef std::pair<float, float> result_type;

    template <typename View>
    result_type operator()(const View& v) const
    {
        typedef typename View::iterator iterator;
        std::pair< iterator, iterator > result = boost::minmax_element( v.begin() , v.end() , pixel_compare_less() );
    	return std::make_pair( *(result.first) , *(result.second) );
    }
};

#define OVERLOAD_MIN_MAX_PARENTHESIS_OPERATOR( r , n , data ) template <> \
any_view_min_max::result_type any_view_min_max::operator()<data::view_t>(const data::view_t& v) const \
{ \
    using namespace std; \
    \
    vector<float> min_max_over_channels; \
    min_max_over_channels.reserve(6); \
    \
    typedef boost::gil::kth_channel_view_type<0,data::view_t>::type::iterator iterator_r; \
    pair< iterator_r , iterator_r > result_r = boost::minmax_element( boost::gil::kth_channel_view<0>(v).begin() , boost::gil::kth_channel_view<0>(v).end() , pixel_compare_less() ); \
    min_max_over_channels.push_back( *(result_r.first) ); \
    min_max_over_channels.push_back( *(result_r.second) ); \
    \
    typedef boost::gil::kth_channel_view_type<1,data::view_t>::type::iterator iterator_g; \
    pair< iterator_r , iterator_g > result_g = boost::minmax_element( boost::gil::kth_channel_view<1>(v).begin() , boost::gil::kth_channel_view<1>(v).end() , pixel_compare_less() ); \
    min_max_over_channels.push_back( *(result_g.first) ); \
    min_max_over_channels.push_back( *(result_g.second) ); \
    \
    typedef boost::gil::kth_channel_view_type<2,data::view_t>::type::iterator iterator_b; \
    pair< iterator_b , iterator_b > result_b = boost::minmax_element( boost::gil::kth_channel_view<2>(v).begin() , boost::gil::kth_channel_view<2>(v).end() , pixel_compare_less() ); \
    min_max_over_channels.push_back( *(result_b.first) ); \
    min_max_over_channels.push_back( *(result_b.second) ); \
    \
    pair< vector<float>::iterator , vector<float>::iterator> min_and_max = boost::minmax_element( min_max_over_channels.begin() , min_max_over_channels.end() ); \
    \
    return std::make_pair( *(min_and_max.first) , *(min_and_max.second) ); \
}

BOOST_PP_SEQ_FOR_EACH( OVERLOAD_MIN_MAX_PARENTHESIS_OPERATOR , ~ , RGB_IMAGE_TYPES )
BOOST_PP_SEQ_FOR_EACH( OVERLOAD_MIN_MAX_PARENTHESIS_OPERATOR , ~ , RGBA_IMAGE_TYPES )
