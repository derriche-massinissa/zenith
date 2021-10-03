/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_TYPE_GL_PIPELINE_ATTRIBUTE_CONFIG_HPP
#define ZEN_RENDERER_TYPE_GL_PIPELINE_ATTRIBUTE_CONFIG_HPP

#include <string>
#include <GL/glew.h>

namespace Zen {

struct PipelineAttributeConfig {
	/**
	 * The name of the attribute as defined in the vertex shader.
	 *
	 * @since 0.0.0
	 */
	std::string name;

	/**
	 * The number of components in the attribute, i.e. 1 for a float, 2 for
	 * a vec2, 3 for a vec3, etc.
	 *
	 * @since 0.0.0
	 */
	int size = 1;

	/**
	 * The data type of the attribute. Can be one of the following:
	 * - GL_BYTE
	 * - GL_SHORT
	 * - GL_UNSIGNED_BYTE
	 * - GL_UNSIGNED_SHORT
	 * - GL_FLOAT
	 *
	 * @since 0.0.0
	 */
	GLenum type;

	/**
	 * Should the attribute data be normalized?
	 *
	 * @since 0.0.0
	 */
	bool normalized = false;
};

}	// namespace Zen

#endif
