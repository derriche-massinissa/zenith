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
#include "utility.hpp"

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

	/**
	 * Sets a uniform of the given name on this shader of type: mat2, mat3, mat4.
	 *
	 * The uniform is only set if the value/s given is/are different than
	 * those previously set.
	 *
	 * This method works by first setting this shader as being the current
	 * shader within the Renderer, if it isn't already. It also sets this
	 * shader as being the current one within the pipeline it belongs to.
	 *
	 * @since 0.0.0
	 *
	 * @tparam T The type of the data.
	 * @tparam N The size of the square matrix.
	 * @param name The name of the uniform to set.
	 * @param transpose Whether to transpose the matrix.
	 * @param mat The new value to set the uniform to.
	 */
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
	 * Sets a uniform of the given name on this shader of type T.
	 *
	 * ```cpp
	 * // vec4 uColor;
	 * shader.set("uColor", 1.f, .7f, .4f, 1.f);
	 * // float uInensity;
	 * shader.set("uIntensity", 1.6f);
	 * ```
	 *
	 * The uniform is only set if the value/s given is/are different than
	 * those previously set.
	 *
	 * This method works by first setting this shader as being the current
	 * shader within the Renderer, if it isn't already. It also sets this
	 * shader as being the current one within the pipeline it belongs to.
	 *
	 * @since 0.0.0
	 *
	 * @tparam T The type of the data.
	 * @param name The name of the uniform to set.
	 * @param value The new first value to set the uniform to.
	 * @param args The new values to set the uniform to in addition to the first.
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
	 * Sets a uniform of the given name on this shader of type vecN
	 * (vec2, vec3, vec4...)
	 *
	 * ```cpp
	 * // vec4 uColor;
	 * shader.set("uColor", {1.f, .7f, .4f, 1.f});
	 * ```
	 *
	 * The uniform is only set if the value/s given is/are different than
	 * those previously set.
	 *
	 * This method works by first setting this shader as being the current
	 * shader within the Renderer, if it isn't already. It also sets this
	 * shader as being the current one within the pipeline it belongs to.
	 *
	 * @since 0.0.0
	 *
	 * @tparam T The type of the data.
	 * @tparam N The size of the vector.
	 * @param name The name of the uniform to set.
	 * @param vec The new value to set the uniform to.
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

	/**
	 * Sets an array uniform of the given name on this shader of type T.
	 *
	 * ```cpp
	 * // float uMainSampler[12];
	 * shader.set("uMainSampler", data);
	 * ```
	 *
	 * The uniform is only set if the value/s given is/are different than
	 * those previously set.
	 *
	 * This method works by first setting this shader as being the current
	 * shader within the Renderer, if it isn't already. It also sets this
	 * shader as being the current one within the pipeline it belongs to.
	 *
	 * @since 0.0.0
	 *
	 * @tparam T The type of the data.
	 * @param name The name of the uniform to set.
	 * @param data The new value to set the uniform to.
	 */
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

	/**
	 * Sets an array uniform of the given name on this shader of type T.
	 *
	 * ```cpp
	 * // float uMainSampler[12];
	 * shader.set("uMainSampler", data);
	 * ```
	 *
	 * The uniform is only set if the value/s given is/are different than
	 * those previously set.
	 *
	 * This method works by first setting this shader as being the current
	 * shader within the Renderer, if it isn't already. It also sets this
	 * shader as being the current one within the pipeline it belongs to.
	 *
	 * @since 0.0.0
	 *
	 * @tparam T The type of the data.
	 * @tparam N The number of elements in the uniform to set.
	 * @param name The name of the uniform to set.
	 * @param data The new value to set the uniform to.
	 */
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
		std::string uniformName = name;

		std::size_t found = uniformName.find("[");
		int index = -1;

		// Is the passed name indexed? e.g. uName[3], uTexture[7]
		if (found != std::string::npos) {
			index = std::stoi(name.substr(found+1));
			uniformName = name.substr(0, found);

			if (index < 0) {
				MessageWarning("Cannot set uniform \"", name, "\" :: Index cannot "
						"be negative");
				return nullptr;
			}
		}

		// Does the uniform exist?
		if (uniforms.find(uniformName) == uniforms.end()) {
			MessageWarning("Cannot set uniform \"", name, "\" :: Uniform does "
					"not exist");
			return nullptr;
		}

		// Get the uniform
		auto &uniform = uniforms[uniformName];

		// The size in bytes of a single element in the uniform if an array,
		// otherwise the size of the whole uniform
		size_t byteSize = sizeof(T) / sizeof(std::uint8_t);

		if ((sizeof(T)*size) > uniform.size) {
			MessageWarning("Cannot set uniform \"", name, "\" :: The size of "
					"the data (", (sizeof(T)*size), " bytes) is greater than "
					"the size of the uniform (", uniform.size, " bytes)");
			return nullptr;
		}

		// If the given name is indexed, is the uniform an array?
		if (index > -1 && uniform.length == 1) {
			// If not, then the passed in name is wrong and shouldn't be indexed
			MessageWarning("Cannot set uniform \"", name, "\" :: The uniform is "
					"not an array");
			return nullptr;
		}
		else if (index == -1) {
			index = 0;
		}
		else if (index >= uniform.length) {
			MessageWarning("Cannot set uniform \"", name, "\" :: Out of bounds "
					"index");
			return nullptr;
		}

		T *value = reinterpret_cast<T*>(
				// Offset pointer if uniform is an array
				uniform.value.data() + byteSize * index
		);

		if (!uniform.value.empty()) {
			// Compare the data
			int res = std::memcmp(data, value, byteSize * size);

			if (res == 0)
				// Both buffers are equal, no need to update the uniform
				return nullptr;
		}
		else {
			uniform.value.resize(uniform.size);
			value = reinterpret_cast<T*>(
					uniform.value.data() + byteSize * index
			);
		}

		// Data is new
		std::memcpy(value, data, byteSize * size);

		emit("set-program", program);
		emit("set-current", this);

		return &uniform;
	}

	/**
	 * The name of this shader in the pipeline it belongs to.
	 *
	 * @since 0.0.0
	 */
	std::string name;

	/**
	 * The OpenGL shader program created from compiling the vertex and fragment
	 * shaders.
	 *
	 * @since 0.0.0
	 */
	GL_program program;

	/**
	 * The amount of vertex attribute components of 32 bit length.
	 *
	 * @since 0.0.0
	 */
	int vertexComponentCount = 0;

	/**
	 * The size, in bytes, of a single vertex.
	 *
	 * This is derived by adding together all of the vertex attributes.
	 *
	 * For example, the Multi Pipeline has the following attributes:
	 *
	 * aPosition - (size 2 x GL_FLOAT) = 8
	 * aTexCoord - (size 2 x GL_FLOAT) = 8
	 * aTexId - (size 1 x GL_FLOAT) = 4
	 * aTintEffect - (size 1 x GL_FLOAT) = 4
	 * aTint - (size 4 x GL_UNSIGNED_BYTE) = 4
	 *
	 * The total, in this case, is 8 + 8 + 4 + 4 + 4 = 28.
	 *
	 * This is calculated automatically during the `createAttributes` method.
	 *
	 * @since 0.0.0
	 */
	int vertexSize = 0;

	/**
	 * Vector of objects that describe the vertex attributes of this shader.
	 *
	 * @since 0.0.0
	 */
	std::vector<PipelineAttribute> attributes;

	/**
	 * The active uniforms that this shader has.
	 *
	 * This is an object that maps the uniform names to their location and
	 * cached values.
	 *
	 * It is populated automatically via the `createUniforms` method.
	 *
	 * @since 0.0.0
	 */
	std::map<std::string, PipelineUniformConfig> uniforms;
};

}	// namespace Zen

#endif
