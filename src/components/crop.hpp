/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_COMPONENTS_CROP_HPP
#define ZEN_COMPONENTS_CROP_HPP

#include "../texture/crop_data.hpp"

namespace Zen {
namespace Components {

struct Crop
{
	bool isCropped = false;

	CropData data;
};

}	// namespace Components
}	// namespace Zen

#endif
