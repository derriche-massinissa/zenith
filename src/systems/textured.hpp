/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_TEXTURED_HPP
#define ZEN_SYSTEMS_TEXTURED_HPP

#include <string>
#include "../ecs/entity.hpp"
#include "../geom/types/rectangle.hpp"
#include "../texture/crop_data.hpp"

namespace Zen {

void SetCrop (Entity entity, int x = -1, int y = -1, int width = -1, int height = -1);

void SetCrop (Entity entity, Rectangle rect);

CropData GetCrop (Entity entity);

void SetTexture (Entity entity, std::string key, std::string frame = "__BASE");

void SetFrame (Entity entity, std::string frameName, bool updateSize = true, bool updateOrigin = true);

Entity GetFrame (Entity entity);

void ResetCropObject (Entity entity);

bool IsCropped (Entity entity);

}	// namespace Zen

#endif
