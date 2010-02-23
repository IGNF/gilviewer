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

#include <boost/gil/pixel.hpp>

struct channel_converter_functor
{
    typedef void result_type;

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

    template <typename ViewType>
    typename boost::enable_if_c<
      boost::gil::num_channels<typename ViewType::value_type>::value == 1,
      result_type >::type
    operator()(const ViewType& src, boost::gil::dev3n8_pixel_t& dst)  const
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
        unsigned char index = (unsigned char) (m_255_over_delta*(src - m_min_src));
        boost::gil::at_c<0>(dst) = m_lut[index];
        boost::gil::at_c<1>(dst) = m_lut[256+index];
        boost::gil::at_c<2>(dst) = m_lut[512+index];
    }

    template<class ViewType> 
    typename boost::enable_if_c<
      boost::gil::num_channels<typename ViewType::value_type>::value >= 3,
      result_type >::type
    operator()(const ViewType& src, boost::gil::dev3n8_pixel_t& dst)  const
    {
        using namespace boost::gil;

        if (at_c<0>(src) < m_min_src)
            at_c<0>(dst)  = m_atc0min;
        else if (at_c<0>(src) > m_max_src)
            at_c<0>(dst)  = m_atc0max;
        else
            at_c<0>(dst) = (unsigned char) (m_255_over_delta*(at_c<0>(src) - m_min_src));

        if (at_c<1>(src) < m_min_src)
            at_c<1>(dst)  = m_atc1min;
        else if (at_c<1>(src) > m_max_src)
            at_c<1>(dst)  = m_atc1max;
        else
            at_c<1>(dst) = (unsigned char) (m_255_over_delta*(at_c<1>(src) - m_min_src));

        if (at_c<2>(src) < m_min_src)
            at_c<2>(dst)  = m_atc2min;
        else if (at_c<2>(src) > m_max_src)
            at_c<2>(dst)  = m_atc2max;
        else
            at_c<2>(dst) = (unsigned char) (m_255_over_delta*(at_c<2>(src) - m_min_src));
    }

};
