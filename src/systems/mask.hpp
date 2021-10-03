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

void SetMask (Entity entity, Entity maskEntity, bool fixedPosition = false);

void ClearMask (Entity entity);

/////////
// Bitmap
/////////
Entity CreateMask ();
void ClearMask (Entity entity);
void SetBitmap (Entity renderable);


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
