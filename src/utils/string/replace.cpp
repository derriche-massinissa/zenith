/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "replace.hpp"

namespace Zen {

std::string Replace (std::string s, std::string a, std::string b)
{
	size_t i = 0;

	while ((i = s.find(a, i)) != std::string::npos) {
		s.replace(i, a.length(), b);
		i += b.length();
	}

	return s;
}

}	// namespace Zen
