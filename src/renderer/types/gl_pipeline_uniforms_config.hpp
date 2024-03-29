/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_TYPE_GL_PIPELINE_UNIFORM_CONFIG_HPP
#define ZEN_RENDERER_TYPE_GL_PIPELINE_UNIFORM_CONFIG_HPP

#include <string>
#include <vector>
#include <GL/glew.h>

namespace Zen {

struct PipelineUniformConfig {
	/**
	 * The name of the uniform as defined in the vertex shader.
	 *
	 * @since 0.0.0
	 */
	std::string name;

	/**
	 * The pipeline will set it when the program is linked.
	 *
	 * @since 0.0.0
	 */
	GLuint location;

	/**
	 * The cached value.
	 *
	 * @since 0.0.0
	 */
	std::vector<std::uint8_t> value;

	/**
	 * The number of elements in this uniform. If more than 1, this uniform
	 * is an array.
	 *
	 * @since 0.0.0
	 */
	int length;

	/**
	 * The OpenGL type of this uniform.
	 *
	 * @since 0.0.0
	 */
	GLenum type;

	/**
	 * The total size in bytes of this uniform.
	 *
	 * @since 0.0.0
	 */
	size_t size;
};

}	// namespace Zen

#endif
