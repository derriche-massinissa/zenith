/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_GL_SHADER_HPP
#define ZEN_RENDERER_GL_SHADER_HPP

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include "types/gl_pipeline_attribute.hpp"
#include "types/gl_pipeline_attribute_config.hpp"
#include "types/gl_pipeline_uniforms_config.hpp"
#include "types/gl_types.hpp"
#include "../event/event_emitter.hpp"

namespace Zen {

class Shader : public EventEmitter {
public:
	Shader (std::string name, std::string vertexShader,
		std::string fragmentShader,
		std::vector<PipelineAttributeConfig> attributes);

	void createProgram (std::string vertexShader, std::string fragmentShader);

	void checkCompileErrors (GLuint shader, std::string type);
	
	void createAttributes (std::vector<PipelineAttributeConfig>);

	void bind (bool setAttributes = false, bool flush = false);

	Shader* rebind ();

	void setAttribPointers (bool reset = false);

	void createUniforms ();

	bool hasUniform (std::string name);

	void resetUniform (std::string name);

	// Utillity uniform functions
	// Booleans ------------------------------------------------------------------
	void set (const std::string &name, bool value);
	// Scalars -------------------------------------------------------------------
	void set (const std::string &name, int value);
	void set (const std::string &name, unsigned int value);
	void set (const std::string &name, float value);
	// Vector 2 ------------------------------------------------------------------
	void set (const std::string &name, const glm::vec2 &vec);
	void set (const std::string &name, float x, float y);
	void set (const std::string &name, const glm::vec<2, int> &vec);
	void set (const std::string &name, int x, int y);
	// Vector 3 ------------------------------------------------------------------
	void set (const std::string &name, const glm::vec3 &vec);
	void set (const std::string &name, float x, float y, float z);
	void set (const std::string &name, const glm::vec<3, int> &vec);
	void set (const std::string &name, int x, int y, int z);
	// Vector 4 ------------------------------------------------------------------
	void set (const std::string &name, const glm::vec4 &vec);
	void set (const std::string &name, float x, float y, float z, float w);
	void set (const std::string &name, const glm::vec<4, int> &vec);
	void set (const std::string &name, int x, int y, int z, int w);
	// Matrices ------------------------------------------------------------------
	void set (const std::string &name, bool transpose, const glm::mat2 &mat);
	void set (const std::string &name, bool transpose, const glm::mat3 &mat);
	void set (const std::string &name, bool transpose, const glm::mat4 &mat);
	// Vector ---------------------------------------------------------------------
	template <typename T>
	void set (const std::string &name, const std::vector<T>& data)
	{
		auto *uniform = prepareUniform(name, data.size(), data.data());

		if (uniform) {
			if constexpr (std::is_same_v<T, float>)
				glUniform1fv(uniform->location, data.size(), data.data());
			else if constexpr (std::is_same_v<T, int>)
				glUniform1iv(uniform->location, data.size(), data.data());
			else if constexpr (std::is_same_v<T, unsigned int>)
				glUniform1uiv(uniform->location, data.size(), data.data());
		}
	}
	// Array ---------------------------------------------------------------------
	template <typename T, int N>
	void set (const std::string &name, const std::array<T, N>& data)
	{
		auto *uniform = prepareUniform(name, N, data.data());

		if (uniform) {
			if constexpr (std::is_same_v<T, float>)
				glUniform1fv(uniform->location, N, data.data());
			else if constexpr (std::is_same_v<T, int>)
				glUniform1iv(uniform->location, N, data.data());
			else if constexpr (std::is_same_v<T, unsigned int>)
				glUniform1uiv(uniform->location, N, data.data());
		}
	}

	template <typename T>
	PipelineUniformConfig* prepareUniform (const std::string &name, size_t size,
			const T* data)
	{
		// Does the uniform exist?
		if (uniforms.find(name) == uniforms.end())
			return nullptr;

		auto &uniform = uniforms[name];
		T *value = reinterpret_cast<T*>(uniform.value.data());

		size_t step = sizeof(T) / sizeof(std::uint8_t);
		if (size == (uniform.value.size()/step)) {
			bool same = true;
			for (size_t i = 0; i < size; i++) {
				if (value[i] != data[i]) {
					same = false;
					break;
				}
			}
			if (same)
				return nullptr;
		}
		else {
			return nullptr;
		}

		// Data is new
		uniform.value.clear();
		uniform.value.resize(size * step);
		memcpy(uniform.value.data(), data, size * step);

		emit("set-program", program);
		emit("set-current", this);

		return &uniform;
	}

	std::string name;

	GL_program program;

	int vertexComponentCount = 0;

	int vertexSize = 0;

	std::vector<PipelineAttribute> attributes;

	std::map<std::string, PipelineUniformConfig> uniforms;
};

}	// namespace Zen

#endif
