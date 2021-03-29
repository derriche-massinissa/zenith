/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_STRUCTS_SIZE_HPP
#define ZEN_STRUCTS_SIZE_HPP

#include <cmath>
#include "types/size.hpp"

namespace Zen {

Size CreateSize (double width, double height, SCALE_MODE aspectMode = SCALE_MODE::RESIZE);

void SetAspectMode (Size *size, SCALE_MODE value = SCALE_MODE::RESIZE);

void SetSnap (Size *size, double snapWidth = 0., double snapHeight = 0.);

void SetMin (Size *size, double width = 0., double height = 0.);

void SetMax (Size *size, double width = 0., double height = 0.);

void SetSize (Size *size, double width = 0., double height = 0.);

void SetAspectRatio (Size *size, double ratio);

void Resize (Size *size, double width, double height);

double GetNewWidth (Size size, double value);

double GetNewHeight (Size size, double value);

void Constrain (Size *size, double width = 0., double height = 0., bool fit = true);

void FitTo (Size *size, double width, double height);

void Envelop (Size *size, double width, double height);

void SetWidth (Size *size, double width);

void SetHeight (Size *size, double height);

}	// namespace Zen

#endif
