/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MESSAGES_HPP
#define ZEN_MESSAGES_HPP

#include <iostream>

namespace Zen {

template <typename... Args>
void MessageError (Args... args)
{
	std::cout << "\033[1;31m";
	std::cout << "Error:";
	std::cout << "\033[0m";
	((std::cout << " ") << ... << args) << std::endl;
}

template <typename... Args>
void MessageWarning (Args... args)
{
	std::cout << "\033[1;33m";
	std::cout << "Warning:";
	std::cout << "\033[0m";
	((std::cout << " ") << ... << args) << std::endl;
}

template <typename... Args>
void MessageNote (Args... args)
{
	std::cout << "\033[0;32m";
	std::cout << "Note:";
	std::cout << "\033[0m";
	((std::cout << " ") << ... << args) << std::endl;
}

}	// namespace Zen

#endif
