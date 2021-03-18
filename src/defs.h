/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_DEFS_H
#define ZEN_DEFS_H

// OS: LINUX | WINDOWS
#define LINUX

/*
 * Component masks, used to know what component a given GameObject
 * inherited from (Like needing to know if the Object has a Size to
 * determine if it can be rendered).
 */
#define COMPONENT_MASK_ALPHA		0b0000000000000000000000000000001
#define COMPONENT_MASK_VISIBILE		0b0000000000000000000000000000010
#define COMPONENT_MASK_SIZE			0b0000000000000000000000000000100

#endif
