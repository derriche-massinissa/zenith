/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_MASK_HPP
#define ZEN_SYSTEMS_MASK_HPP

#include "../ecs/entity.hpp"

namespace Zen {

Entity GetMask (Entity entity);

/**
 * Sets the mask that this entity will use to render with.
 *
 * The mask is another entity that have been previously created. It can either be
 * a textured entity for bitmap masks or a shape/geometry entity for geometry masks.
 *
 * If a mask is already set on this Game Object it will be immediately replaced.
 *
 * Masks are positioned in global space and are not relative to the entity to
 * which they are applied. The reason for this is that multiple Game Objects
 * can all share the same mask.
 *
 * Masks have no impact on physics or input detection. They are purely a
 * rendering component that allows you to limit what is visible during the
 * render pass.
 *
 * @since 0.0.0
 *
 * @param entity The entity to set the mask of.
 * @param mask The entity to use as a mask.
 */
void SetMask (Entity entity, Entity maskEntity, bool fixedPosition = false);

/**
 * Clears the mask that this entity was using.
 *
 * @since 0.0.0
 *
 * @param enity The entity to clear the mask from.
 */
void ClearMask (Entity entity);

void MakeMaskBitmap (Entity entity);

/////////
// Bitmap
/////////
Entity CreateMask ();

void ClearMask (Entity entity);

void DeleteMask (Entity entity);


///////////
// Geometry
///////////
void SetShape (Entity mask, Entity graphicsGeometry);
void SetInvertAlpha (Entity mask, bool value);
void PreRender (Entity mask, Entity child, Entity camera);
void ApplyStencil (Entity mask, Entity camera, bool inc);
void PostRender (Entity mask);

// TODO
void PreRenderMask (Entity mask, Entity camera);
void PostRenderMask (Entity mask, Entity camera);
bool IsMaskStencil (Entity mask);

}	// namespace Zen

#endif
