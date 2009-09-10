/***********************************************************************

This file is part of the GilViewer project source files.

GilViewer is an open source 2D viewer (raster and vector) based on Boost
GIL and wxWidgets. GilViewer is distributed under the CeCILL-B licence. 
See Licence_CeCILL-B_V1-en.txt or http://www.cecill.info for more details.


Homepage: 

	http://launchpad.net/gilviewer
	
Copyright:
	
	Institut Geographique National (2009)

Authors: 

	Olivier Tournaire, Adrien Chauve

	
	

This software is governed by the CeCILL-B license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL-B
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL-B license and that you accept its terms.
 
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
