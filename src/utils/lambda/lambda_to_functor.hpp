/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_UTILS_LAMBDA_LAMBDA2FUNCTOR_HPP
#define ZEN_UTILS_LAMBDA_LAMBDA2FUNCTOR_HPP

#include <functional>

namespace Zen {

/**
 * Automatically defines the type of a functor from the lambda type only.
 *
 * Normally, it would be necessary to rewrite the parameter type in both the
 * lambda and the functor like the following:
 * ```cpp
 * std::function<int(int, int)> functor = [] (int a, int b) {
 *     return a + b;
 * }
 * ```
 *
 * This might be ok, but becomes impossible in a function that takes a lambda
 * as a parameter like so:
 * ```cpp
 * template <typename L>
 * void function (const L& lambda)
 * {
 *     std::function<???> functor = lambda;
 * }
 * ```
 *
 * Getting the parameter and return types in this case isn't possible. This is
 * where these type traits come into play, the previous problem can be solved
 * like this:
 * ```cpp
 * template <typename L>
 * void function (const L& lambda)
 * {
 *     typename lambda_to_functor<L>::type functor = lambda;
 * }
 * ```
 *
 * This will automatically extract the return and parameter types from the
 * lambda at compile time.
 *
 * It can also be used with a lambda instance, using the `decltype` keyword:
 * ```cpp
 * auto lambda = [] (int a, int b) {
 *     return a + b;
 * };
 *
 * typename lambda_to_functor<decltype(lambda)>::type functor = lambda;
 *
 * // Equivalent to:
 * std::function<int(int, int)> functor = lambda;
 * ```
 *
 * @since 0.0.0
 */

/**
 * Interface.
 *
 * @tparam L The lambda type to make a functor out of.
 *
 * @since 0.0.0
 */
template <typename L>
struct lambda_to_functor
	: lambda_to_functor<decltype(&L::operator())>
{};

/**
 * Mutable Specialization.
 *
 * @tparam T The return type of the lambda
 * @tparam C The type of the context of the lambda
 * @tparam Args The paramter types of the lambda function
 *
 * @since 0.0.0
 */
template <typename T, typename C, typename... Args>
struct lambda_to_functor<T (C::*)(Args...)>
{
	//typedef typename std::function<T(Args...)> type;
	using type = typename std::function<T(Args...)>;
};

/**
 * Constant Specialization.
 *
 * @tparam T The return type of the lambda
 * @tparam C The type of the context of the lambda
 * @tparam Args The paramter types of the lambda function
 *
 * @since 0.0.0
 */
template <typename T, typename C, typename... Args>
struct lambda_to_functor<T (C::*)(Args...) const>
{
	//typedef typename std::function<T(Args...)> type;
	using type = typename std::function<T(Args...)>;
};

}	// namespace Zen

#endif
