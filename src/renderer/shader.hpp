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
#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "types/gl_pipeline_attribute.hpp"
#include "types/gl_pipeline_attribute_config.hpp"
#include "types/gl_pipeline_uniforms_config.hpp"
#include "types/gl_types.hpp"
#include "../event/event_emitter.hpp"

namespace Zen {

/**
 * Instances of the Shader class belong to the Pipeline classes.
 * When the pipeline is created it will create an instance of this class for
 * each one of its shaders, as defined in the pipeline configuration.
 *
 * This class encapsulates everything needed to manage a shader in a pipeline,
 * including the shader attributes and uniforms, as well as lots of handy
 * methods such as `set`, for setting uniform values on this shader.
 *
 * Typically, you do not create an instance of this class directly, as it works
 * in unison with the pipeline to which it belongs. You can gain access to this
 * class via a pipeline's `shaders` array, post-creation.
 *
 * @since 0.0.0
 */
class Shader : public EventEmitter {
public:
	/**
	 * @since 0.0.0
	 *
	 * @param name The name of this Shader.
	 * @param vertexShader The vertex shader source code as a single string.
	 * @param fragmentShader The fragment shader source code as a single string.
	 * @param attributes - A vector of attributes.
	 */
	Shader (std::string name, std::string vertexShader,
		std::string fragmentShader,
		std::vector<PipelineAttributeConfig> attributes);

	void createProgram (std::string vertexShader, std::string fragmentShader);

	void checkCompileErrors (GLuint shader, std::string type);
	
    /**
	 * Takes the vertex attributes config and parses it, creating the resulting
	 * array that is stored in this shaders `attributes` property, calculating
	 * the offset, normalization and location in the process.
     *
	 * Calling this method resets Shader::attributes,
	 * Shader::vertexSize and Shader::vertexComponentCount.
     *
	 * It is called automatically when this class is created, but can be called
	 * manually if required.
     *
     * @since 0.0.0
     *
     * @param attributes An array of attributes configs.
     */
	void createAttributes (std::vector<PipelineAttributeConfig> attirbutes);

    /**
	 * Sets the program this shader uses as being the active shader in the Renderer.
     *
	 * This method is called every time the parent pipeline is made the current
	 * active pipeline.
     *
     * @since 0.0.0
     *
     * @param setAttributes Should the vertex attribute pointers be set?
     * @param flush Flush the pipeline before binding this shader?
     */
	void bind (bool setAttributes = false, bool flush = false);

    /**
	 * Sets the program this shader uses as being the active shader in the Renderer.
     *
     * Then resets all of the attribute pointers.
     *
     * @since 0.0.0
     */
	Shader* rebind ();

    /**
     * Sets the vertex attribute pointers.
     *
     * This should __ONLY__ be called after the _vertex array_ has been bound.
     *
     * It is called automatically during the `bind` method.
     *
     * @since 0.0.0
     *
     * @param reset Reset the vertex attribute locations?
     */
	void setAttribPointers (bool reset = false);

    /**
	 * Sets up the `Shader.uniforms` object, populating it with the names and
	 * locations of the shader uniforms this shader requires.
     *
	 * It works by first calling `glGetProgramiv(program, GL_ACTIVE_UNIFORMS,
	 * &totalUniforms)` to find out how many active uniforms this shader has.
	 * It then iterates through them, calling `glGetActiveUniform` to get the
	 * Active Info from each one. Finally, the name and location are stored in
	 * the local array.
     *
     * This method is called automatically when this class is created.
     *
     * @since 0.0.0
     */
	void createUniforms ();

    /**
     * Checks to see if the given uniform name exists and is active in this shader.
     *
     * @since 0.0.0
     *
     * @param name The name of the uniform to check for.
     *
     * @return `true` if the uniform exists, otherwise `false`.
     */
	bool hasUniform (std::string name);

    /**
     * Resets the cached values of the given uniform.
     *
     * @since 0.0.0
     *
     * @param name The name of the uniform to reset.
     */
	void resetUniform (std::string name);

	/*
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
	*/

