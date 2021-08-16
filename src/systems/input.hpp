/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_INPUT_HPP
#define ZEN_SYSTEMS_INPUT_HPP

#include "../ecs/entity.hpp"
#include "../components/input.hpp"

namespace Zen {

/**
 * Indicate if the passed entity is input enabled or no.
 *
 * @since 0.0.0
 *
 * @param entity The entity to inspect.
 *
 * @return `true` if the entity is input enabled. Otherwise `false`.
 */
bool IsInputEnabled (Entity entity);

/**
 * Make this entity a drop zone for drag and drop events.
 *
 * @since 0.0.0
 *
 * @param entity The entity to inspect.
 */
void MakeDropZone (Entity entity, bool value = true);

/**
 * @since 0.0.0
 *
 * @param entity The entity to inspect.
 *
 * @return A pointer to the input component of the entity.
 */
Components::Input* GetInput (Entity entity);

/**
 * Set the cursor of this entity, to display when hovered.
 *
 * @since 0.0.0
 *
 * @param entity The entity to inspect.
 */
void SetCursor (Entity entity, std::string cursor);

/**
 * Resets the cursor of this entity.
 *
 * @since 0.0.0
 *
 * @param entity The entity to inspect.
 */
void ResetCursor (Entity entity);

/**
 * Returns the currently active cursor of this entity.
 *
 * @since 0.0.0
 *
 * @param entity The entity to inspect.
 *
 * @return Cursor of the entity.
 */
std::string GetCursor (Entity entity);

}	// namespace Zen

#endif
