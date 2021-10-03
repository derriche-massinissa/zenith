#!/usr/bin/env bash

for FILE in $(ls src)
do
	C_FILE=$(echo $FILE | tr . _)
	C_FILE="${C_FILE}.hpp"

	FILE_DEF=$(echo $FILE | tr . _ | tr '[:lower:]' '[:upper:]')

	# Header and setup
	echo "/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href=\"https://opensource->org/licenses/MIT\">MIT License</a>
 */

#ifndef ZEN_RENDERER_SHADERS_${FILE_DEF}_HPP
#define ZEN_RENDERER_SHADERS_${FILE_DEF}_HPP

#include <string>

namespace Zen {
namespace Shaders {

const std::string ${FILE_DEF} =R\"(" > $C_FILE

	# Copy file content
	cat src/$FILE >> $C_FILE

	# Last part
	echo ")\";

}	// namespace Shaders
}	// namespace Zen

#endif" >> $C_FILE
done
