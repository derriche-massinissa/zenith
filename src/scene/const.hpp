/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SCENE_CONST_HPP
#define ZEN_SCENE_CONST_HPP

namespace Zen {

enum class SCENE
{
	PENDING = 0,
	INIT,
	START,
	LOADING,
	CREATING,
	RUNNING,
	PAUSED,
	SLEEPING
};

enum class SCENE_OP
{
	NONE = 0,
	START,
	RUN,
	PAUSE,
	RESUME,
	SLEEP,
	WAKE,
	SWAP,
	REMOVE,
	BRING_TO_TOP,
	SEND_TO_BACK,
	MOVE_UP,
	MOVE_DOWN,
	MOVE_ABOVE,
	MOVE_BELOW,
	SWAP_POSITIONS
};

}	// namespace Zen

#endif
