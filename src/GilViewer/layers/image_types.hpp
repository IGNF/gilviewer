#ifndef __IMAGE_TYPES_HPP__
#define __IMAGE_TYPES_HPP__

#include <boost/mpl/vector.hpp>
#include <boost/mpl/copy.hpp>

#include "boost/gil/extension/matis/float_images.hpp"

/**
This header defines image types available in GilViewer. They are separated in several categories to be easily used in functors specialization.
*/

typedef boost::mpl::vector<
	boost::gil::gray8_image_t,
	boost::gil::gray16_image_t,
        boost::gil::gray16s_image_t,
        boost::gil::gray32_image_t,
	boost::gil::gray32F_image_t,
	boost::gil::gray64F_image_t
> gray_image_types;

typedef boost::mpl::vector< 
	boost::gil::rgb8_image_t,
	boost::gil::rgb16_image_t,
	boost::gil::rgb32_image_t
> rgb_image_types;

typedef boost::mpl::vector<
        boost::gil::rgba8_image_t,
        boost::gil::rgba16_image_t
> rgba_image_types;

typedef boost::mpl::vector<
        boost::gil::dev1n8_image_t,
        boost::gil::dev1n16_image_t,
        boost::gil::dev1n32F_image_t,
        boost::gil::dev3n8_image_t
> device_image_types;

// Concatenation of all available image types defined below
typedef boost::mpl::copy< boost::mpl::copy< boost::mpl::copy< gray_image_types,
                 boost::mpl::back_inserter< rgb_image_types  > >::type,
                 boost::mpl::back_inserter< rgba_image_types > >::type,
                 boost::mpl::back_inserter< device_image_types > >::type
	all_image_types;

#include <boost/gil/extension/dynamic_image/any_image.hpp>
typedef boost::gil::any_image<all_image_types> any_image_type;
typedef any_image_type::view_t any_view_type;

#include <boost/variant/variant.hpp>
typedef boost::variant< any_view_type
                      , boost::gil::dynamic_x_step_type<any_view_type>::type
                      , boost::gil::dynamic_y_step_type<any_view_type>::type
                      , boost::gil::dynamic_xy_step_type<any_view_type>::type
                      , boost::gil::dynamic_xy_step_transposed_type<any_view_type>::type
                      //, boost::gil::nth_channel_view_type<any_view_type>::type
                      > any_variant_view_type;

struct image_type {
  typedef any_image_type type;
  type value;
  image_type(const type& v) : value(v) {}
  image_type() {}
};

struct view_type {
  typedef any_view_type type;
  type value;
  view_type(const type& v) : value(v) {}
};

struct variant_view_type {
    typedef any_variant_view_type type;
    type value;
    variant_view_type(const type& v) : value(v) {}
};

#endif // __IMAGE_TYPES_HPP__
