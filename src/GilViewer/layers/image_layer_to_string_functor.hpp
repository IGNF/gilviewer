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

struct to_string_functor
{
	   template <typename ViewT>
	   void operator()(const ViewT& src, const int i, const int j, std::ostringstream& oss)  const
	   {
		   oss << src(i,j);
	   }
};

template <>
void to_string_functor::operator()(const rgb8c_view_t& src, const int i, const int j, std::ostringstream& oss)  const
{
	oss << (int) at_c<0>(src(i,j)) << "," << (int) at_c<1>(src(i,j)) << "," << (int) at_c<2>(src(i,j));
}

template <>
void to_string_functor::operator()(const rgb16c_view_t& src, const int i, const int j, std::ostringstream& oss)  const
{
	oss << (int) at_c<0>(src(i,j)) << "," << (int) at_c<1>(src(i,j)) << "," << (int) at_c<2>(src(i,j));
}


template <>
void to_string_functor::operator()(const rgba8c_view_t& src, const int i, const int j, std::ostringstream& oss)  const
{
	oss << (int) at_c<0>(src(i,j)) << "," << (int) at_c<1>(src(i,j)) << "," << (int) at_c<2>(src(i,j)) << "," << (int) at_c<3>(src(i,j));
}

template <>
void to_string_functor::operator()(const rgb32c_view_t& src, const int i, const int j, std::ostringstream& oss)  const
{
        oss << (int) at_c<0>(src(i,j)) << "," << (int) at_c<1>(src(i,j)) << "," << (int) at_c<2>(src(i,j));
}

template <>
void to_string_functor::operator()(const gray8c_view_t& src, const int i, const int j, std::ostringstream& oss)  const
{
	oss << (int) (src(i,j));
}

template <>
void to_string_functor::operator()(const gray8sc_view_t& src, const int i, const int j, std::ostringstream& oss)  const
{
	oss << (int) (src(i,j));
}

struct any_view_to_string {
    typedef void result_type;

    any_view_to_string(const int i, const int j, std::ostringstream& oss):
    	i_(i), j_(j), oss_(oss){}

    template <typename View> result_type operator()(View& vue)
    {
    	if(isInside(vue, i_,j_))
    		to_string_functor()(vue, i_, j_, oss_);
    	else
    		oss_ << "outside";
    }

    const int i_, j_;
    std::ostringstream& oss_;
};
