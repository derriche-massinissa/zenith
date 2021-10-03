/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_TYPE_GL_SHADER_CONFIG_HPP
#define ZEN_RENDERER_TYPE_GL_SHADER_CONFIG_HPP

#include <string>
#include <vector>
#include "gl_pipeline_attribute_config.hpp"

namespace Zen {

struct PipelineShaderConfig {
	/**
	 * The name of the shader.
	 *
	 * @since 0.0.0
	 */
	std::string name;

	/**
	 * The source code, as a string, for the vertex shader. If not given, uses the
	 * pipeline's property instead.
	 *
	 * @since 0.0.0
	 */
	std::string vertShader;

	/**
	 * The source code, as a string, for the fragment shader. If not given, uses the
	 * pipeline's property instead.
	 *
	 * @since 0.0.0
	 */
	std::string fragShader;


	/**
	 * A vector of shader attribute data. All shaders bound to this pipeline must
	 * use the same attributes.
	 *
	 * @since 0.0.0
	 */
	std::vector<PipelineAttributeConfig> attributes;
};

}	// namespace Zen

#endif
