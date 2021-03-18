/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GEOM_LINE_H
#define ZEN_GEOM_LINE_H

#include <vector>
#include <algorithm>

#include "../math/random_data_generator.h"
#include "../math/vector2.h"
#include "point.h"
 
namespace Zen {
namespace Math { extern RandomDataGenerator random; }
namespace Geom {

/**
 * Defines a Line segment, a part of a line between two endpoints.
 *
 * @class Line
 * @since 0.0.0
 *
 */
class Line
{
public:
	/**
	 * @since 0.0.0
	 *
	 * @param x1_ The x coordinate of the lines starting point.
	 * @param y1_ The y coordinate of the lines starting point.
	 * @param x2_ The x coordinate of the lines ending point.
	 * @param y2_ The y coordinate of the lines ending point.
	 */
	Line (int x1_ = 0, int y1_ = 0, int x2_ = 0, int y2_ = 0);

	/**
	 * The x coordinate of the lines starting point.
	 *
	 * @since 0.0.0
	 */
	int x1 = 0;

	/**
	 * The y coordinate of the lines starting point.
	 *
	 * @since 0.0.0
	 */
	int x2 = 0;

	/**
	 * The x coordinate of the lines ending point.
	 *
	 * @since 0.0.0
	 */
	int y1 = 0;

	/**
	 * The y coordinate of the lines ending point.
	 *
	 * @since 0.0.0
	 */
	int y2 = 0;

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
	Point getPoint (double position_);

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
	std::vector<Point> getPoints (int quantity_, double stepRate_);

	/**
	 * Calculate the length of the given line.
	 *
	 * @since 0.0.0
	 *
	 * @return The length of the line.
	 */
	double getLength ();

	/**
	 * Get a random Point on the Line.
	 *
	 * @since 0.0.0
	 *
	 * @return A random Point on the Line.
	 */
	Point getRandomPoint ();

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
	Line& setTo (int x1_ = 0, int y1_ = 0, int x2_ = 0, int y2_ = 0);

	/**
	 * Returns a Vector2 object that corresponds to the start of this Line.
	 *
	 * @since 0.0.0
	 *
	 * @return A Vector2 object that corresponds to the start of this Line.
	 */
	Math::Vector2 getPointA ();

	/**
	 * Returns a Vector2 object that corresponds to the end of this Line.
	 *
	 * @since 0.0.0
	 *
	 * @return A Vector2 object that corresponds to the end of this Line.
	 */
	Math::Vector2 getPointB ();

	// Getters

	/**
	 * The left position of the Line.
	 *
	 * @since 0.0.0
	 *
	 * @return The left position of the Line.
	 */
	int left ();

	/**
	 * The left position of the Line.
	 *
	 * @since 0.0.0
	 *
	 * @return The left position of the Line.
	 */
	int getLeft ();

	/**
	 * The right position of the Line.
	 *
	 * @since 0.0.0
	 *
	 * @return The right position of the Line.
	 */
	int right ();

	/**
	 * The right position of the Line.
	 *
	 * @since 0.0.0
	 *
	 * @return The right position of the Line.
	 */
	int getRight ();

	/**
	 * The top position of the Line.
	 *
	 * @since 0.0.0
	 *
	 * @return The top position of the Line.
	 */
	int top ();

	/**
	 * The top position of the Line.
	 *
	 * @since 0.0.0
	 *
	 * @return The top position of the Line.
	 */
	int getTop ();

	/**
	 * The bottom position of the Line.
	 *
	 * @since 0.0.0
	 *
	 * @return The bottom position of the Line.
	 */
	int bottom ();

	/**
	 * The bottom position of the Line.
	 *
	 * @since 0.0.0
	 *
	 * @return The bottom position of the Line.
	 */
	int getBottom ();

	// Setters

	/**
	 * The left position of the Line.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ The new value.
	 *
	 * @return This Line object.
	 */
	Line& left (int value_);

	/**
	 * The left position of the Line.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ The new value.
	 *
	 * @return This Line object.
	 */
	Line& setLeft (int value_);

	/**
	 * The right position of the Line.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ The new value.
	 *
	 * @return This Line object.
	 */
	Line& right (int value_);

	/**
	 * The right position of the Line.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ The new value.
	 *
	 * @return This Line object.
	 */
	Line& setRight (int value_);

	/**
	 * The top position of the Line.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ The new value.
	 *
	 * @return This Line object.
	 */
	Line& top (int value_);

	/**
	 * The top position of the Line.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ The new value.
	 *
	 * @return This Line object.
	 */
	Line& setTop (int value_);

	/**
	 * The bottom position of the Line.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ The new value.
	 *
	 * @return This Line object.
	 */
	Line& bottom (int value_);

	/**
	 * The bottom position of the Line.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ The new value.
	 *
	 * @return This Line object.
	 */
	Line& setBottom (int value_);
};

}	// namespace Geom
}	// namespace Zen

#endif
