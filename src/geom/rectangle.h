/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GEOM_RECTANGLE_H
#define ZEN_GEOM_RECTANGLE_H

#include <vector>
#include "point.h"
#include "../math/random_data_generator.h"

namespace Zen {
namespace Math { extern RandomDataGenerator random; }
namespace Geom {

class Rectangle
{
public:
	Rectangle (int x_ = 0, int y_ = 0, int width_ = 0, int height_ = 0);

	bool contains (int x_, int y_);

	Point getPoint (double position_);

	std::vector<Point> getPoints (int quantity_, double stepRate_);

	Point getRandomPoint ();

	Rectangle& setTo (int x_, int y_, int width_, int height_);

	Rectangle& setEmpty ();

	Rectangle& setPosition (int x_, int y_);

	Rectangle& setPosition (int x_);

	Rectangle& setSize (int width_, int height_);

	Rectangle& setSize (int width_);

	bool isEmpty ();

	Line getLineA ();

	Line getLineB ();

	Line getLineC ();

	Line getLineD ();

	int getLeft ();
	int left ();
	Rectangle& setLeft (int value_);
	Rectangle& left (int value_);

	int getRight ();
	int right ();
	Rectangle& setRight (int value_);
	Rectangle& right (int value_);

	int getTop ();
	int top ();
	Rectangle& setTop (int value_);
	Rectangle& top (int value_);

	int getBottom ();
	int bottom ();
	Rectangle& setBottom (int value_);
	Rectangle& bottom (int value_);

	int getCenterX ();
	int centerX ();
	Rectangle& setCenterX (int value_);
	Rectangle& centerX (int value_);

	int getCenterY ();
	int centerY ();
	Rectangle& setCenterY (int value_);
	Rectangle& centerY (int value_);

	int x = 0;

	int y = 0;

	int width = 0;

	int height = 0;
};

}	// namespace Geom
}	// namespace Zen

#endif
