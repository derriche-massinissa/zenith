/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_DATA_EVENTS_SET_DATA_HPP
#define ZEN_DATA_EVENTS_SET_DATA_HPP

#include <string>

namespace Zen {
namespace Events {

/**
 * The Set Data Event.
 * 
 * This event is dispatched by a Data object when a new item is added to the
 * data store.
 * 
 * To listen for
 * the addition of a new data item on a Data object you would use:
 *
 * ```cpp
 * data.on("setdata", callback)
 * ```
 *
 * @since 0.0.0
 * 
 * @param key The unique key of the data item within the Data object.
 * @param value The value of the new item in the Data object.
 */
const std::string SET_DATA = "setdata";

}	// namespace Events
}	// namespace Zen

#endif
