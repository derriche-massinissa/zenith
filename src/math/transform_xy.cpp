/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "transform_xy.hpp"

#include <cmath>

namespace Zen {
namespace Math {

Math::Vector2 TransformXY (double x, double y, double positionX, double positionY, double rotation, double scaleX, double scaleY)
{
    double radianSin = std::sin(rotation);
    double radianCos = std::cos(rotation);

    // Rotate and Scale
    double a = radianCos * scaleX;
    double b = radianSin * scaleX;
    double c = -radianSin * scaleY;
    double d = radianCos * scaleY;

    //  Invert
    double id = 1 / ((a * d) + (c * -b));

    double outX = (d * id * x) + (-c * id * y) + (((positionY * c) - (positionX * d)) * id);
    double outY = (a * id * y) + (-b * id * x) + (((-positionY * a) + (positionX * b)) * id);

    return {outX, outY};
}

}	// namespace Math
}	// namespace Zen
