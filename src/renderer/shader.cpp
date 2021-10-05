/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "shader.hpp"
#include "utility.hpp"
#include "../utils/map/contains.hpp"
#include "renderer.hpp"
#include <functional>

namespace Zen {

extern Renderer g_renderer;

Shader::Shader (std::string name, std::string vertexShader,
		std::string fragmentShader,
		std::vector<PipelineAttributeConfig> attributes)
	: name (name)
{
	createProgram(vertexShader, fragmentShader);
	createAttributes(attributes);
	createUniforms();
}

void Shader::createProgram (std::string vertexShader, std::string fragmentShader)
{
	const char *vsCode = vertexShader.c_str();
	const char *fsCode = fragmentShader.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsCode, nullptr);
	glCompileShader(vs);
	checkCompileErrors(vs, "VERTEX");

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsCode, nullptr);
	glCompileShader(fs);
	checkCompileErrors(fs, "FRAGMENT");

	// Shader program
	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	checkCompileErrors(program, "PROGRAM");

	// Delete the shaders as they're linked into our program now and no
	// longer necessery
	glDeleteShader(vs);
	glDeleteShader(fs);
}

void Shader::checkCompileErrors (GLuint shader, std::string type)
{
	GLint success;

	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			GLchar infoLog[1024];
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			MessageError("Shader compilation failed: ", type, " | ", infoLog);
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			GLchar infoLog[1024];
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			MessageError("Program linking failed: ", type, " | ", infoLog);
		}
	}
}

void Shader::createAttributes (
		std::vector<PipelineAttributeConfig> attributes)
{
	GLuint count = 0;
	GLuint offset = 0;
	std::vector<PipelineAttribute> result;

	vertexComponentCount = 0;

	for (size_t i = 0; i < attributes.size(); i++) {
		auto &element = attributes[i];

		std::string name = element.name;
		int size = element.size;
		GLenum type = element.type;
		std::size_t typeSize = GetGLTypeSize(type);

		bool normalized = element.normalized;

		result.emplace_back(
			name,
			size,
			type,
			offset,
			normalized,
			false,
			-1
		);

		if (typeSize == 4)
			count += size;
		else
			count++;

		// offset in bytes = number of elements in component * byte size of type
		offset += size * typeSize;
	}

	this->vertexSize = offset;
	this->vertexComponentCount = count;
	this->attributes = result;
}

void Shader::bind (bool setAttributes, bool flush)
{
	if (flush)
		emit("pipeline-flush");

	g_renderer.setProgram(program);

	//if (setAttributes)
	//	setAttribPointers();
}

Shader* Shader::rebind ()
{
	g_renderer.setProgram(program);

	setAttribPointers(true);

	return this;
}

void Shader::setAttribPointers (bool reset)
{
	for (size_t i = 0; i < attributes.size(); i++) {
		auto &element = attributes[i];

		if (reset) {
			GLint attribLocation = glGetAttribLocation(program,
					element.name.c_str());

			if (attribLocation >= 0) {
				glEnableVertexAttribArray(attribLocation);
				glVertexAttribPointer(attribLocation, element.size, element.type,
						element.normalized, vertexSize,
						reinterpret_cast<void*>(element.offset));

				element.enabled = true;
				element.location = attribLocation;
			}
			else if (attribLocation != -1) {
				glDisableVertexAttribArray(attribLocation);
			}
		}
		else if (element.enabled) {
			glVertexAttribPointer(element.location, element.size, element.type,
					element.normalized, vertexSize,
					reinterpret_cast<void*>(element.offset));
		}
		else if (!element.enabled && element.location > -1) {
			glDisableVertexAttribArray(element.location);
			element.location = -1;
		}
	}
}

void Shader::createUniforms ()
{
	GLint i = 0;
	GLint location;
	GLsizei length;
	GLint size;
	GLenum type;
	GLchar nameinfo;

	std::string name;

	// Look-up all active uniforms
	GLint totalUniforms;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &totalUniforms);
	
	for (i = 0; i < totalUniforms; i++) {
		glGetActiveUniform(program, i, 512, &length, &size, &type, &nameinfo);

		if (length > 0) {
			name = std::string(&nameinfo);
			location = glGetUniformLocation(program, name.c_str());

			if (location < 0) {
				uniforms[name] = {
					.name = name,
					.location = static_cast<GLuint>(location)
				};
			}

			// If the uniform contains "[]" for an array struct,
			// we'll add an entry for the non-struct name as well.
			// Such as uMainSampler[12] = uMainSampler

			std::size_t found = name.find("[");
			if (found != std::string::npos) {
				name = name.substr(found);

				if (uniforms.find(name) == uniforms.end()) {
					location = glGetUniformLocation(program, name.c_str());

					if (location < 0) {
						uniforms[name] = {
							.name = name,
							.location = static_cast<GLuint>(location)
						};
					}
				}
			}
		}
	}
}

bool Shader::hasUniform (std::string name)
{
	return uniforms.find(name) != uniforms.end();
}

void Shader::resetUniform (std::string name)
{
	if (uniforms.find(name) == uniforms.end())
		return;

	auto &uniform = uniforms[name];

	uniform.value.clear();
}

