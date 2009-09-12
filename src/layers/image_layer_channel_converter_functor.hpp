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

///Didier
struct channel_converter_functor
{
		float m_min_src, m_max_src;
		dev3n8_pixel_t m_min_dst, m_max_dst;
		const unsigned char* m_lut;


			channel_converter_functor(const float min, const float max, const ColorLookupTable& lut):
			   m_min_src(min), m_max_src(max),
			   m_min_dst(dev3n8_pixel_t(0)),
			   m_max_dst(dev3n8_pixel_t(255)),
			   m_lut(&lut.getData().front())
		   {
//				boost::gil::at_c<3>(m_min_dst) = 255;
		   }


			/// SHOULD NOT EXIST (to replace by boost::...
		   template <typename Ch1, typename Ch2>
		   void operator()(const Ch1& src, Ch2& dst)  const;

		   template <typename Ch1>
		   void operator()(const Ch1& src, dev3n8_pixel_t& dst)  const
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
				   unsigned char index = (src - m_min_src)/(m_max_src - m_min_src)*255;
				   at_c<0>(dst) = m_lut[index];
				   at_c<1>(dst) = m_lut[256+index];
				   at_c<2>(dst) = m_lut[512+index];

		   }
};
///

template <>
void channel_converter_functor::operator()<rgb8_pixel_t>(const rgb8_pixel_t& src, dev3n8_pixel_t& dst) const
{
	if (at_c<0>(src) < m_min_src)
		at_c<0>(dst)  = at_c<0>(m_min_dst);
		else if (at_c<0>(src) > m_max_src)
			at_c<0>(dst)  = at_c<0>(m_max_dst);
			else
				at_c<0>(dst) = (at_c<0>(src) - m_min_src)/(m_max_src - m_min_src)*255;
	if (at_c<1>(src) < m_min_src)
		at_c<1>(dst)  = at_c<1>(m_min_dst);
		else if (at_c<1>(src) > m_max_src)
			at_c<1>(dst)  = at_c<1>(m_max_dst);
			else
				at_c<1>(dst) = (at_c<1>(src) - m_min_src)/(m_max_src - m_min_src)*255;
	if (at_c<2>(src) < m_min_src)
		at_c<2>(dst)  = at_c<2>(m_min_dst);
		else if (at_c<2>(src) > m_max_src)
			at_c<2>(dst)  = at_c<2>(m_max_dst);
			else
				at_c<2>(dst) = (at_c<2>(src) - m_min_src)/(m_max_src - m_min_src)*255;
}

template <>
void channel_converter_functor::operator()<rgb16_pixel_t>(const rgb16_pixel_t& src, dev3n8_pixel_t& dst) const
{
	if (at_c<0>(src) < m_min_src)
		at_c<0>(dst)  = at_c<0>(m_min_dst);
		else if (at_c<0>(src) > m_max_src)
			at_c<0>(dst)  = at_c<0>(m_max_dst);
			else
				at_c<0>(dst) = (at_c<0>(src) - m_min_src)/(m_max_src - m_min_src)*255;
	if (at_c<1>(src) < m_min_src)
		at_c<1>(dst)  = at_c<1>(m_min_dst);
		else if (at_c<1>(src) > m_max_src)
			at_c<1>(dst)  = at_c<1>(m_max_dst);
			else
				at_c<1>(dst) = (at_c<1>(src) - m_min_src)/(m_max_src - m_min_src)*255;
	if (at_c<2>(src) < m_min_src)
		at_c<2>(dst)  = at_c<2>(m_min_dst);
		else if (at_c<2>(src) > m_max_src)
			at_c<2>(dst)  = at_c<2>(m_max_dst);
			else
				at_c<2>(dst) = (at_c<2>(src) - m_min_src)/(m_max_src - m_min_src)*255;
}

template <>
void channel_converter_functor::operator()<rgb32_pixel_t>(const rgb32_pixel_t& src, dev3n8_pixel_t& dst) const
{
        if (at_c<0>(src) < m_min_src)
                at_c<0>(dst)  = at_c<0>(m_min_dst);
                else if (at_c<0>(src) > m_max_src)
                        at_c<0>(dst)  = at_c<0>(m_max_dst);
                        else
                                at_c<0>(dst) = (at_c<0>(src) - m_min_src)/(m_max_src - m_min_src)*255;
        if (at_c<1>(src) < m_min_src)
                at_c<1>(dst)  = at_c<1>(m_min_dst);
                else if (at_c<1>(src) > m_max_src)
                        at_c<1>(dst)  = at_c<1>(m_max_dst);
                        else
                                at_c<1>(dst) = (at_c<1>(src) - m_min_src)/(m_max_src - m_min_src)*255;
        if (at_c<2>(src) < m_min_src)
                at_c<2>(dst)  = at_c<2>(m_min_dst);
                else if (at_c<2>(src) > m_max_src)
                        at_c<2>(dst)  = at_c<2>(m_max_dst);
                        else
                                at_c<2>(dst) = (at_c<2>(src) - m_min_src)/(m_max_src - m_min_src)*255;
}