	// Matrices ------------------------------------------------------------------
	template <typename T, int N>
	void set (const std::string &name, bool transpose, const glm::mat<N, N, T>& mat)
	{
		const T *ptr = glm::value_ptr(mat);
		auto *uniform = prepareUniform(name, N*N, ptr);

		if (!uniform)
			return;

		if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
			if		constexpr	(N == 2)
				glUniformMatrix2fv(uniform->location, 1, transpose, ptr);
			else if constexpr	(N == 3)
				glUniformMatrix3fv(uniform->location, 1, transpose, ptr);
			else if constexpr	(N == 4)
				glUniformMatrix4fv(uniform->location, 1, transpose, ptr);
		}
		else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, bool>) {
			if		constexpr	(N == 2)
				glUniformMatrix2iv(uniform->location, 1, transpose, ptr);
			else if constexpr	(N == 3)
				glUniformMatrix3iv(uniform->location, 1, transpose, ptr);
			else if constexpr	(N == 4)
				glUniformMatrix4iv(uniform->location, 1, transpose, ptr);
		}
		else if constexpr (std::is_same_v<T, unsigned int>) {
			if		constexpr	(N == 2)
				glUniformMatrix2uiv(uniform->location, 1, transpose, ptr);
			else if constexpr	(N == 3)
				glUniformMatrix3uiv(uniform->location, 1, transpose, ptr);
			else if constexpr	(N == 4)
				glUniformMatrix4uiv(uniform->location, 1, transpose, ptr);
		}
		else {
			MessageWarning("Unsupported uniform type!");
		}
	}

	/**
	 * One to 4 scalars
	 *
	 * ```cpp
	 * set("uColor", 0.4, 0.5, 0.1);
	 * set("uSize", 175);
	 * set("uDensity", 57.4);
	 * ```
	 *
	 * @since 0.0.0
	 */
	/*
	template <typename T>
	void set (const std::string &name, T value)
	{
		std::vector<T> vec {value};
		auto *uniform = prepareUniform(name, 1, vec.data());
		if (!uniform) return;
	}

	template <typename T>
	void set (const std::string &name, T value, T value2)
	{
	}

	template <typename T>
	void set (const std::string &name, T value, T value2, T value3)
	{
	}

	template <typename T>
	void set (const std::string &name, T value, T value2, T value3, T value4)
	{
	}
	*/

	template <
		typename T,
		typename ... Args,
		typename = std::enable_if_t<(std::is_same_v<T, Args> && ...)>
	>
	void set (const std::string &name, T value, Args ... args)
	{
		constexpr std::size_t N = sizeof...(Args) + 1;

		PipelineUniformConfig *uniform;
		if constexpr (std::is_same_v<T, double>) {		// If double, make it float
			std::vector<float> vec {value, args...};
			uniform = prepareUniform(name, N, vec.data());
		}
		else if constexpr (std::is_same_v<T, bool>) {	// If bool, make it int
			std::vector<int> vec {value, args...};
			uniform = prepareUniform(name, N, vec.data());
		}
		else {
			std::vector<T> vec {value, args...};
			uniform = prepareUniform(name, N, vec.data());
		}

		if (!uniform)
			return;

		if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
			if		constexpr (N == 1)
				glUniform1f(uniform->location, value);
			else if	constexpr (N == 2)
				glUniform2f(uniform->location, value, args...);
			else if	constexpr (N == 3)
				glUniform3f(uniform->location, value, args...);
			else if	constexpr (N == 4)
				glUniform4f(uniform->location, value, args...);
		}
		else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, bool>) {
			if		constexpr (N == 1)
				glUniform1i(uniform->location, value);
			else if	constexpr (N == 2)
				glUniform2i(uniform->location, value, args...);
			else if	constexpr (N == 3)
				glUniform3i(uniform->location, value, args...);
			else if	constexpr (N == 4)
				glUniform4i(uniform->location, value, args...);
		}
		else if constexpr (std::is_same_v<T, unsigned int>) {
			if		constexpr (N == 1)
				glUniform1ui(uniform->location, value);
			else if	constexpr (N == 2)
				glUniform2ui(uniform->location, value, args...);
			else if	constexpr (N == 3)
				glUniform3ui(uniform->location, value, args...);
			else if	constexpr (N == 4)
				glUniform4ui(uniform->location, value, args...);
		}
		else {
			MessageWarning("Unsupported uniform type!");
		}
	}

	/**
	 * GLM Vector
	 */
	template <typename T, int N>
	void set (const std::string &name, const glm::vec<N, T>& vec)
	{
		const T *ptr = glm::value_ptr(vec);
		auto *uniform = prepareUniform(name, N, ptr);

		if (!uniform)
			return;

		if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
			if		constexpr (N == 2) glUniform2fv(uniform->location, 1, ptr);
			else if	constexpr (N == 3) glUniform3fv(uniform->location, 1, ptr);
			else if	constexpr (N == 4) glUniform4fv(uniform->location, 1, ptr);
		}
		else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, bool>) {
			if		constexpr (N == 2) glUniform2iv(uniform->location, 1, ptr);
			else if	constexpr (N == 3) glUniform3iv(uniform->location, 1, ptr);
			else if	constexpr (N == 4) glUniform4iv(uniform->location, 1, ptr);
		}
		else if constexpr (std::is_same_v<T, unsigned int>) {
			if		constexpr (N == 2) glUniform2uiv(uniform->location, 1, ptr);
			else if	constexpr (N == 3) glUniform3uiv(uniform->location, 1, ptr);
			else if	constexpr (N == 4) glUniform4uiv(uniform->location, 1, ptr);
		}
		else {
			MessageWarning("Unsupported uniform type!");
		}
	}

	// Vector ---------------------------------------------------------------------
	template <typename T>
	void set (const std::string &name, const std::vector<T>& data)
	{
		auto *uniform = prepareUniform(name, data.size(), data.data());

		if (!uniform)
			return;

		if constexpr (std::is_same_v<T, float>)
			glUniform1fv(uniform->location, data.size(), data.data());
		else if constexpr (std::is_same_v<T, int>)
			glUniform1iv(uniform->location, data.size(), data.data());
		else if constexpr (std::is_same_v<T, unsigned int>)
			glUniform1uiv(uniform->location, data.size(), data.data());
		else
			MessageWarning("Unsupported uniform type!");
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
			else
				MessageWarning("Unsupported uniform type!");
		}
	}

	/**
	 * Find uniform and verifies if the data we're about to set it to is different,
	 * otherwise, updating it is useless.
	 *
	 * @since 0.0.0
	 *
	 * @tparam T The type of the data and thus of the uniform itself.
	 * @param name The name of the uniform.
	 * @param size The number of elements in the data array.
	 * @param data A data array.
	 */
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
		std::memcpy(uniform.value.data(), data, size * step);

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
