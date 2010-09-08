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

#ifndef SAMPLE_PLUGIN_FUNCTOR
#define SAMPLE_PLUGIN_FUNCTOR

#include <boost/gil/image_view_factory.hpp>
#include "../layers/image_layer.hpp"
#include "../layers/image_types.hpp"

struct sample_plugin_functor
{
    typedef void result_type;

    sample_plugin_functor(const image_layer::variant_view_t::type& dest) : m_dest(dest) {}

    template <typename ViewType>
    result_type operator()(const ViewType& v)
    {
        m_dest = boost::gil::rotated90cw_view(v);
    }

    image_layer::variant_view_t::type m_dest;
};

#endif // SAMPLE_PLUGIN_FUNCTOR
