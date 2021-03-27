/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../textured.hpp"

#include "../../utils/assert.hpp"
#include "../../texture/texture_manager.hpp"

// Components
#include "../../components/textured.hpp"
#include "../../components/flip.hpp"
#include "../../components/crop.hpp"
#include "../../components/renderable.hpp"
#include "../../components/size.hpp"
#include "../../components/origin.hpp"
#include "../../systems/size.hpp"
#include "../../systems/origin.hpp"

// Textures
#include "../../texture/components/frame.hpp"
#include "../../texture/systems/frame.hpp"
#include "../../texture/systems/texture.hpp"

#define FLAG 0b1000

namespace Zen {

extern entt::registry g_registry;
extern TextureManager g_textures;

void SetCrop (Entity entity, int x, int y, int width, int height)
{
	auto [textured, flip, crop] = g_registry.try_get<Components::Textured, Components::Flip, Components::Crop>(entity);
	ZEN_ASSERT(textured, "Entity has no 'Textured' component.");

	auto frame = g_registry.try_get<Components::Frame>(textured->frame);

	if (x < 0)
	{
		textured->isCropped = false;
	}
	else if (frame)
	{
		if (flip)
			SetFrameCropUVs(textured->frame, crop->data, x, y, width, height, flip->y, flip->y);
		else
			SetFrameCropUVs(textured->frame, crop->data, x, y, width, height, false, false);

		textured->isCropped = true;
	}
}

void SetCrop (Entity entity, Rectangle rect)
{
	SetCrop(entity, rect.x, rect.y, rect.width, rect.height);
}

void SetTexture (Entity entity, std::string key, std::string frame)
{
	auto textured = g_registry.try_get<Components::Textured>(entity);
	ZEN_ASSERT(textured, "The entity has no 'Textured' component.");

	textured->texture = g_textures.get(key);

	SetFrame(entity, frame);
}

void SetFrame (Entity entity, std::string frameName, bool updateSize, bool updateOrigin)
{
	auto [textured, renderable, size, origin, crop, flip] = g_registry.try_get<Components::Textured, Components::Renderable, Components::Size, Components::Origin, Components::Crop, Components::Flip>(entity);
	ZEN_ASSERT(textured && renderable, "The entity has no 'Textured' or 'Renderable' component.");

	textured->frame = GetFrame(textured->texture, frameName);

	auto frame = g_registry.try_get<Components::Frame>(textured->frame);
	ZEN_ASSERT(frame, "The entity has no 'Frame' component.");

	if (!frame->cutWidth || !frame->cutHeight)
		renderable->flags &= ~FLAG;
	else
		renderable->flags |= FLAG;

	if (size)
	{
		if (updateSize)
			SetSizeToFrame(entity);
	}

	if (origin)
	{
		if (updateOrigin)
		{
			if (frame->customPivot)
			{
				SetOrigin(entity, frame->pivotX, frame->pivotY);
			}
			else
			{
				UpdateDisplayOrigin(entity);
			}
		}
	}

	if (textured->isCropped)
		UpdateFrameCropUVs(textured->frame, crop->data, flip->x, flip->y);
}

void ResetCropObject (Entity entity)
{
	auto crop = g_registry.try_get<Components::Crop>(entity);
	ZEN_ASSERT(crop, "Entity has no 'Crop' component.");

	crop->data.u0 = 0.;
	crop->data.v0 = 0.;
	crop->data.u1 = 0.;
	crop->data.u1 = 0.;

	crop->data.x = 0;
	crop->data.y = 0;

	crop->data.cx = 0;
	crop->data.cy = 0;
	crop->data.cw = 0;
	crop->data.ch = 0;

	crop->data.width = 0;
	crop->data.height = 0;

	crop->data.flipX = false;
	crop->data.flipY = false;
}

}	// namespace Zen

#undef FLAG
