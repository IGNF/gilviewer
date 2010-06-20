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

#ifndef __LAYER_INFOS_FUNCTOR__
#define __LAYER_INFOS_FUNCTOR__

struct nb_components_functor
{
	typedef std::size_t result_type;
	template <typename ViewT>
	result_type operator()(const ViewT&) const
	{
        using namespace boost::gil;
	    return num_channels<ViewT>::value;
	}
};

struct custom_typename
{
	typedef std::string result_type;

	template <typename T>
	result_type operator()(const T&) { return "unknow";}
};
#include <boost/preprocessor/stringize.hpp>

#define MACRO_CUSTOM_TYPENAME(TYPE) \
template <> \
custom_typename::result_type custom_typename::operator()<TYPE>(const TYPE&)\
{\
	return BOOST_PP_STRINGIZE(TYPE);\
}

MACRO_CUSTOM_TYPENAME(unsigned char)
MACRO_CUSTOM_TYPENAME(unsigned short)
MACRO_CUSTOM_TYPENAME(signed short)
MACRO_CUSTOM_TYPENAME(unsigned int)
MACRO_CUSTOM_TYPENAME(float)
MACRO_CUSTOM_TYPENAME(double)

struct type_channel_functor
{
	typedef std::string result_type;
	template <typename ViewT>
	result_type operator()(const ViewT&) const
	{
        using namespace boost::gil;
		return custom_typename()( typename channel_type<ViewT>::type ());
	}
};

#endif // __LAYER_INFOS_FUNCTOR__
