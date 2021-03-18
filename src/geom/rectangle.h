/**
 * @file		rectangle.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef RECTANGLE_H
#define RECTANGLE_H

namespace Zen
{
	namespace Geom
	{
		class Rectangle
		{
		public:
			Rectangle (int _x = 0, int _y = 0, int _width = 0, int _height = 0);
			~Rectangle();

			static bool contains (int rectX, int rectY, int rectW, int rectH, int x, int y);

			int x = 0;

			int y = 0;

			int width = 0;

			int height = 0;
		};
	}
}

#endif
