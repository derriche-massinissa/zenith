/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_ALPHA_HPP
#define ZEN_SYSTEMS_ALPHA_HPP

#include "../ecs/entity.hpp"

namespace Zen {

void ClearAlpha (Entity entity);

/**
 * Set the Alpha level of this GameObject. The alpha controls the opacity of the
 * GameObject as it renders.
 * Alpha values are provided as a double between 0, fully transparent, and 1, fully
 * opaque.
 *
 * You can optionally specify four different alpha values, each of which
 * correspond to the four corners of the GameObject.
 *
 * @since 0.0.0
 *
 * @param topLeft The alpha value used for the top-left of the GameObject. If this
 * is the only value given it's applied across the whole GameObject.
 * @param topRight The alpha value used for the top-right of the GameObject.
 * @param bottomLeft The alpha value used for the bottom-left of the GameObject.
 * @param bottomRight The alpha value used for the bottom-right of the GameObject.
 */
void SetAlpha (
		Entity entity,
		double topLeft,
		double topRight,
		double bottomLeft,
		double bottomRight
		);

/**
 * Returns the global alpha value of the given entity.
 *
 * @since 0.0.0
 *
 * @param entity The entity owning the AlphaComponent.
 *
 * @return Alpha value.
 */
double GetAlpha (Entity entity);

double GetAlphaTopLeft (Entity entity);

double GetAlphaTopRight (Entity entity);

double GetAlphaBottomLeft (Entity entity);

double GetAlphaBottomRight (Entity entity);

/**
 * Sets the global alpha value.
 *
 * @since 0.0.0
 */
void SetAlpha (Entity entity, double value = 1.0);

void SetAlphaTopLeft (Entity entity, double value);

void SetAlphaTopRight (Entity entity, double value);

void SetAlphaBottomLeft (Entity entity, double value);

void SetAlphaBottomRight (Entity entity, double value);

}	// namespace Zen

#endif
