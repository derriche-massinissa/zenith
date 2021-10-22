/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_DATA_EVENTS_CHANGE_DATA_HPP
#define ZEN_DATA_EVENTS_CHANGE_DATA_HPP

#include <string>

namespace Zen {
namespace Events {

/**
 * The Change Data Event.
 * 
 * This event is dispatched by a Data object when an item in the data store is
 * changed.
 *
 * ```cpp
 * data.on("changedata", callback);
 * ```
 * 
 * This event is dispatched for all items that change in the Data object.
 * To listen for the change of a specific item, use the `CHANGE_DATA_KEY` event.
 *
 * @since 0.0.0
 * 
 * @param key The unique key of the data item within the Data object.
 * @param value The new value of the item in the Data object.
 * @param previousValue The previous value of the item in the Data object.
 */
const std::string CHANGE_DATA = "changedata";

}	// namespace Events
}	// namespace Zen

#endif
