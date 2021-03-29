/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GEOM_RECTANGLE_HPP
#define ZEN_GEOM_RECTANGLE_HPP

#include <vector>
#include "types/line.hpp"
#include "types/rectangle.hpp"
#include "../math/types/vector2.hpp"

namespace Zen {

bool Contains (Rectangle rectangle, double x, double y);

bool Contains (Rectangle container, Rectangle item);

Math::Vector2 GetPoint (Rectangle rectangle, double position);

std::vector<Math::Vector2> GetPoints (Rectangle rectangle, int quantity, double stepRate);

Math::Vector2 GetRandomPoint (Rectangle rectangle);

double GetPerimeter(Rectangle rectangle);

void SetTo (Rectangle *rectangle, double x, double y, double width, double height);

void SetEmpty (Rectangle *rectangle);

void SetPosition (Rectangle *rectangle, double x, double y);

void SetPosition (Rectangle *rectangle, double x);

void SetSize (Rectangle *rectangle, double width, double height);

void SetSize (Rectangle *rectangle, double width);

/**
 * Moves the top-left corner of a Rectangle so that its center is at the given coordinates.
 *
 * @since 0.0.0
 *
 * @param x The X coordinate of the Rectangle's center.
 * @param y The Y coordinate of the Rectangle's center.
 *
 * @return This rectangle.
 */
void CenterOn (Rectangle *rectangle, double x, double y);
void CenterX (Rectangle *rectangle, double x);
void CenterY (Rectangle *rectangle, double y);

bool IsEmpty (Rectangle *rectangle);

Line GetLineA (Rectangle rectangle);

Line GetLineB (Rectangle rectangle);

Line GetLineC (Rectangle rectangle);

Line GetLineD (Rectangle rectangle);

double GetLeft (Rectangle rectangle);
void SetLeft (Rectangle *rectangle, double value);

double GetRight (Rectangle rectangle);
void SetRight (Rectangle *rectangle, double value);

double GetTop (Rectangle rectangle);
void SetTop (Rectangle *rectangle, double value);

double GetBottom (Rectangle rectangle);
void SetBottom (Rectangle *rectangle, double value);

double GetCenterX (Rectangle rectangle);
void SetCenterX (Rectangle *rectangle, double value);

double GetCenterY (Rectangle rectangle);
void SetCenterY (Rectangle *rectangle, double value);

}	// namespace Zen

#endif
