/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_UTILS_ASSERT_HPP
#define ZEN_UTILS_ASSERT_HPP

#include <iostream>
#include <exception>

#ifndef NDEBUG
#define ZEN_ASSERT(condition, message) \
	do { \
		if (! (condition)) \
		{ \
			std::cerr << "Assertion " #condition " failed in " << __FILE__ \
					  << " line " << __LINE__ << "." << std::endl << message << std::endl; \
			std::terminate(); \
		} \
	} while (false)
#else
#define ZEN_ASSERT(condition, message) do {} while (false)
#endif

#endif
