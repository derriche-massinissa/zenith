/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "triangle.hpp"

namespace Zen {

bool Contains (Triangle triangle, double x, double y)
{
    double v0x = triangle.x3 - triangle.x1;
    double v0y = triangle.y3 - triangle.y1;

    double v1x = triangle.x2 - triangle.x1;
    double v1y = triangle.y2 - triangle.y1;

    double v2x = x - triangle.x1;
    double v2y = y - triangle.y1;

    double dot00 = (v0x * v0x) + (v0y * v0y);
    double dot01 = (v0x * v1x) + (v0y * v1y);
    double dot02 = (v0x * v2x) + (v0y * v2y);
    double dot11 = (v1x * v1x) + (v1y * v1y);
    double dot12 = (v1x * v2x) + (v1y * v2y);

    // Compute barycentric coordinates
    double b = ((dot00 * dot11) - (dot01 * dot01));
    double inv = (b == 0) ? 0. : (1. / b);
    double u = ((dot11 * dot02) - (dot01 * dot12)) * inv;
    double v = ((dot00 * dot12) - (dot01 * dot02)) * inv;

    return (u >= 0 && v >= 0 && (u + v < 1));
}

bool TriangleContains (Shape shape, double x, double y)
{
	return Contains(shape.triangle, x, y);
}

}	// namespace Zen
