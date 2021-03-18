/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GEOM_POINT_H
#define ZEN_GEOM_POINT_H

namespace Zen {
namespace Geom {

/**
 * Defines a Point in 2D space, with an x and y component.
 *
 * @class Point
 * @since 0.0.0
 */
class Point
{
public:
	/**
	 * @since 0.0.0
	 *
     * @param x_ The x and y coordinates of this Point.
	 */
	Point (int x_ = 0);

	/**
	 * @since 0.0.0
	 *
     * @param x_ The x coordinate of this Point.
     * @param y_ The y coordinate of this Point.
	 */
	Point (int x_, int y_);

	/**
	 * The x coordinate of this Point.
	 *
	 * @since 0.0.0
	 */
	int x = 0;

	/**
	 * The y coordinate of this Point.
	 *
	 * @since 0.0.0
	 */
	int y = 0;

    /**
     * Set the x and y coordinates of the point to the given values.
     *
     * @since 0.0.0
     *
     * @param x_ The x and y coordinates of this Point.
     *
     * @return This Point object.
     */
	Point& setTo (int x_);

    /**
     * Set the x and y coordinates of the point to the given values.
     *
     * @since 0.0.0
     *
     * @param x_ The x coordinate of this Point.
     * @param y_ The y coordinate of this Point.
     *
     * @return This Point object.
     */
	Point& setTo (int x_, int y_);
};

}	// namespace Geom
}	// namespace Zen

#endif
