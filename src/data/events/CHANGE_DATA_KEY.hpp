/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_DATA_EVENTS_CHANGE_DATA_KEY_HPP
#define ZEN_DATA_EVENTS_CHANGE_DATA_KEY_HPP

#include <string>

namespace Zen {
namespace Events {

/**
 * The Change Data Key Event.
 * 
 * This event is dispatched by a Data object when an item in the data store is
 * changed.
 *
 * To listen for the change of a specific data item from a Data object you
 * would use: `data.on("changedata-key", callback)`, where `key` is the unique
 * string key of the data item. For example, if you have a data item stored
 * called `gold` the you can do as follow:
 *
 * ```cpp
 * data.on("changedata-gold");
 * data.set("gold", 7);
 * ```
 *
 * @since 0.0.0
 * 
 * @param value The new value of the item in the Data object.
 * @param previousValue The previous value of the item in the Data object.
 */
const std::string CHANGE_DATA_KEY = "changedata-";

}	// namespace Events
}	// namespace Zen

#endif