/*
// Bool ----------------------------------------------------------------------
void Shader::set (const std::string &name, bool value)
{
	auto *uniform = prepareUniform(name, 1, &value);
	if (uniform)
		glUniform1i(uniform->location, value);
}

// Scalars -------------------------------------------------------------------
void Shader::set (const std::string &name, int value)
{
	auto *uniform = prepareUniform(name, 1, &value);
	if (uniform)
		glUniform1i(uniform->location, value);
}

void Shader::set (const std::string &name, unsigned int value)
{
	auto *uniform = prepareUniform(name, 1, &value);
	if (uniform)
		glUniform1ui(uniform->location, value);
}

void Shader::set (const std::string &name, float value)
{
	auto *uniform = prepareUniform(name, 1, &value);
	if (uniform)
		glUniform1f(uniform->location, value);
}

// Vector 2 ------------------------------------------------------------------
void Shader::set (const std::string &name, const glm::vec2 &vec)
{
	const float *v = glm::value_ptr(vec);
	auto *uniform = prepareUniform(name, 2, v);
	if (uniform)
		glUniform2fv(uniform->location, 1, v);
}

void Shader::set (const std::string &name, float x, float y)
{
	const glm::vec2 vec {x, y};
	const float *v = glm::value_ptr(vec);

	auto *uniform = prepareUniform(name, 2, v);
	if (uniform)
		glUniform2fv(uniform->location, 1, v);
}

void Shader::set (const std::string &name, const glm::vec<2, int> &vec)
{
	const int *v = glm::value_ptr(vec);
	auto *uniform = prepareUniform(name, 2, v);

	if (uniform)
		glUniform2iv(uniform->location, 1, v);
}

void Shader::set (const std::string &name, int x, int y)
{
	const glm::vec<2, int> vec {x, y};
	const int *v = glm::value_ptr(vec);

	auto *uniform = prepareUniform(name, 2, v);
	if (uniform)
		glUniform2iv(uniform->location, 1, v);
}

// Vector 3 ------------------------------------------------------------------
void Shader::set (const std::string &name, const glm::vec3 &vec)
{
	const float *v = glm::value_ptr(vec);
	auto *uniform = prepareUniform(name, 3, v);
	if (uniform)
		glUniform3fv(uniform->location, 1, v);
}

void Shader::set (const std::string &name, float x, float y, float z)
{
	const glm::vec3 vec {x, y, z};
	const float *v = glm::value_ptr(vec);

	auto *uniform = prepareUniform(name, 3, v);
	if (uniform)
		glUniform3fv(uniform->location, 1, v);
}

void Shader::set (const std::string &name, const glm::vec<3, int> &vec)
{
	const int *v = glm::value_ptr(vec);
	auto *uniform = prepareUniform(name, 3, v);

	if (uniform)
		glUniform3iv(uniform->location, 1, v);
}

void Shader::set (const std::string &name, int x, int y, int z)
{
	const glm::vec<3, int> vec {x, y, z};
	const int *v = glm::value_ptr(vec);

	auto *uniform = prepareUniform(name, 3, v);
	if (uniform)
		glUniform3iv(uniform->location, 1, v);
}

// Vector 4 ------------------------------------------------------------------
void Shader::set (const std::string &name, const glm::vec4 &vec)
{
	const float *v = glm::value_ptr(vec);
	auto *uniform = prepareUniform(name, 4, v);
	if (uniform)
		glUniform4fv(uniform->location, 1, v);
}

void Shader::set (const std::string &name, float x, float y, float z, float w)
{
	const glm::vec4 vec {x, y, z, w};
	const float *v = glm::value_ptr(vec);

	auto *uniform = prepareUniform(name, 4, v);
	if (uniform)
		glUniform4fv(uniform->location, 1, v);
}

void Shader::set (const std::string &name, const glm::vec<4, int> &vec)
{
	const int *v = glm::value_ptr(vec);
	auto *uniform = prepareUniform(name, 4, v);

	if (uniform)
		glUniform4iv(uniform->location, 1, v);
}

void Shader::set (const std::string &name, int x, int y, int z, int w)
{
	const glm::vec<4, int> vec {x, y, z, w};
	const int *v = glm::value_ptr(vec);

	auto *uniform = prepareUniform(name, 4, v);
	if (uniform)
		glUniform4iv(uniform->location, 1, v);
}

// Matrices ------------------------------------------------------------------
void Shader::set (const std::string &name, bool transpose, const glm::mat2 &mat)
{
	const float *v = glm::value_ptr(mat);
	auto *uniform = prepareUniform(name, 4, v);
	if (uniform)
		glUniformMatrix2fv(uniform->location, 1, transpose, v);
}

void Shader::set (const std::string &name, bool transpose, const glm::mat3 &mat)
{
	const float *v = glm::value_ptr(mat);
	auto *uniform = prepareUniform(name, 9, v);
	if (uniform)
		glUniformMatrix3fv(uniform->location, 1, transpose, v);
}

void Shader::set (const std::string &name, bool transpose, const glm::mat4 &mat)
{
	const float *v = glm::value_ptr(mat);
	auto *uniform = prepareUniform(name, 16, v);
	if (uniform)
		glUniformMatrix4fv(uniform->location, 1, transpose, v);
}
*/

}	// namespace Zen
