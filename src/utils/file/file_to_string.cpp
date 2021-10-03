/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "file_to_string.hpp"
#include <fstream>
#include <sstream>
#include "../messages.hpp"

namespace Zen {

std::string FileToString (std::string path)
{
	// Retrieve
	std::string text;
	std::ifstream file;

	// Ensure ifstream objects can throw exceptions
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// Open file
		file.open(path);
		std::stringstream stream;

		// Read file's buffer content into streams
		stream << file.rdbuf();

		// Close file handler
		file.close();

		// Convert stream to string
		text = stream.str();
	}
	catch (std::ifstream::failure& e) {
		MessageError("File couldn't be read!");
	}

	return text;
}

}	// namespace Zen
