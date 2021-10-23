/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "data.hpp"
#include <fstream>
#include <iomanip>
#include "../utils/base64/base64_encode.hpp"
#include "../utils/base64/base64_decode.hpp"
#include "../utils/file/file_to_string.hpp"

namespace Zen {

Data::Data (bool jsonMode)
	: jsonMode (jsonMode)
{}

void Data::remove (std::string key)
{
	if (jsonMode) {
		auto iterator = json.find(key);
		if (iterator != json.end()) {
			json.erase(iterator);
		}
	}
	else {
		auto iterator = data.find(key);
		if (iterator != data.end()) {
			data.erase(iterator);
		}
	}

	emit(Events::REMOVE_DATA, key);
}

void Data::toggle (std::string key)
{
	if (frozen)
		return;

	bool contains = (jsonMode) ? json.contains(key) : data.contains(key);

	if (contains)
		set(key, !get<bool>(key));
	else
		set(key, true);
}

void Data::merge (Data &rhs)
{
	if (jsonMode != rhs.jsonMode) {
		MessageWarning("Cannot merge Data objects of different modes");
		return;
	}

	if (jsonMode)
		json.merge_patch(rhs.json);
	else
		data.merge(rhs.data);
}

void Data::clear ()
{
	if (jsonMode)
		json.clear();
	else
		data.clear();

	frozen = false;
}

bool Data::has (std::string key)
{
	bool contains = (jsonMode) ? json.contains(key) : data.contains(key);

	return contains;
}

void Data::setFreeze (bool value)
{
	frozen = value;
}

size_t Data::size ()
{
	return (jsonMode) ? json.size() : data.size();
}

void Data::save (std::string filepath, int clarity)
{
	if (!jsonMode) {
		MessageWarning("Can only save Data object to file if in Json mode");
		return;
	}

	std::ofstream file (filepath);

	if (file.is_open()) {
		switch (clarity) {
			case 2:
				file << std::setw(4) << json << std::endl;
				break;
			case 1:
				file << json << std::endl;
				break;
			case 0:
				file << Base64Encode(json.dump()) << std::endl;
				break;
		}

		file.close();
	}
	else {
		MessageError("File couldn't be opened: ", filepath);
	}
}

void Data::load (std::string filepath, bool decode)
{
	if (!jsonMode) {
		MessageWarning("Can only load Data object from file if in Json mode");
		return;
	}

	if (decode) {
		json = nlohmann::json::parse(
			Base64Decode(
				FileToString(filepath)
			)
		);
	}
	else {
		std::ifstream file (filepath);
		if (file.is_open()) {
			file >> json;
			file.close();
		}
		else {
			MessageError("File couldn't be opened: ", filepath);
		}
	}

}

}	// namespace Zen
