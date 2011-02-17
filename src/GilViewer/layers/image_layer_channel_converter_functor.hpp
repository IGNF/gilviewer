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

#ifndef __CHANNEL_CONVERTER_FUNCTOR_HPP__
#define __CHANNEL_CONVERTER_FUNCTOR_HPP__

#include <boost/gil/pixel.hpp>

struct channel_converter_functor
{
    typedef void result_type;

    float m_min_src, m_max_src, m_255_over_delta, m_1_over_delta;
    boost::gil::dev3n8_pixel_t m_min_dst, m_max_dst;
    unsigned char m_atc0min, m_atc1min, m_atc2min;
    unsigned char m_atc0max, m_atc1max, m_atc2max;
    const boost::shared_array<float>& m_gamma_array;
    const unsigned char* m_lut;
    unsigned int m_red_index, m_green_index, m_blue_index;

    channel_converter_functor(const float min, const float max, const boost::shared_array<float>& gamma_array, const color_lookup_table& lut, unsigned int red_index=0, unsigned int green_index=1, unsigned int blue_index=2):
            m_min_src(min),
            m_max_src(max),
            m_255_over_delta( 255. / float(m_max_src - m_min_src) ),
            m_1_over_delta(1./float(m_max_src - m_min_src)),
            m_gamma_array(gamma_array),
            m_lut(&lut.get_data().front()),
            m_red_index(red_index),
            m_green_index(green_index),
            m_blue_index(blue_index)
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

    template <typename PixelType>
    typename boost::enable_if_c<
      boost::gil::num_channels<typename PixelType::value_type>::value == 1,
      result_type >::type
    operator()(const PixelType& src, boost::gil::dev3n8_pixel_t& dst)  const
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
        else
        {
            // BV: apply gamma BEFORE lut
            unsigned int index_gamma = 1000 * m_1_over_delta * (src - m_min_src); // TODO: 1000 = m_nbelt_tab_gamma !!!!
            unsigned char index = (unsigned char) (255 * m_gamma_array[index_gamma]);
			boost::gil::at_c<0>(dst) = m_lut[index];
            boost::gil::at_c<1>(dst) = m_lut[256+index];
            boost::gil::at_c<2>(dst) = m_lut[512+index];
        }
    }

    template<class PixelType>
    typename boost::enable_if_c<
      boost::gil::num_channels<typename PixelType::value_type>::value >= 3,
      result_type >::type
    operator()(const PixelType& src, boost::gil::dev3n8_pixel_t& dst)  const
    {
        using namespace boost::gil;
		// convert from [m_min_src, m_min_src+delta] to [0,1]
		int r = 1000 * m_1_over_delta * (src[m_red_index] - m_min_src);
		int g = 1000 * m_1_over_delta * (src[m_green_index] - m_min_src);
		int b = 1000 * m_1_over_delta * (src[m_blue_index] - m_min_src);

		// clamp
        if (r < 0) r = 0;
        else if (r > 1000) r = 1000;
		if (g < 0) g = 0;
        else if (g > 1000) g = 1000;
		if (b < 0) b = 0;
        else if (b > 1000) b = 1000;

		// switch back to dest type
		boost::gil::at_c<0>(dst) = (unsigned char)(255*m_gamma_array[r]);
		boost::gil::at_c<1>(dst) = (unsigned char)(255*m_gamma_array[g]);
		boost::gil::at_c<2>(dst) = (unsigned char)(255*m_gamma_array[b]);
    }
};

#endif // __CHANNEL_CONVERTER_FUNCTOR_HPP__
