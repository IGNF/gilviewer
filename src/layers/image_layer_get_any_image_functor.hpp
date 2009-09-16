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

struct get_any_image_functor
{
	//typedef any_image< boost::mpl::vector<r"gb8_image_t> > result_type;
	typedef boost::shared_ptr< ImageLayer::usable_images_t > result_type;
	template <typename ViewT>
	result_type operator()(ViewT& src) const
	{
		gray8_image_t im( src.dimensions() );
		boost::gil::copy_pixels( src , view(im) );
		return result_type(new ImageLayer::usable_images_t(im));
	}
};

// TODO : use boost pp to generate all that ...

template <>
get_any_image_functor::result_type get_any_image_functor::operator()<gray8_view_t>( gray8_view_t& src ) const
{
	gray8_image_t im( src.dimensions() );
	boost::gil::copy_pixels( src , view(im) );
	return result_type(new ImageLayer::usable_images_t(im));
}

/*
template <>
get_any_image_functor::result_type get_any_image_functor::operator()<gray16_image_t>( gray16_image_t& src ) const
{
	gray16_image_t im( src.dimensions() );
	boost::gil::copy_pixels( src , view(im) );
	return result_type(new ImageLayer::usable_images_t(im));
}

template <>
get_any_image_functor::result_type get_any_image_functor::operator()<gray32_image_t>( gray32_image_t& src ) const
{
	gray32_image_t im( src.dimensions() );
	boost::gil::copy_pixels( src , view(im) );
	return result_type(new ImageLayer::usable_images_t(im));
}

template <>
get_any_image_functor::result_type get_any_image_functor::operator()<gray32F_image_t>( gray32F_image_t& src ) const
{
	gray32F_image_t im( src.dimensions() );
	boost::gil::copy_pixels( src , view(im) );
	return result_type(new ImageLayer::usable_images_t(im));
}

template <>
get_any_image_functor::result_type get_any_image_functor::operator()<gray64F_image_t>( gray64F_image_t& src ) const
{
	gray64F_image_t im( src.dimensions() );
	boost::gil::copy_pixels( src , view(im) );
	return result_type(new ImageLayer::usable_images_t(im));
}

template <>
get_any_image_functor::result_type get_any_image_functor::operator()<rgb8_image_t>( rgb8_image_t& src ) const
{
	rgb8_image_t im( src.dimensions() );
	boost::gil::copy_and_convert_pixels( src , view(im) );
	return result_type(new ImageLayer::usable_images_t(im));
}

template <>
get_any_image_functor::result_type get_any_image_functor::operator()<rgb16_image_t>( rgb16_image_t& src ) const
{
	rgb16_image_t im( src.dimensions() );
	boost::gil::copy_pixels( src , view(im) );
	return result_type(new ImageLayer::usable_images_t(im));
}

template <>
get_any_image_functor::result_type get_any_image_functor::operator()<rgb32_image_t>( rgb32_image_t& src ) const
{
	rgb32_image_t im( src.dimensions() );
	boost::gil::copy_pixels( src , view(im) );
	return result_type(new ImageLayer::usable_images_t(im));
}
*/
