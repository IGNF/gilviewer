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

#ifdef WIN32
#	undef min
#	undef max
#endif // WIN32

struct min_max_functor
{
	min_max_functor():
		m_min(std::numeric_limits<float>::max()),
		m_max(std::numeric_limits<float>::min())
		{}

	template<class PixelT>
	inline void operator()(const PixelT& src)
	{
		using namespace std;
		m_min = min(static_cast<float>(at_c<0>(src)), m_min);
		m_max = max(static_cast<float>(at_c<0>(src)), m_max);
	}

    template <class PixelType>
    inline void compute_min_max_3_channels(const PixelType& p)
    {
        using namespace std;
        m_min = min(static_cast<float>(at_c<0>(p)), m_min);
        m_max = max(static_cast<float>(at_c<0>(p)), m_max);
        m_min = min(static_cast<float>(at_c<1>(p)), m_min);
        m_max = max(static_cast<float>(at_c<1>(p)), m_max);
        m_min = min(static_cast<float>(at_c<2>(p)), m_min);
        m_max = max(static_cast<float>(at_c<2>(p)), m_max);
    }

	float m_min, m_max;
};

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/range_c.hpp>

template<>
void min_max_functor::operator()<rgb8_pixel_t>(const rgb8_pixel_t& src)
{
    compute_min_max_3_channels(src);
    //boost::mpl::for_each< boost::mpl::range_c<int,0,2> >(boost::bind(&foobar, _1));
}

template<>
void min_max_functor::operator()<rgb16_pixel_t>(const rgb16_pixel_t& src)
{
    compute_min_max_3_channels(src);
}

template<>
void min_max_functor::operator()<rgba8_pixel_t>(const rgba8_pixel_t& src)
{
    compute_min_max_3_channels(src);
}

struct any_view_min_max
{
    typedef std::pair<float, float> result_type;

    template <typename View>
    result_type operator()(View& vue) const
    {
    	min_max_functor f = for_each_pixel(vue, min_max_functor());
    	return std::make_pair(f.m_min, f.m_max);
    }
};


///////////////// Optimized version /////////////////
struct pixel_compare_less
{
	template <typename PixelType>
	bool operator()( const PixelType& p1 , const PixelType& p2 ) const
	{
		return at_c<0>(p1) < at_c<0>(p2);
	}
};

#include <boost/algorithm/minmax.hpp>
#include <boost/algorithm/minmax_element.hpp>

struct any_view_min_max_optimized
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

template <>
any_view_min_max_optimized::result_type any_view_min_max_optimized::operator()<rgb8_view_t>(const rgb8_view_t& v) const
{
    using namespace boost;
    using namespace std;

    //typedef rgb8_view_t::value_type value_type;
    vector<float> min_max_over_channels;
    min_max_over_channels.reserve(6);

    typedef kth_channel_view_type<0,rgb8_view_t>::type::iterator iterator_r;
    pair< iterator_r , iterator_r > result_r = minmax_element( kth_channel_view<0>(v).begin() , kth_channel_view<0>(v).end() , pixel_compare_less() );
    min_max_over_channels.push_back( *(result_r.first) );
    min_max_over_channels.push_back( *(result_r.second) );

    typedef kth_channel_view_type<1,rgb8_view_t>::type::iterator iterator_g;
    pair< iterator_r , iterator_g > result_g = minmax_element( kth_channel_view<1>(v).begin() , kth_channel_view<1>(v).end() , pixel_compare_less() );
    min_max_over_channels.push_back( *(result_g.first) );
    min_max_over_channels.push_back( *(result_g.second) );

    typedef kth_channel_view_type<2,rgb8_view_t>::type::iterator iterator_b;
    pair< iterator_b , iterator_b > result_b = minmax_element( kth_channel_view<2>(v).begin() , kth_channel_view<2>(v).end() , pixel_compare_less() );
    min_max_over_channels.push_back( *(result_b.first) );
    min_max_over_channels.push_back( *(result_b.second) );

    pair< vector<float>::iterator , vector<float>::iterator> min_and_max = minmax_element( min_max_over_channels.begin() , min_max_over_channels.end() );

    return std::make_pair( *(min_and_max.first) , *(min_and_max.second) );
}

