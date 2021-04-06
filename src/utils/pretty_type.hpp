/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_UTILS_PRETTYTYPE_HPP
#define ZEN_UTILS_PRETTYTYPE_HPP

#ifndef NDEBUG
#include <cxxabi.h>
#include <string>

template <typename T>
std::string PRETTY_TYPE ()
{
	int status = 0;

	std::string typeName = typeid(T).name();

	char* demangledName = abi::__cxa_demangle(typeName.c_str(), nullptr, nullptr, &status);

	if (status == 0)
	{
		typeName = demangledName;
		std::free(demangledName);
	}

	return typeName;
}
#endif

#endif
