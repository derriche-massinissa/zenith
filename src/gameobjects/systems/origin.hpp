/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_SYSTEMS_ORIGIN_HPP
#define ZEN_GAMEOBJECTS_SYSTEMS_ORIGIN_HPP

namespace Zen {

void SetDisplayOriginX (Entity entity, int value);

void SetDisplayOriginY (Entity entity, int value);

void SetDisplayOrigin (Entity entity, int x, int y);

void SetDisplayOrigin (Entity entity, int value = 0);

void SetOrigin (Entity entity, double x, double y);

void SetOrigin (Entity entity, double value = 0.5);

void SetOriginFromFrame (Entity entity);

double GetOriginX (Entity entity);

double GetOriginY (Entity entity);

double GetDisplayOriginX (Entity entity);

double GetDisplayOriginY (Entity entity);

}	// namespace Zen

#endif
