#ifndef GIL_FLOAT_IMAGES_HPP
#define GIL_FLOAT_IMAGES_HPP

#include <boost/gil/gil_all.hpp>

namespace boost { namespace gil {

typedef float  bits32F;
GIL_DEFINE_BASE_TYPEDEFS(32F,gray)

#define GIL_SUPPORTS_32BITS_FLOAT_IMAGES 1

typedef double  bits64F;
GIL_DEFINE_BASE_TYPEDEFS(64F,gray)

#define GIL_SUPPORTS_64BITS_FLOAT_IMAGES 1

struct dummy_color_converter
{
	template<class SrcType, class DstType>
	void operator()(const SrcType &src, DstType &dst) const
	{
		dst = src;
	}
};

}}//namespace boost::gil

#endif
