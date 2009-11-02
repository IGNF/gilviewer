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

struct channel_converter_functor
{
		float m_min_src, m_max_src, m_255_over_delta;
		boost::gil::dev3n8_pixel_t m_min_dst, m_max_dst;
		unsigned char m_atc0min, m_atc1min, m_atc2min;
		unsigned char m_atc0max, m_atc1max, m_atc2max;
		const unsigned char* m_lut;

        channel_converter_functor(const float min, const float max, const ColorLookupTable& lut):
            m_min_src(min),
            m_max_src(max),
            m_255_over_delta( 255 / (m_max_src - m_min_src) ),
			m_lut(&lut.getData().front())
        {
            boost::gil::at_c<0>(m_min_dst) = m_lut[0];
			boost::gil::at_c<1>(m_min_dst) = m_lut[256];
			boost::gil::at_c<2>(m_min_dst) = m_lut[512];
			boost::gil::at_c<0>(m_max_dst) = m_lut[255];
			boost::gil::at_c<1>(m_max_dst) = m_lut[511];
            boost::gil::at_c<2>(m_max_dst) = m_lut[767];

            m_atc0min = boost::gil::at_c<0>(m_min_dst);
            m_atc1min = boost::gil::at_c<1>(m_min_dst);
            m_atc2min = boost::gil::at_c<2>(m_min_dst);
            m_atc0max = boost::gil::at_c<0>(m_max_dst);
            m_atc1max = boost::gil::at_c<1>(m_max_dst);
            m_atc2max = boost::gil::at_c<2>(m_max_dst);
        }

		/// SHOULD NOT EXIST (to replace by boost::...
	   template <typename Ch1, typename Ch2>
	   void operator()(const Ch1& src, Ch2& dst)  const;

	   template <typename Ch1>
	   void operator()(const Ch1& src, boost::gil::dev3n8_pixel_t& dst)  const
	   {
		   if (src < m_min_src)
		   {
			   dst = m_min_dst;
                return;
			}
            if (src > m_max_src)
			{
			   dst = m_max_dst;
			   return;
			}
			unsigned char index = m_255_over_delta*(src - m_min_src);
			boost::gil::at_c<0>(dst) = m_lut[index];
			boost::gil::at_c<1>(dst) = m_lut[256+index];
			boost::gil::at_c<2>(dst) = m_lut[512+index];
	   }
};

#define OVERLOAD_CHANNEL_CONVERTER_PARENTHESIS_OPERATOR( r , n , data ) template <> \
void channel_converter_functor::operator()<data::value_type>(const data::value_type& v , boost::gil::dev3n8_pixel_t& dst) const \
{ \
	if (boost::gil::at_c<0>(v) < m_min_src) \
		boost::gil::at_c<0>(dst)  = m_atc0min; \
	else if (boost::gil::at_c<0>(v) > m_max_src) \
		boost::gil::at_c<0>(dst)  = m_atc0max; \
	else \
		boost::gil::at_c<0>(dst) = m_255_over_delta*(boost::gil::at_c<0>(v) - m_min_src); \
    \
	if (boost::gil::at_c<1>(v) < m_min_src) \
		boost::gil::at_c<1>(dst)  = m_atc1min; \
    else if (boost::gil::at_c<1>(v) > m_max_src) \
		boost::gil::at_c<1>(dst)  = m_atc1max; \
	else \
		boost::gil::at_c<1>(dst) = m_255_over_delta*(boost::gil::at_c<1>(v) - m_min_src); \
    \
	if (boost::gil::at_c<2>(v) < m_min_src) \
		boost::gil::at_c<2>(dst)  = m_atc2min; \
    else if (boost::gil::at_c<2>(v) > m_max_src) \
		boost::gil::at_c<2>(dst)  = m_atc2max; \
	else \
		boost::gil::at_c<2>(dst) = m_255_over_delta*(boost::gil::at_c<2>(v) - m_min_src); \
}

BOOST_PP_SEQ_FOR_EACH( OVERLOAD_CHANNEL_CONVERTER_PARENTHESIS_OPERATOR , ~ , RGB_IMAGE_TYPES )
BOOST_PP_SEQ_FOR_EACH( OVERLOAD_CHANNEL_CONVERTER_PARENTHESIS_OPERATOR , ~ , RGBA_IMAGE_TYPES )
