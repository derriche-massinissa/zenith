/**
 * @file		zenith.hpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZENITH_HPP
#define ZENITH_HPP

// Cameras
#include "cameras/2d/systems/camera.hpp"

// Core
#include "core/config.hpp"

// Geometry
#include "geom/circle.hpp"
#include "geom/ellipse.hpp"
#include "geom/line.hpp"
#include "geom/rectangle.hpp"
#include "geom/shapes.hpp"
#include "geom/triangle.hpp"

// Math
#include "math/angle/between.hpp"
#include "math/angle/wrap_degrees.hpp"
#include "math/angle/wrap_radians.hpp"
#include "math/distance/distance_between.hpp"
#include "math/fuzzy/fuzzy_ceil.hpp"
#include "math/fuzzy/fuzzy_equal.hpp"
#include "math/fuzzy/fuzzy_floor.hpp"
#include "math/fuzzy/fuzzy_greater_than.hpp"
#include "math/fuzzy/fuzzy_less_than.hpp"
#include "math/interpolation/smooth_step_interpolation.hpp"
#include "math/snap/snap_ceil.hpp"
#include "math/snap/snap_floor.hpp"
#include "math/snap/snap_to.hpp"
#include "math/clamp.hpp"
#include "math/const.hpp"
#include "math/deg_to_rad.hpp"
#include "math/linear.hpp"
#include "math/rad_to_deg.hpp"
#include "math/random.hpp"
#include "math/random_data_generator.hpp"
#include "math/rotate_around.hpp"
#include "math/smooth_step.hpp"
#include "math/transform_xy.hpp"
#include "math/vector2.hpp"
#include "math/wrap.hpp"

// Scenes
#include "scene/scene.hpp"

// Systems
#include "systems/actor.hpp"
#include "systems/alpha.hpp"
#include "systems/background_color.hpp"
#include "systems/blend_mode.hpp"
#include "systems/bounds.hpp"
#include "systems/deadzone.hpp"
#include "systems/depth.hpp"
#include "systems/dirty.hpp"
#include "systems/flip.hpp"
#include "systems/follow.hpp"
#include "systems/get_bounds.hpp"
#include "systems/id.hpp"
#include "systems/input.hpp"
#include "systems/mask.hpp"
#include "systems/name.hpp"
#include "systems/origin.hpp"
#include "systems/position.hpp"
#include "systems/renderable.hpp"
#include "systems/rotation.hpp"
#include "systems/scale.hpp"
#include "systems/scroll.hpp"
#include "systems/scroll_factor.hpp"
#include "systems/size.hpp"
#include "systems/text.hpp"
#include "systems/textured.hpp"
#include "systems/tint.hpp"
#include "systems/transform.hpp"
#include "systems/transform_matrix.hpp"
#include "systems/transparent.hpp"
#include "systems/viewport.hpp"
#include "systems/visible.hpp"
#include "systems/zoom.hpp"
	

#endif
