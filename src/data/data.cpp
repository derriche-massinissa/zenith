/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "data.hpp"
#include "../utils/map/contains.hpp"

namespace Zen {

void Data::remove (std::string key)
{
	auto iterator = data.find(key);

	if (iterator != data.end()) {
		data.erase(iterator);
	}

	emit(Events::REMOVE_DATA, key);
}

void Data::toggle (std::string key)
{
	if (frozen)
		return;

	if (data.contains(key))
		set(key, !get<bool>(key));
	else
		set(key, true);
}

void Data::merge (Data &rhs)
{
	data.merge(rhs.data);
}

void Data::clear ()
{
	data.clear();

	frozen = false;
}

bool Data::has (std::string key)
{
	return data.contains(key);
}

void Data::setFreeze (bool value)
{
	frozen = value;
}

size_t Data::size ()
{
	return data.size();
}

}	// namespace Zen
