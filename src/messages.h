/**
 * @file		messages.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef MESSAGES_H
#define MESSAGES_H

#include <string>
#include <iostream>

namespace Zen {
	void messageError (std::string message, const std::string& opt = "");
	void messageWarning (std::string message, const std::string& opt = "");
	void messageNote (std::string message, const std::string& opt = "");
}

#endif
