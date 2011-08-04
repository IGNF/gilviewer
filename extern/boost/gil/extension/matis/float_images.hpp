#ifndef GIL_FLOAT_IMAGES_HPP
#define GIL_FLOAT_IMAGES_HPP

#include <boost/gil/typedefs.hpp>

namespace boost { namespace gil {

//GIL_DEFINE_ALL_TYPEDEFS_INTERNAL(16, dev4n, devicen_t<4>, rgb_layout_t)
//GIL_DEFINE_ALL_TYPEDEFS_INTERNAL(16s, dev4n, devicen_t<4>, rgb_layout_t)

struct ir_t {};
typedef boost::mpl::vector4<red_t,green_t,blue_t,ir_t> rgbi_t;
typedef layout<rgbi_t> rgbi_layout_t;
GIL_DEFINE_ALL_TYPEDEFS_INTERNAL(16, dev4ni, devicen_t<4>, rgbi_layout_t)

typedef float bits32F;
GIL_DEFINE_BASE_TYPEDEFS(32F,gray)
GIL_DEFINE_ALL_TYPEDEFS_INTERNAL(32F, dev2n, devicen_t<2>, devicen_layout_t<2>)
GIL_DEFINE_ALL_TYPEDEFS_INTERNAL(32F, dev3n, devicen_t<3>, devicen_layout_t<3>)

#define GIL_SUPPORTS_32BITS_FLOAT_IMAGES 1

typedef double bits64F;
GIL_DEFINE_BASE_TYPEDEFS(64F,gray)
GIL_DEFINE_ALL_TYPEDEFS_INTERNAL(64F, dev2n, devicen_t<2>, devicen_layout_t<2>)
GIL_DEFINE_ALL_TYPEDEFS_INTERNAL(64F, dev3n, devicen_t<3>, devicen_layout_t<3>)

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
