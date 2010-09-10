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

#ifndef __HISTOGRAM_FUNCTOR__
#define __HISTOGRAM_FUNCTOR__

#include <boost/gil/pixel.hpp>

struct histogram_functor
{
    typedef std::vector< std::vector<double> > histogram_type;
    typedef boost::shared_ptr<const histogram_type> result_type;

    const static std::size_t histogram_size = 256;

    histogram_functor(const double mini, const double maxi):
        m_scale(255. / (maxi-mini)),
        m_offset(-mini * m_scale)
    {}

    template <typename ViewType>
    typename boost::enable_if_c<
      boost::gil::num_channels<typename ViewType::value_type>::value == 1,
      result_type >::type
    operator()(const ViewType& v) const
    {
        boost::shared_ptr<histogram_type> histo(new histogram_type(1, std::vector<double>(histogram_size)));
        typename ViewType::iterator it_begin = v.begin(), it_end = v.end();
        for (; it_begin!=it_end; ++it_begin)
            increment_histo<0, ViewType>(it_begin, *histo);
        return histo;
    }

    template <typename ViewType>
    typename boost::enable_if_c<
      boost::gil::num_channels<typename ViewType::value_type>::value >= 3,
      result_type >::type
    operator()(const ViewType& v) const
    {
        boost::shared_ptr<histogram_type> histo(new histogram_type(3, std::vector<double>(histogram_size)));

        typename ViewType::iterator it_begin = v.begin(), it_end = v.end();
        for (; it_begin!=it_end; ++it_begin)
        {
            increment_histo<0, ViewType>(it_begin, *histo);
            increment_histo<1, ViewType>(it_begin, *histo);
            increment_histo<2, ViewType>(it_begin, *histo);
        }
        return histo;
    }

private:
    template<size_t ChannelNb, typename ViewType>
    void increment_histo(typename ViewType::iterator it, histogram_type& histo) const
    {
        const size_t id = static_cast<size_t>(boost::gil::at_c<ChannelNb>(*it)*m_scale + m_offset);
        assert(id >=0);
        assert(id < histo[ChannelNb].size());
        assert(histo.size() == 3);
        ++histo[ChannelNb][id];
    }

    double m_scale;
    double m_offset;
};

#endif // __HISTOGRAM_FUNCTOR__
