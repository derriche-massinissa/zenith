/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_AUDIO_TOOLS_AL_HPP
#define ZEN_AUDIO_TOOLS_AL_HPP

#include <iostream>
#include "AL/al.h"

#define ZEN_AL_CALL(function, ...) al_call_impl(__FILE__, __LINE__, function, __VA_ARGS__)

namespace Zen {

/**
 * @param filename The name of the file this function was invoked in.
 * @param line The line in which this function was invoked.
 *
 * @return `true` if no error occured, `false` otherwise.
 */
bool check_al_errors (const std::string& filename, const std::uint_fast32_t line);

/**
 * This overload is for OpenAL functions that return a value.
 *
 * @tparam alFunction An OpenAL function.
 * @tparam Args Arguments to pass to the OpenAL function.
 *
 * @param filename The name of the file this function was invoked in.
 * @param line The line in which this function was invoked.
 * @param function An OpenAL function.
 * @param args Arguments to pass to the OpenAL function.
 *
 * @return Whatever the passed in OpenAL function returned.
 */
template<typename alFunction, typename... Args>
auto al_call_impl(
		const char* filename,
		const std::uint_fast32_t line,
		alFunction function,
		Args... args)
	->typename std::enable_if_t< // Set if `function` return type is not void
		!std::is_same<void, decltype(function(args...))>::value,
		decltype(function(args...))
	>
{
	auto ret = function(std::forward<Args>(args)...);
	check_al_errors(filename, line);
	return ret;
}

/**
 * This overload is for OpenAL functions that do not return a value. The result of
 * `check_al_errors` is returned instead.
 *
 * @tparam alFunction An OpenAL function.
 * @tparam Args Arguments to pass to the OpenAL function.
 *
 * @param filename The name of the file this function was invoked in.
 * @param line The line in which this function was invoked.
 * @param function An OpenAL function.
 * @param args Arguments to pass to the OpenAL function.
 *
 * @return `true` if no error occured, `false` otherwise.
 */
template<typename alFunction, typename... Args>
auto al_call_impl(
		const char* filename,
		const std::uint_fast32_t line,
		alFunction function,
		Args... args)
	->typename std::enable_if_t< // Set if `function` return type is void
		std::is_same<void, decltype(function(args...))>::value,
		bool
	>
{
	function(std::forward<Args>(args)...);
	return check_al_errors(filename, line);
}

}	// namespace Zen

#endif
