/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_DATA_HPP
#define ZEN_DATA_HPP

#include <map>
#include <any>
#include <string>
#include <vector>
#include <functional>
#include "events/events.hpp"
#include "../event/event_emitter.hpp"
#include "json/json.hpp"

namespace Zen {

/**
 * The Data class is a multitype data container. Entries are composed of pairs of
 * string keys and arbitrary data (std::any).
 *
 * This is used extensively for sending data during scene operations.
 *
 * @class Data
 * @since 0.0.0
 */
class Data : public EventEmitter
{
private:
	/**
	 * The data map.
	 *
	 * @since 0.0.0
	 */
	std::map<std::string, std::any> data;

	/**
	 * The json data, used if this Data object is in json mode.
	 *
	 * @since 0.0.0
	 */
	nlohmann::json json;

	/**
	 * Is this Data object in json mode? Defaults to false.
	 *
	 * Json mode:
	 * - Only bool, numbers and text can be stored in this data object.
	 * - Nested lists and maps of above types can also be stored in this Data
	 *   object.
	 * - Custom types can be stored if serialization functions are provided.
	 * - This data object can be serialized and saved into a file.
	 *
	 * Non json mode:
	 * - Faster.
	 * - Anything can be stored in this data object: Basic, custom data, pointers...
	 * - It is not possible to save this data object into a file.
	 *
	 * @since 0.0.0
	 */
	bool jsonMode;

	/**
	 * Whether editing this data object is permitted.
	 *
	 * @since 0.0.0
	 */
	bool frozen = false;

public:
	/**
	 * @since 0.0.0
	 */
	Data (bool jsonMode = false);

	/**
	 * Retrieves the value for the given key. Note that the key needs to exist!
	 *
	 * ```cpp
	 * int lvl = data.get<int>("level");
	 * ```
	 *
	 * @since 0.0.0
	 *
	 * @tparam T The type of the data to retrieve.
	 * @param key The key of the value to retrieve.
	 *
	 * @return The value belonging to the given key.
	 */
	template <typename T>
	T get (std::string key)
	{

		if (jsonMode) {
			if constexpr (std::is_class_v<T>) {
				MessageError("Cannot use complex types in Json mode");
				return {};
			}
			else {
				return json[key].get<T>();
			}
		}
		else {
			return std::any_cast<T>(data[key]);
		}
	}

	/**
	 * Retrieves the values for the given keys. Note that the keys needs to exist!
	 * Used to retrieve the values of many keys of different types.
	 *
	 * ```cpp
	 * auto [lvl, gold, name] =
	 *     data.get<int, double, const char*>("level", "gold", "name");
	 * ```
	 *
	 * @since 0.0.0
	 *
	 * @tparam Ta The type of the first data piece to retrieve.
	 * @tparam Tb The type of the second data piece to retrieve.
	 * @tparam Ts The types of the data pack to retrieve.
	 * @tparam Keys Pack of std::string.
	 * @param keyA The key of the first value to retrieve.
	 * @param keyB The key of the second value to retrieve.
	 * @param keys The keys of the value pack to retrieve.
	 *
	 * @return A tuple of values belonging to the given keys.
	 */
	template <typename Ta, typename Tb, typename ... Ts, typename ... Keys>
	std::tuple<Ta, Tb, Ts...> get
		(std::string keyA, std::string keyB, Keys ... keys)
	{
		return std::tuple<Ta, Tb, Ts...> (get<Ta>(keyA), get<Tb>(keyB), get<Ts>(keys)...);
	}

