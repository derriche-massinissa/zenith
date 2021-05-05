/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "easing.hpp"

#include "../math/easing/linear/linear.hpp"
#include "../math/easing/stepped/stepped.hpp"

#include "../math/easing/back/in.hpp"
#include "../math/easing/bounce/in.hpp"
#include "../math/easing/circular/in.hpp"
#include "../math/easing/cubic/in.hpp"
#include "../math/easing/elastic/in.hpp"
#include "../math/easing/expo/in.hpp"
#include "../math/easing/quadratic/in.hpp"
#include "../math/easing/quartic/in.hpp"
#include "../math/easing/quintic/in.hpp"
#include "../math/easing/sine/in.hpp"

#include "../math/easing/back/out.hpp"
#include "../math/easing/bounce/out.hpp"
#include "../math/easing/circular/out.hpp"
#include "../math/easing/cubic/out.hpp"
#include "../math/easing/elastic/out.hpp"
#include "../math/easing/expo/out.hpp"
#include "../math/easing/quadratic/out.hpp"
#include "../math/easing/quartic/out.hpp"
#include "../math/easing/quintic/out.hpp"
#include "../math/easing/sine/out.hpp"

#include "../math/easing/back/in_out.hpp"
#include "../math/easing/bounce/in_out.hpp"
#include "../math/easing/circular/in_out.hpp"
#include "../math/easing/cubic/in_out.hpp"
#include "../math/easing/elastic/in_out.hpp"
#include "../math/easing/expo/in_out.hpp"
#include "../math/easing/quadratic/in_out.hpp"
#include "../math/easing/quartic/in_out.hpp"
#include "../math/easing/quintic/in_out.hpp"
#include "../math/easing/sine/in_out.hpp"

namespace Zen {

std::function<double(double)> GetEaseFunction(TWEEN ease)
{
	std::function<double(double)> output = nullptr;

	switch (ease)
	{
		case TWEEN::POWER0:
			output = Math::Easing::Linear::Linear;
			break;

		case TWEEN::POWER1:
			output = Math::Easing::Quadratic::Out;
			break;

		case TWEEN::POWER2:
			output = Math::Easing::Cubic::Out;
			break;

		case TWEEN::POWER3:
			output = Math::Easing::Quartic::Out;
			break;

		case TWEEN::POWER4:
			output = Math::Easing::Quintic::Out;
			break;

		case TWEEN::LINEAR:
			output = Math::Easing::Linear::Linear;
			break;

		case TWEEN::QUAD:
			output = Math::Easing::Quadratic::Out;
			break;

		case TWEEN::CUBIC:
			output = Math::Easing::Cubic::Out;
			break;

		case TWEEN::QUART:
			output = Math::Easing::Quartic::Out;
			break;

		case TWEEN::QUINT:
			output = Math::Easing::Quintic::Out;
			break;

		case TWEEN::SINE:
			output = Math::Easing::Sine::Out;
			break;

		case TWEEN::EXPO:
			output = Math::Easing::Expo::Out;
			break;

		case TWEEN::CIRCULAR:
			output = Math::Easing::Circular::Out;
			break;

		case TWEEN::ELASTIC:
			output = [] (double value) {
				return Math::Easing::Elastic::Out(value);
			};
			break;

		case TWEEN::BACK:
			output = [] (double value) {
				return Math::Easing::Back::Out(value);
			};
			break;

		case TWEEN::BOUNCE:
			output = Math::Easing::Bounce::Out;
			break;

		case TWEEN::STEPPED:
			output = [] (double value) {
				return Math::Easing::Stepped::Stepped(value);
			};
			break;

		case TWEEN::QUAD_IN:
			output = Math::Easing::Quadratic::In;
			break;

		case TWEEN::CUBIC_IN:
			output = Math::Easing::Cubic::In;
			break;

		case TWEEN::QUART_IN:
			output = Math::Easing::Quartic::In;
			break;

		case TWEEN::QUINT_IN:
			output = Math::Easing::Quintic::In;
			break;

		case TWEEN::SINE_IN:
			output = Math::Easing::Sine::In;
			break;

		case TWEEN::EXPO_IN:
			output = Math::Easing::Expo::In;
			break;

		case TWEEN::CIRCULAR_IN:
			output = Math::Easing::Circular::In;
			break;

		case TWEEN::ELASTIC_IN:
			output = [] (double value) {
				return Math::Easing::Elastic::In(value);
			};
			break;

		case TWEEN::BACK_IN:
			output = [] (double value) {
				return Math::Easing::Back::In(value);
			};
			break;

		case TWEEN::BOUNCE_IN:
			output = Math::Easing::Bounce::In;
			break;

		case TWEEN::QUAD_OUT:
			output = Math::Easing::Quadratic::Out;
			break;

		case TWEEN::CUBIC_OUT:
			output = Math::Easing::Cubic::Out;
			break;

		case TWEEN::QUART_OUT:
			output = Math::Easing::Quartic::Out;
			break;

		case TWEEN::QUINT_OUT:
			output = Math::Easing::Quintic::Out;
			break;

		case TWEEN::SINE_OUT:
			output = Math::Easing::Sine::Out;
			break;

		case TWEEN::EXPO_OUT:
			output = Math::Easing::Expo::Out;
			break;

		case TWEEN::CIRCULAR_OUT:
			output = Math::Easing::Circular::Out;
			break;

		case TWEEN::ELASTIC_OUT:
			output = [] (double value) {
				return Math::Easing::Elastic::Out(value);
			};
			break;

		case TWEEN::BACK_OUT:
			output = [] (double value) {
				return Math::Easing::Back::Out(value);
			};
			break;

		case TWEEN::BOUNCE_OUT:
			output = Math::Easing::Bounce::Out;
			break;

		case TWEEN::QUAD_INOUT:
			output = Math::Easing::Quadratic::InOut;
			break;

		case TWEEN::CUBIC_INOUT:
			output = Math::Easing::Cubic::InOut;
			break;

		case TWEEN::QUART_INOUT:
			output = Math::Easing::Quartic::InOut;
			break;

		case TWEEN::QUINT_INOUT:
			output = Math::Easing::Quintic::InOut;
			break;

		case TWEEN::SINE_INOUT:
			output = Math::Easing::Sine::InOut;
			break;

		case TWEEN::EXPO_INOUT:
			output = Math::Easing::Expo::InOut;
			break;

		case TWEEN::CIRCULAR_INOUT:
			output = Math::Easing::Circular::InOut;
			break;

		case TWEEN::ELASTIC_INOUT:
			output = [] (double value) {
				return Math::Easing::Elastic::InOut(value);
			};
			break;

		case TWEEN::BACK_INOUT:
			output = [] (double value) {
				return Math::Easing::Back::InOut(value);
			};
			break;

		case TWEEN::BOUNCE_INOUT:
			output = Math::Easing::Bounce::InOut;
			break;

		default:
			break;
	}

	return output;
}

}	// namespace Zen