template <>
any_view_min_max_optimized::result_type any_view_min_max_optimized::operator()<rgb16_view_t>(const rgb16_view_t& v) const
{
    using namespace boost;
    using namespace std;

    //typedef rgb8_view_t::value_type value_type;
    vector<float> min_max_over_channels;
    min_max_over_channels.reserve(6);

    typedef kth_channel_view_type<0,rgb16_view_t>::type::iterator iterator_r;
    pair< iterator_r , iterator_r > result_r = minmax_element( kth_channel_view<0>(v).begin() , kth_channel_view<0>(v).end() , pixel_compare_less() );
    min_max_over_channels.push_back( *(result_r.first) );
    min_max_over_channels.push_back( *(result_r.second) );

    typedef kth_channel_view_type<1,rgb16_view_t>::type::iterator iterator_g;
    pair< iterator_r , iterator_g > result_g = minmax_element( kth_channel_view<1>(v).begin() , kth_channel_view<1>(v).end() , pixel_compare_less() );
    min_max_over_channels.push_back( *(result_g.first) );
    min_max_over_channels.push_back( *(result_g.second) );

    typedef kth_channel_view_type<2,rgb16_view_t>::type::iterator iterator_b;
    pair< iterator_b , iterator_b > result_b = minmax_element( kth_channel_view<2>(v).begin() , kth_channel_view<2>(v).end() , pixel_compare_less() );
    min_max_over_channels.push_back( *(result_b.first) );
    min_max_over_channels.push_back( *(result_b.second) );

    pair< vector<float>::iterator , vector<float>::iterator> min_and_max = minmax_element( min_max_over_channels.begin() , min_max_over_channels.end() );

    return std::make_pair( *(min_and_max.first) , *(min_and_max.second) );
}

template <>
any_view_min_max_optimized::result_type any_view_min_max_optimized::operator()<rgb32_view_t>(const rgb32_view_t& v) const
{
    using namespace boost;
    using namespace std;

    //typedef rgb8_view_t::value_type value_type;
    vector<float> min_max_over_channels;
    min_max_over_channels.reserve(6);

    typedef kth_channel_view_type<0,rgb32_view_t>::type::iterator iterator_r;
    pair< iterator_r , iterator_r > result_r = minmax_element( kth_channel_view<0>(v).begin() , kth_channel_view<0>(v).end() , pixel_compare_less() );
    min_max_over_channels.push_back( *(result_r.first) );
    min_max_over_channels.push_back( *(result_r.second) );

    typedef kth_channel_view_type<1,rgb32_view_t>::type::iterator iterator_g;
    pair< iterator_r , iterator_g > result_g = minmax_element( kth_channel_view<1>(v).begin() , kth_channel_view<1>(v).end() , pixel_compare_less() );
    min_max_over_channels.push_back( *(result_g.first) );
    min_max_over_channels.push_back( *(result_g.second) );

    typedef kth_channel_view_type<2,rgb32_view_t>::type::iterator iterator_b;
    pair< iterator_b , iterator_b > result_b = minmax_element( kth_channel_view<2>(v).begin() , kth_channel_view<2>(v).end() , pixel_compare_less() );
    min_max_over_channels.push_back( *(result_b.first) );
    min_max_over_channels.push_back( *(result_b.second) );

    pair< vector<float>::iterator , vector<float>::iterator> min_and_max = minmax_element( min_max_over_channels.begin() , min_max_over_channels.end() );

    return std::make_pair( *(min_and_max.first) , *(min_and_max.second) );
}

template <>
any_view_min_max_optimized::result_type any_view_min_max_optimized::operator()<rgba8_view_t>(const rgba8_view_t& v) const
{
    using namespace boost;
    using namespace std;

    //typedef rgb8_view_t::value_type value_type;
    vector<float> min_max_over_channels;
    min_max_over_channels.reserve(6);

    typedef kth_channel_view_type<0,rgba8_view_t>::type::iterator iterator_r;
    pair< iterator_r , iterator_r > result_r = minmax_element( kth_channel_view<0>(v).begin() , kth_channel_view<0>(v).end() , pixel_compare_less() );
    min_max_over_channels.push_back( *(result_r.first) );
    min_max_over_channels.push_back( *(result_r.second) );

    typedef kth_channel_view_type<1,rgba8_view_t>::type::iterator iterator_g;
    pair< iterator_r , iterator_g > result_g = minmax_element( kth_channel_view<1>(v).begin() , kth_channel_view<1>(v).end() , pixel_compare_less() );
    min_max_over_channels.push_back( *(result_g.first) );
    min_max_over_channels.push_back( *(result_g.second) );

    typedef kth_channel_view_type<2,rgba8_view_t>::type::iterator iterator_b;
    pair< iterator_b , iterator_b > result_b = minmax_element( kth_channel_view<2>(v).begin() , kth_channel_view<2>(v).end() , pixel_compare_less() );
    min_max_over_channels.push_back( *(result_b.first) );
    min_max_over_channels.push_back( *(result_b.second) );

    pair< vector<float>::iterator , vector<float>::iterator> min_and_max = minmax_element( min_max_over_channels.begin() , min_max_over_channels.end() );

    return std::make_pair( *(min_and_max.first) , *(min_and_max.second) );
}
