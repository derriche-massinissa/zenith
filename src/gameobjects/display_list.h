/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECT_DISPLAYLIST_H
#define ZEN_GAMEOBJECT_DISPLAYLIST_H

#include <vector>

#include "gameobject.fwd.h"

namespace Zen {
namespace GameObjects {

class DisplayList
{
public:

	std::vector<GameObject*>& getChildren ()
	{
		return list;
	}

	std::vector<GameObject*> list;

	void depthSort ()
	{}

	void queueDepthSort ()
	{}
};

}	// namespace Input
}	// namespace Zen

#endif
