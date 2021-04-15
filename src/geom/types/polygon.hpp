/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GEOM_TYPES_POLYGON_HPP
#define ZEN_GEOM_TYPES_POLYGON_HPP

#include "../shapes.hpp"
#include "../../math/types/vector2.hpp"
#include <algorithm>

namespace Zen {

/**
 * Defines a Polygon.
 *
 * @struct Polygon
 * @since 0.0.0
 */
struct Polygon
{
	/**
	 * Used for active type detection for the Shape union;
	 *
	 * @since 0.0.0
	 */
	const SHAPE type = SHAPE::POLYGON;

	unsigned int size = 0;

	Math::Vector2 *points = nullptr;

	// Move to polygon systems file
	Polygon (unsigned int size_ = 0)
	{
		size = size_;

		points = new Math::Vector2[size];
	}

	Polygon (const Polygon& other_)
	{
		size = other_.size;

		points = new Math::Vector2[size];

		for (unsigned int i = 0; i < other_.size; i++)
		{
			points[i] = other_.points[i];
		}
	}

	Polygon (Polygon&& other_)
		: size (other_.size), points (other_.points)
	{
		other_.size = 0;
		other_.points = nullptr;
	}

	~Polygon ()
	{
		if (points)
			delete[] points;
	}

	void operator = (const Polygon& other)
	{
		size = other.size;

		points = new Math::Vector2[size];

		for (unsigned int i = 0; i < other.size; i++)
		{
			points[i] = other.points[i];
		}
	}

	void operator = (Polygon&& other)
	{
		size = other.size;

		delete points;
		points = other.points;

		other.size = 0;
		other.points = nullptr;
	}
};

}	// namespace Zen

#endif
