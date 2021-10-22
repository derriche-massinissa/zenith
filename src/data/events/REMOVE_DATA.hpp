/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_DATA_EVENTS_REMOVE_DATA_HPP
#define ZEN_DATA_EVENTS_REMOVE_DATA_HPP

#include <string>

namespace Zen {
namespace Events {

/**
 * The Remove Data Event.
 * 
 * This event is dispatched by a Data object when an item is removed from it.
 * 
 * To listen for
 * the removal of a data item on a Data object you would use:
 *
 * ```cpp
 * data.on('removedata', listener)
 * ```
 *
 * @since 0.0.0
 * 
 * @param key The unique key of the data item within the Data object.
 */
const std::string REMOVE_DATA = "removedata";

}	// namespace Events
}	// namespace Zen

#endif
