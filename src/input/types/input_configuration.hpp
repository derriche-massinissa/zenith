/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_INPUT_INPUTCONFIGURATION_HPP
#define ZEN_INPUT_INPUTCONFIGURATION_HPP

#include <string>
#include "../../geom/types/shape.hpp"
#include "hit_callback.hpp"

namespace Zen {

struct InputConfiguration
{
	Shape hitArea {};

	HitCallback hitAreaCallback = nullptr;

	bool draggable = false;

	bool dropZone = false;

	bool useHandCursor = false;

	std::string cursor = "";

	bool pixelPerfect = false;

	double alphaTolerance = 1.;
};

}	// namespace Zen

#endif
