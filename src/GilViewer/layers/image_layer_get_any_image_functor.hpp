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

#include "../layers/ImageLayer.hpp"

struct get_any_image_functor
{
    typedef boost::shared_ptr< ImageLayer::image_t > result_type;
    
    template <typename ViewT>
    result_type operator()(const ViewT& src) const
    {
    	boost::gil::image<typename ViewT::value_type,false,std::allocator<unsigned char> > dst( src.dimensions() );
    	boost::gil::copy_pixels( src , view(dst) );
    	return result_type(new ImageLayer::image_t(dst));
    }    
};