	/**
	 * Retrieves the values for the given keys. Note that the keys needs to exist!
	 * Used to retrieve the values of many keys of the same type.
	 *
	 * ```cpp
	 * std::vector<int> info = data.get<int>("level", "gold", "power");
	 * ```
	 *
	 * @since 0.0.0
	 *
	 * @tparam T The type of the data to retrieve.
	 * @tparam Keys Pack of std::string.
	 * @param keyA The key of the first value to retrieve.
	 * @param keyB The key of the second value to retrieve.
	 * @param keys The keys of the value pack to retrieve.
	 *
	 * @return A vector of values belonging to the given keys.
	 */
	template <typename T, typename ... Keys>
	std::vector<T> get (std::string keyA, std::string keyB, Keys ... keys)
	{
		return std::vector<T> {get<T>(keyA), get<T>(keyB), get<T>(keys)...};
	}

	/**
	 * Sets a value for the given key. If the key doesn't already exist then it
	 * is created.
	 *
	 * ```cpp
	 * data.set("name", "Flame Crystal");
	 * ```
	 *
	 * @since 0.0.0
	 */
	template <typename T>
	void set (std::string key, T value)
	{
		if (frozen) return;

		T previousValue;
		bool change = data.contains(key);

		if (change) {
			previousValue = get<T>(key);
		}

		if (jsonMode) {
			if constexpr (std::is_class_v<T>) {
				MessageError("Cannot use complex types in Json mode");
				return;
			}
			else {
				json[key] = value;
			}
		}
		else
			data[key] = value;

		if (!change) {
			emit(Events::SET_DATA, key, value);
		}
		else {
			emit(Events::CHANGE_DATA, key, value, previousValue);
			emit(Events::CHANGE_DATA_KEY + key, value, previousValue);
		}
	}

	/**
	 * Increase the value of the given key. If the key doesn't exist then it is
	 * increased from 0.
	 *
	 * @since 0.0.0
	 */
	template <typename T>
	void inc (std::string key, T value = 1)
	{
		if (frozen)
			return;

		if (has(key))
			set(key, get<T>(key) + value);
		else
			set(key, value);
	}

	/**
	 * Toggle a boolean value for the given key. If the key doesn't already exist
	 * then it is toggled from false.
	 *
	 * @since 0.0.0
	 */
	void toggle (std::string key);

	/**
	 * Merge this Data object with the given Data object. This will move every non
	 * duplicate element from the source data to this one, modifying both objects.
	 *
	 * @since 0.0.0
	 *
	 * @param rhs The data object to merge with this one.
	 */
	void merge (Data &rhs);

	/**
	 * Remove the value for the given key.
	 *
	 * @since 0.0.0
	 */
	void remove (std::string key);

	/**
     * Determines whether the given key is set in this Data object.
     *
	 * Please note that the keys are case-sensitive.  This means the keys
	 * `gold` and `Gold` are treated as two unique values within the Data
	 * object.
	 *
	 * @since 0.0.0
	 *
	 * @param key the key to ckeck.
	 *
	 * @return `true` if the key exists, otherwise `false`.
	 */
	bool has (std::string key);

	/**
	 * Freeze or unfreeze this Data Manager. A frozen Data object will block
	 * all attempts to create new values or update existing ones.
	 *
	 * @since 0.0.0
	 */
	void setFreeze (bool value = true);

	/**
	 * Returns the number of element in this Data object.
	 *
	 * @since 0.0.0
	 */
	size_t size ();

	/**
     * Delete all data in this Data object and unfreeze it.
	 *
	 * @since 0.0.0
	 */
	void clear ();

	/**
	 * Save this Data object in as a Json file, if in json mode.
	 *
	 * @since 0.0.0
	 *
	 * @param filepath The file to save this Data object in.
	 * @param pretty The clarity level when saving:
	 * - 2: Prettified json
	 * - 1: Packed json
	 * - 0: Encoded to Base64
	 */
	void save (std::string filepath, int clarity = 2);

	/**
	 * Load a Json file and store its data to this Data object if in Json mode.
	 *
	 * @since 0.0.0
	 *
	 * @param filepath The file to load data from.
	 * @param decode Should the content be first decoded fromm Base64?
	 */
	void load (std::string filepath, bool decode = false);
};

}	// namespace Zen

#endif
