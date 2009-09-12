/***********************************************************************

This file is part of the GilViewer project source files.

GilViewer is an open source 2D viewer (raster and vector) based on Boost
GIL and wxWidgets.


Homepage: 

	http://launchpad.net/gilviewer
	
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
	void operator()(const PixelT& src)
	{
		using namespace std;
		m_min = min(static_cast<float>(at_c<0>(src)), m_min);
		m_max = max(static_cast<float>(at_c<0>(src)), m_max);
	}

	float m_min, m_max;
};

template<>
void min_max_functor::operator()<rgb8_pixel_t>(const rgb8_pixel_t& src)
{
	using namespace std;
	m_min = min(static_cast<float>(at_c<0>(src)), m_min);
	m_max = max(static_cast<float>(at_c<0>(src)), m_max);
	m_min = min(static_cast<float>(at_c<1>(src)), m_min);
	m_max = max(static_cast<float>(at_c<1>(src)), m_max);
	m_min = min(static_cast<float>(at_c<2>(src)), m_min);
	m_max = max(static_cast<float>(at_c<2>(src)), m_max);
}

template<>
void min_max_functor::operator()<rgb16_pixel_t>(const rgb16_pixel_t& src)
{
	using namespace std;
	m_min = min(static_cast<float>(at_c<0>(src)), m_min);
	m_max = max(static_cast<float>(at_c<0>(src)), m_max);
	m_min = min(static_cast<float>(at_c<1>(src)), m_min);
	m_max = max(static_cast<float>(at_c<1>(src)), m_max);
	m_min = min(static_cast<float>(at_c<2>(src)), m_min);
	m_max = max(static_cast<float>(at_c<2>(src)), m_max);
}

struct any_view_min_max {
    typedef std::pair<float, float> result_type;
    template <typename View> result_type operator()(View& vue) const
    {
    	min_max_functor f = for_each_pixel(vue, min_max_functor());
    	return std::make_pair(f.m_min, f.m_max);
    }
};
