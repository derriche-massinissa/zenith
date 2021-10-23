/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_UTILS_BASE64_ENCODE_HPP
#define ZEN_UTILS_BASE64_ENCODE_HPP

#include <string>

namespace Zen {

std::string Base64Encode (const std::string& in);

std::string Base64Encode (const std::string&& in);

}	// namespace Zen

#endif
