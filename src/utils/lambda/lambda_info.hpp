/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_UTILS_LAMBDA_INFO_HPP
#define ZEN_UTILS_LAMBDA_INFO_HPP

#include <tuple>

namespace Zen {

/**
 * Source: angeart - https://qiita.com/angeart/items/94734d68999eca575881
 *
 * This type trait allows one to get the following information about a given
 * lambda function:
 * - Return type
 * - Arity (Number of paramters)
 * - Type of each parameter
 * - Type of context/instance (In case of member function)
 *
 * It can be used as follow:
 * ```cpp
 * auto lambda = [] () { return 5; };
 *
 * std::cout << "Return type: " << std::is_same<lambda_info<decltype(lambda)>::type, int>::value << std::endl;
 *
 * std::cout << "Args size: " << lambda_info<decltype(lambda)>::arity << std::endl;
 *
 * std::cout << "Argument 0 Type: " << std::is_same<lambda_info<decltype(lambda)>::arg<0>::type, int>::value << std::endl;
 * ```
 *
 * @since 0.0.0
 */

/**
 * @tparam T The return type of the lambda
 * @tparam C -
 * @tparam Args The paramter types of the lambda function
 *
 * @since 0.0.0
 */
template <typename T, typename C, typename... Args>
struct lambda_details
{
	using type = T;

	enum { arity = sizeof...(Args) };

	template<unsigned int i>
	struct arg
	{
		typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
	};
};

/**
 * Interface.
 *
 * @tparam L The lambda type to make a functor out of.
 *
 * @since 0.0.0
 */
template <typename L>
struct lambda_info
	: lambda_info<decltype(&L::operator())>
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
struct lambda_info<T (C::*)(Args...)>
	: lambda_details<T, C, Args...>
{};


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
struct lambda_info<T (C::*)(Args...) const>
	: lambda_details<T, C, Args...>
{};

}	// namespace Zen

#endif
