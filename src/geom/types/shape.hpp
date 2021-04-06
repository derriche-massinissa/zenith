/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GEOM_TYPES_SHAPE_HPP
#define ZEN_GEOM_TYPES_SHAPE_HPP

#include "rectangle.hpp"
#include "line.hpp"
#include "circle.hpp"
#include "ellipse.hpp"
#include "triangle.hpp"
#include "polygon.hpp"

// TODO initialize like this:
// Shape myShape { .ellipse = {.x = 0} };
// `.shape.type` will be initialized by the activated structure thanks to the
// union's Common Initial Sequence

namespace Zen {

/**
 *
 * @struct Shape
 * @since 0.0.0
 */
union Shape
{
	struct {
		SHAPE type;
	} shape;

	Rectangle rectangle;

	Circle circle;

	Ellipse ellipse;

	Triangle triangle;

	Line line;

	Polygon polygon;

	Shape (SHAPE type = SHAPE::NONE)
	{
		switch (type)
		{
			case SHAPE::NONE:
				shape.type = SHAPE::NONE;
				break;

			case SHAPE::RECTANGLE:
				shape.type = SHAPE::RECTANGLE;
				rectangle.x = 0.;
				rectangle.y = 0.;
				rectangle.width = 0.;
				rectangle.height = 0.;
				break;

			case SHAPE::CIRCLE:
				shape.type = SHAPE::CIRCLE;
				circle.x = 0.;
				circle.y = 0.;
				circle.radius = 0.;
				break;

			case SHAPE::ELLIPSE:
				shape.type = SHAPE::ELLIPSE;
				ellipse.x = 0.;
				ellipse.y = 0.;
				ellipse.width = 0.;
				ellipse.height = 0.;
				break;

			case SHAPE::TRIANGLE:
				shape.type = SHAPE::TRIANGLE;
				triangle.x1 = 0.;
				triangle.y1 = 0.;
				triangle.x2 = 0.;
				triangle.y2 = 0.;
				triangle.x3 = 0.;
				triangle.y3 = 0.;
				break;

			case SHAPE::LINE:
				shape.type = SHAPE::LINE;
				line.x1 = 0.;
				line.y1 = 0.;
				line.x2 = 0.;
				line.y2 = 0.;
				break;

			case SHAPE::POLYGON:
				shape.type = SHAPE::POLYGON;
				polygon.size = 0;
				polygon.points = nullptr;
				break;
		}
	}

	Shape (const Shape& other)
		: shape (other.shape)
	{
		switch (other.shape.type)
		{
			case SHAPE::NONE:
				break;

			case SHAPE::RECTANGLE:
				rectangle = other.rectangle;
				break;

			case SHAPE::CIRCLE:
				circle = other.circle;
				break;

			case SHAPE::ELLIPSE:
				ellipse = other.ellipse;
				break;

			case SHAPE::TRIANGLE:
				triangle = other.triangle;
				break;

			case SHAPE::LINE:
				line = other.line;
				break;

			case SHAPE::POLYGON:
				polygon = std::move(other.polygon);
				break;
		}
	}

	Shape (Rectangle other)
	{
		shape.type = SHAPE::RECTANGLE;
		rectangle = other;
	}

	Shape (Circle other)
	{
		shape.type = SHAPE::CIRCLE;
		circle = other;
	}

	Shape (Ellipse other)
	{
		shape.type = SHAPE::ELLIPSE;
		ellipse = other;
	}

	Shape (Triangle other)
	{
		shape.type = SHAPE::TRIANGLE;
		triangle = other;
	}

	Shape (Line other)
	{
		shape.type = SHAPE::LINE;
		line = other;
	}

	Shape (Polygon&& other)
	{
		shape.type = SHAPE::POLYGON;
		polygon = std::move(other);
	}

	Shape (Shape&& other)
		: shape (other.shape)
	{
		if (other.shape.type == SHAPE::POLYGON)
		{
			polygon = std::move(other.polygon);
		}
		else
		{
			operator = (other);
		}
	}

	~Shape ()
	{
		if (shape.type == SHAPE::POLYGON)
			polygon.~Polygon();
	}

	void operator = (Shape& other)
	{
		switch (other.shape.type)
		{
			case SHAPE::NONE:
				break;

			case SHAPE::RECTANGLE:
				rectangle = other.rectangle;
				break;

			case SHAPE::CIRCLE:
				circle = other.circle;
				break;

			case SHAPE::ELLIPSE:
				ellipse = other.ellipse;
				break;

			case SHAPE::TRIANGLE:
				triangle = other.triangle;
				break;

			case SHAPE::LINE:
				line = other.line;
				break;

			case SHAPE::POLYGON:
				polygon = other.polygon;
				break;
		}
	}

	void operator = (Shape&& other)
	{
		if (other.shape.type == SHAPE::POLYGON)
		{
			shape.type = SHAPE::POLYGON;
			polygon = std::move(other.polygon);
		}
		else
		{
			operator = (other);
		}
	}
};

}	// namespace Zen

#endif
