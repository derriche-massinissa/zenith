/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_TYPE_RENDER_TARGET_CONFIG_HPP
#define ZEN_RENDERER_TYPE_RENDER_TARGET_CONFIG_HPP

namespace Zen {

struct RenderTargetConfig {
	double width = 0;
	double height = 0;
	double scale = 1;
	int minFilter = 0;
	bool autoClear = true;
	bool autoResize = false;
};

}	// namespace Zen

#endif
