/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

// https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c
// Manuel Martinez

#include "base64_decode.hpp"

#include <vector>

namespace Zen {

std::string Base64Decode (const std::string& in)
{
    std::string out;

    std::vector<int> T (256, -1);

    for (int i = 0; i < 64; i++)
	{
		T[
			"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
			[i]
		] = i;
	}

    int val=0, valb=-8;

    for (unsigned char c : in)
	{
        if (T[c] == -1)
			break;

        val = (val << 6) + T[c];
        valb += 6;

        if (valb >= 0)
		{
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }

    return out;
}

std::string Base64Decode (const std::string&& in)
{
	return Base64Decode(in);
}

}	// namespace Zen
