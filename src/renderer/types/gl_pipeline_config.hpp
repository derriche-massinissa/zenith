/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_TYPE_GL_PIPELINE_CONFIG_HPP
#define ZEN_RENDERER_TYPE_GL_PIPELINE_CONFIG_HPP

#include <string>
#include <vector>
#include <GL/glew.h>
#include "gl_pipeline_attribute_config.hpp"
#include "gl_pipeline_shader_config.hpp"
#include "render_target_config.hpp"

namespace Zen {

struct PipelineConfig {
	std::string name;

	GLenum topology = GL_TRIANGLES;

	std::string vertShader;

	std::string fragShader;

	int batchSize = 4096;

	/**
	 * The size, in bytes, of a single entry is the vertex buffer.
	 *
	 * @since 0.0.0
	 */
	int vertexSize = 1;

	/**
	 * @since 0.0.0
	 */
	std::vector<float> vertices;

	/**
	 * A vector of shader attribute data. All shaders bound to this pipeline must
	 * use the same attributes.
	 *
	 * @since 0.0.0
	 */
	std::vector<PipelineAttributeConfig> attributes;

	std::vector<PipelineShaderConfig> shaders;

	/**
	 * Force the shader to use just a single sampler2D?
	 *
	 * @since 0.0.0
	 */
	bool forceZero = false;

	std::vector<RenderTargetConfig> renderTarget;
};

}	// namespace Zen

#endif
