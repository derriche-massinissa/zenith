/**
 * @file		scale_manager.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "scale_manager.h"

void Zen::ScaleManager::preBoot ()
{

}

void Zen::ScaleManager::boot ()
{
	if (scaleMode != ScaleMode::RESIZE)
		displaySize.setAspectMode(scaleMode);

	if (scaleMode == ScaleMode::NONE) {
		resize(width, height);
	}
}
