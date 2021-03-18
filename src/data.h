/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_DATA_H
#define ZEN_DATA_H

#include <vector>
#include <string>

namespace Zen {

struct Data
{
	std::vector<int> i;
	std::vector<std::string> s;
	std::vector<double> d;
};

}	// namespace Zen

#endif
