/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_AUDIO_TOOLS_ALC_HPP
#define ZEN_AUDIO_TOOLS_ALC_HPP

#include <iostream>
#include "AL/alc.h"

#define ZEN_ALC_CALL(function, device, ...) \
	alc_call_impl(__FILE__, __LINE__, function, device, __VA_ARGS__)

namespace Zen {

/**
 * @param filename The name of the file this function was invoked in.
 * @param line The line in which this function was invoked.
 * @param device The audio device used.
 *
 * @return `true` if no error occured, `false` otherwise.
 */
bool check_alc_errors(
		const std::string& filename,
		const std::uint_fast32_t line,
		ALCdevice* device);

/**
 * This overload is for OpenAL Context functions that do not return a value. The
 * result of `check_alc_errors` is returned instead.
 *
 * @tparam alFunction An OpenAL Context function.
 * @tparam Args Arguments to pass to the OpenAL Context function.
 *
 * @param filename The name of the file this function was invoked in.
 * @param line The line in which this function was invoked.
 * @param function An OpenAL Context function.
 * @param device The audio device to use.
 * @param args Arguments to pass to the OpenAL Context function.
 *
 * @return `true` if no error occured, `false` otherwise.
 */
template<typename alcFunction, typename... Args>
auto alc_call_impl(
		const char* filename,
		const std::uint_fast32_t line,
		alcFunction function,
		ALCdevice* device,
		Args... args)
	->typename std::enable_if_t<
		std::is_same<void, decltype(function(args...))>::value,
		bool
	>
{
	function(std::forward<Args>(args)...);
	return check_alc_errors(filename, line, device);
}

/**
 * This overload is for OpenAL Context functions that return a value.
 *
 * @tparam alFunction An OpenAL Context function.
 * @tparam ReturnType The return type of the OpenAL Context function.
 * @tparam Args Arguments to pass to the OpenAL Context function.
 *
 * @param filename The name of the file this function was invoked in.
 * @param line The line in which this function was invoked.
 * @param function An OpenAL Context function.
 * @param returnValue A reference to a variable to store `function` result in.
 * @param device The audio device to use.
 * @param args Arguments to pass to the OpenAL Context function.
 *
 * @return `true` if no error occured, `false` otherwise.
 */
template<typename alcFunction, typename ReturnType, typename... Args>
auto alc_call_impl(
		const char* filename,
		const std::uint_fast32_t line,
		alcFunction function,
		ReturnType& returnValue,
		ALCdevice* device,
		Args... args)
	->typename std::enable_if_t<
		!std::is_same<void, decltype(function(args...))>::value,
		bool
	>
{
	returnValue = function(std::forward<Args>(args)...);
	return check_alc_errors(filename, line, device);
}

}	// namespace Zen

#endif
