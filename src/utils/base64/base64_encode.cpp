/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

// https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c
// Manuel Martinez

#include "base64_encode.hpp"

namespace Zen {

std::string Base64Encode (const std::string& in)
{
	std::string out;

	int val = 0, valb = -6;

	for (unsigned char c : in)
	{
		val = (val << 8) + c;
		valb += 8;
		while (valb >= 0)
		{
			out.push_back(
				"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
				[(val >> valb) & 0x3F]
				);
			valb -= 6;
		}
	}

	if (valb>-6)
		out.push_back(
				"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" 
				[((val << 8) >> (valb + 8)) & 0x3F]
				);

	while (out.size() % 4)
		out.push_back('=');

	return out;
}

std::string Base64Encode (const std::string&& in)
{
	return Base64Encode(in);
}

}	// namespace Zen
