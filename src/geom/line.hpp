/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GEOM_LINE_HPP
#define ZEN_GEOM_LINE_HPP

#include <vector>
#include "types/line.hpp"
#include "../math/types/vector2.hpp"

namespace Zen {

/**
 * Get a point on a line that's a given percentage along its length.
 *
 * @since 0.0.0
 *
 * @param position_ A value between 0 and 1, where 0 is the start, 0.5 is the
 * middle and 1 is the end of the line.
 *
 * @return A Point, containing the coordinates of the point on the line.
 */
Math::Vector2 GetPoint (Line line, double position);

/**
 * Get a number of points along a line's length.
 *
 * Provide a `quantity` to get an exact number of points along the line.
 *
 * Provide a `stepRate` to ensure a specific distance between each point on the 
 * line. Set `quantity` to `0` when providing a `stepRate`.
 *
 * @since 0.0.0
 *
 * @param quantity_ The number of points to place on the line. Set to `0` to
 * use `stepRate` instead.
 * @param stepRate_ The distance between each point on the line. When set,
 * `quantity` is implied and should be set to `0`.
 *
 * @return A vector of Points, containing the coordinates of the points on
 * the line.
 */
std::vector<Math::Vector2> GetPoints (Line line, int quantity, double stepRate);

/**
 * Calculate the length of the given line.
 *
 * @since 0.0.0
 *
 * @return The length of the line.
 */
double GetLength (Line line);

/**
 * Get a random Point on the Line.
 *
 * @since 0.0.0
 *
 * @return A random Point on the Line.
 */
Math::Vector2 GetRandomPoint (Line line);

/**
 * Set new coordinates for the line endpoints.
 *
 * @since 0.0.0
 *
 * @param x1_ The x coordinate of the lines starting point.
 * @param y1_ The y coordinate of the lines starting point.
 * @param x2_ The x coordinate of the lines ending point.
 * @param y2_ The y coordinate of the lines ending point.
 *
 * @return This Line object.
 */
void SetTo (Line *line, double x1 = 0., double y1 = 0., double x2 = 0., double y2 = 0.);

/**
 * The left position of the Line.
 *
 * @since 0.0.0
 *
 * @return The left position of the Line.
 */
int GetLeft (Line line);

/**
 * The right position of the Line.
 *
 * @since 0.0.0
 *
 * @return The right position of the Line.
 */
int GetRight (Line line);

/**
 * The top position of the Line.
 *
 * @since 0.0.0
 *
 * @return The top position of the Line.
 */
int GetTop (Line line);

/**
 * The bottom position of the Line.
 *
 * @since 0.0.0
 *
 * @return The bottom position of the Line.
 */
int GetBottom (Line line);

/**
 * The left position of the Line.
 *
 * @since 0.0.0
 *
 * @param value The new value.
 *
 * @return This Line object.
 */
void SetLeft (Line *line, int value);

/**
 * The right position of the Line.
 *
 * @since 0.0.0
 *
 * @param value The new value.
 *
 * @return This Line object.
 */
void SetRight (Line *line, int value);

/**
 * The top position of the Line.
 *
 * @since 0.0.0
 *
 * @param value The new value.
 *
 * @return This Line object.
 */
void SetTop (Line *line, int value);

/**
 * The bottom position of the Line.
 *
 * @since 0.0.0
 *
 * @param value The new value.
 *
 * @return This Line object.
 */
void SetBottom (Line *line, int value);

}	// namespace Zen

#endif
