/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_SCALE_HPP
#define ZEN_SYSTEMS_SCALE_HPP

#include "../ecs/entity.hpp"

namespace Zen {

/**
 * Sets the scale of the given entity.
 *
 * @since 0.0.0
 *
 * @param entity The entity to set the scale of.
 * @param value The new scale value.
 */
void SetScale (Entity entity, double value);

/**
 * Sets the scale of the given entity.
 *
 * @since 0.0.0
 *
 * @param entity The entity to set the scale of.
 * @param x The new x-scale value.
 * @param y The new y-scale value.
 */
void SetScale (Entity entity, double x, double y);

/**
 * Sets the x-scale of the given entity.
 *
 * @since 0.0.0
 *
 * @param entity The entity to set the scale of.
 * @param value The new scale value.
 */
void SetScaleX (Entity entity, double value);

/**
 * Sets the y-scale of the given entity.
 *
 * @since 0.0.0
 *
 * @param entity The entity to set the scale of.
 * @param value The new scale value.
 */
void SetScaleY (Entity entity, double value);

/**
 * Returns the mean between the x-scale and y-scale of the given entity.
 *
 * @since 0.0.0
 *
 * @param entity The entity to get the scale of.
 *
 * @returns The scale of the entity.
 */
double GetScale (Entity entity);

/**
 * Returns the scale of the x-axis of the given entity.
 *
 * @since 0.0.0
 *
 * @param entity The entity to get the scale of.
 *
 * @returns The x-scale of the entity.
 */
double GetScaleX (Entity entity);

/**
 * Returns the scale of the y-axis of the given entity.
 *
 * @since 0.0.0
 *
 * @param entity The entity to get the scale of.
 *
 * @returns The y-scale of the entity.
 */
double GetScaleY (Entity entity);

}	// namespace Zen

#endif
