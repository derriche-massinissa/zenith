/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_TYPE_GL_PIPELINE_ATTRIBUTE_HPP
#define ZEN_RENDERER_TYPE_GL_PIPELINE_ATTRIBUTE_HPP

#include <string>
#include <GL/glew.h>

namespace Zen {

struct PipelineAttribute {
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
	int size;

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
	 * The offset, in bytes, of this attribute data in the vertex array. Equivalent
	 * to `offsetof(vertex, attrib)`.
	 *
	 * @since 0.0.0
	 */
	GLsizei offset;

	/**
	 * Should the attribute data be normalized?
	 *
	 * @since 0.0.0
	 */
	GLboolean normalized = GL_FALSE;

	/**
	 * Should be `false` by default. The pipeline will enable it on boot.
	 *
	 * @since 0.0.0
	 */
	bool enabled = false;

	/**
	 * Should be `-1` by default. The pipeline will set it on boot.
	 *
	 * @since 0.0.0
	 */
	int location = -1;
};

}	// namespace Zen

#endif
