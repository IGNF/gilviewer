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

#ifndef TRANSPARENCY_FUNCTOR
#define TRANSPARENCY_FUNCTOR

#include <boost/gil/pixel.hpp>

struct transparency_functor
{
    typedef bool result_type;

    transparency_functor(const double min_alpha, const double max_alpha):
            m_min_alpha(min_alpha),
            m_max_alpha(max_alpha)
    {}

    template <typename ViewType>
    typename boost::enable_if_c<
      boost::gil::num_channels<typename ViewType::value_type>::value == 1,
      result_type >::type
    operator()(const ViewType & src) const
    {
        if (m_min_alpha <= m_max_alpha)
            return m_min_alpha <= src && src <= m_max_alpha;
        else
            return m_min_alpha <= src || src <= m_max_alpha;
    }

    template<class ViewType> 
    typename boost::enable_if_c<
      boost::gil::num_channels<typename ViewType::value_type>::value >= 3,
      result_type >::type
    operator()(const ViewType &src) const
    {
        using namespace boost::gil;

        if (m_min_alpha <= m_max_alpha)
            return m_min_alpha <= boost::gil::at_c<0>(src) && boost::gil::at_c<0>(src) <= m_max_alpha
                   && m_min_alpha <= boost::gil::at_c<1>(src) && boost::gil::at_c<1>(src) <= m_max_alpha
                   && m_min_alpha <= boost::gil::at_c<2>(src) && boost::gil::at_c<2>(src) <= m_max_alpha;
        else
            return (m_min_alpha <= boost::gil::at_c<0>(src) || boost::gil::at_c<0>(src) <= m_max_alpha)
                   && (m_min_alpha <= boost::gil::at_c<1>(src) || boost::gil::at_c<1>(src) <= m_max_alpha)
                   && (m_min_alpha <= boost::gil::at_c<2>(src) || boost::gil::at_c<2>(src) <= m_max_alpha);
    }

    const double m_min_alpha, m_max_alpha;
};

#endif // TRANSPARENCY_FUNCTOR
