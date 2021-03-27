/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_COMPONENTS_TINT_HPP
#define ZEN_COMPONENTS_TINT_HPP

namespace Zen {
namespace Components {

struct Tint
{
	int tint = 0xffffff,
		tl = 0xffffff,
		tr = 0xffffff,
		bl = 0xffffff,
		br = 0xffffff;

	bool fill = false;
};

}	// namespace Components
}	// namespace Zen

#endif
