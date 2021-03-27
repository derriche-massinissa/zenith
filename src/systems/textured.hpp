/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_TEXTURED_HPP
#define ZEN_SYSTEMS_TEXTURED_HPP

#include <string>
#include "../geom/types/rectangle.hpp"
#include "../ecs/entity.hpp"

namespace Zen {

void SetCrop (Entity entity, int x = -1, int y = -1, int width = -1, int height = -1);

void SetCrop (Entity entity, Rectangle rect);

void SetTexture (Entity entity, std::string key, std::string frame);

void SetFrame (Entity entity, std::string frameName, bool updateSize = true, bool updateOrigin = true);

void ResetCropObject (Entity entity);

}	// namespace Zen

#endif
