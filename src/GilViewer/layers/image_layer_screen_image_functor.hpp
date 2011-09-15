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
#ifndef SCREEN_IMAGE_FUNCTOR
#define SCREEN_IMAGE_FUNCTOR

#include "image_layer_channel_converter_functor.hpp"
#include "image_layer_transparency_functor.hpp"
#include "layer_transform.hpp"

#include <boost/gil/extension/dynamic_image/dynamic_image_all.hpp>
#include <boost/gil/image_view_factory.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/variant/apply_visitor.hpp>

class screen_image_functor
{
public:
    typedef void result_type;
    screen_image_functor( boost::gil::dev3n8_view_t &screen_view,
                          channel_converter_functor cc,
                          const layer_transform& trans,
                          boost::gil::gray8_view_t& canal_alpha,
                          const double min_alpha,
                          const double max_alpha,
                          const unsigned char alpha,
                          bool isTransparent) :
            m_screen_view(screen_view), m_canal_alpha(canal_alpha), m_cc(cc),
            m_transform(trans),
            m_alpha(alpha),
            m_zero(0),
            m_transparencyFonctor(min_alpha, max_alpha),
            m_isTransparent(isTransparent)
    {
    }

    template <typename ViewType>
    result_type operator()( const ViewType& src ) const
    {
    switch(m_transform.orientation()){
    case layer_transform::LO_0: return apply_rotated(src);
    case layer_transform::LO_180: return apply_rotated(rotated180_view(src));
    case layer_transform::LO_90: return apply_rotated(rotated90cw_view(src));
    case layer_transform::LO_270: return apply_rotated(rotated90ccw_view(src));
    }
    }


    private:

    template <typename ViewType>
    result_type apply_rotated( const ViewType& src ) const
    {
        boost::gil::dev3n8_pixel_t blank;
        boost::gil::at_c<0>(blank) = 0;
        boost::gil::at_c<1>(blank) = 0;
        boost::gil::at_c<2>(blank) = 0;

        boost::gil::fill_pixels(m_screen_view, blank);

        //TODO to be optimized ?
        for (std::ptrdiff_t y=0; y < m_screen_view.height(); ++y)
        {
            int yb = (int) floor(y*m_transform.zoom_factor() - m_transform.translation_y());

            if (yb < 0 || yb >= src.height())
                continue;

            boost::gil::dev3n8_view_t::x_iterator screen_it = m_screen_view.row_begin(y);
            boost::gil::gray8_view_t::x_iterator alpha_it = m_canal_alpha.row_begin(y);
            typename ViewType::x_iterator src_it = src.row_begin(yb);

            for (std::ptrdiff_t x=0; x < m_screen_view.width(); ++x) //, ++loc.x())
            {
                int xb = (int) floor(x*m_transform.zoom_factor() - m_transform.translation_x());

                if (xb>=0 && xb < src.width())
                {
                    m_cc(src_it[xb], screen_it[x]);

                    if (m_isTransparent && m_transparencyFonctor(src_it[xb]))
                        alpha_it[x] = m_zero;
                    else
                        alpha_it[x] = m_alpha;
                }
            }
        }
    }

    boost::gil::dev3n8_view_t& m_screen_view;
    boost::gil::gray8_view_t& m_canal_alpha;
    channel_converter_functor m_cc;
    layer_transform m_transform;
    const boost::gil::gray8_pixel_t m_alpha, m_zero;
    transparency_functor m_transparencyFonctor;
    bool m_isTransparent;
};

#endif // SCREEN_IMAGE_FUNCTOR
