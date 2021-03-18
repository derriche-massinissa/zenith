/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "messages.h"

namespace Zen {

void messageError (std::string message, const std::string opt)
{
	std::cerr << "Error: " << message << opt << std::endl;
}

void messageWarning (std::string message, const std::string opt)
{
	std::cout << "Warning: " << message << opt << std::endl;
}

void messageNote (std::string message, const std::string opt)
{
	std::cout << "Note: " << message << opt << std::endl;
}

}	// namespace Zen
