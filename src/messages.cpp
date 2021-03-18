/**
 * @file		messages.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "messages.h"

void Zen::messageError (std::string message, const std::string& opt)
{
	std::cout << "Error: " << message << opt << std::endl;
}

void Zen::messageWarning (std::string message, const std::string& opt)
{
	std::cout << "Warning: " << message << opt << std::endl;
}

void Zen::messageNote (std::string message, const std::string& opt)
{
	std::cout << "Note: " << message << opt << std::endl;
}
