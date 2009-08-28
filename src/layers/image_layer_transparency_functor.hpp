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

struct transparency_functor
{
	transparency_functor(const double min_alpha, const double max_alpha):
		m_min_alpha(min_alpha),
		m_max_alpha(max_alpha)
		{}

	template<class PixelT>
	bool operator()(const PixelT& src) const
	{
		if(m_min_alpha<m_max_alpha)
			return m_min_alpha<=at_c<0>(src) && at_c<0>(src)<=m_max_alpha;
		else
			return m_min_alpha<=at_c<0>(src) || at_c<0>(src)<=m_max_alpha;
	}

	const double m_min_alpha, m_max_alpha;
};


template<>
bool transparency_functor::operator()<rgb8_pixel_t>(const rgb8_pixel_t& src) const
{
	if(m_min_alpha<m_max_alpha)
		return m_min_alpha<=at_c<0>(src) && at_c<0>(src)<=m_max_alpha
			&& m_min_alpha<=at_c<1>(src) && at_c<1>(src)<=m_max_alpha
			&& m_min_alpha<=at_c<2>(src) && at_c<2>(src)<=m_max_alpha;
	else
		return (m_min_alpha<=at_c<0>(src) || at_c<0>(src)<=m_max_alpha)
			&& (m_min_alpha<=at_c<1>(src) || at_c<1>(src)<=m_max_alpha)
			&& (m_min_alpha<=at_c<2>(src) || at_c<2>(src)<=m_max_alpha);
}

template<>
bool transparency_functor::operator()<rgb16_pixel_t>(const rgb16_pixel_t& src) const
{
	if(m_min_alpha<m_max_alpha)
		return m_min_alpha<=at_c<0>(src) && at_c<0>(src)<=m_max_alpha
			&& m_min_alpha<=at_c<1>(src) && at_c<1>(src)<=m_max_alpha
			&& m_min_alpha<=at_c<2>(src) && at_c<2>(src)<=m_max_alpha;
	else
		return (m_min_alpha<=at_c<0>(src) || at_c<0>(src)<=m_max_alpha)
			&& (m_min_alpha<=at_c<1>(src) || at_c<1>(src)<=m_max_alpha)
			&& (m_min_alpha<=at_c<2>(src) || at_c<2>(src)<=m_max_alpha);
}

struct apply_transparency_functor
{
	apply_transparency_functor(const double min_alpha, const double max_alpha, const unsigned char alpha):
		m_alpha(alpha),
		m_zero(0),
		m_transparencyFonctor(min_alpha, max_alpha)
		{}

	template<class PixelTSrc>
	gray8_pixel_t operator()(const PixelTSrc& src) const
	{
		if(m_transparencyFonctor(src))
			return m_zero;
		return m_alpha;
	}

	const gray8_pixel_t m_alpha, m_zero;
	transparency_functor m_transparencyFonctor;
};
