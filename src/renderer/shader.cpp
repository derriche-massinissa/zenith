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

	glUseProgram(program);
}

void Shader::checkCompileErrors (GLuint shader, std::string type)
{
	GLint success;

	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			GLchar infoLog[1024];
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			MessageError("Shader compilation failed for shader \"", name, "\" : ",
					type, " | ", infoLog);
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			GLchar infoLog[1024];
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			MessageError("Program linking failed for shader \"", name, "\" : ",
					type, " | ", infoLog);
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
		int size = element.size;	// 1 for float, 2 for vec2, 4 for vec4...
		GLenum type = element.type;	// GL_FLOAT, GL_UNSIGNED_BYTE...
		std::size_t typeSize = GetGLTypeSize(type);	// Size of the type in byte

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

	if (setAttributes)
		setAttribPointers();
}

Shader* Shader::rebind ()
{
	g_renderer.setProgram(program);

	setAttribPointers(true);

	return this;
}

void Shader::setAttribPointers (bool reset)
{
	int tmp = -3;
	tmp++;

	for (size_t i = 0; i < attributes.size(); i++) {
		auto &element = attributes[i];

		if (reset) {
			GLint attribLocation = i;

			glEnableVertexAttribArray(attribLocation);
			glVertexAttribPointer(attribLocation, element.size, element.type,
					element.normalized, vertexSize,
					reinterpret_cast<void*>(element.offset));

			element.enabled = true;
			element.location = attribLocation;
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
	GLchar nameBuffer[512];

	std::string name;

	// Look-up all active uniforms
	GLint totalUniforms;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &totalUniforms);
	
	for (i = 0; i < totalUniforms; i++) {
		glGetActiveUniform(program, i, 512, &length, &size, &type, nameBuffer);

		if (length > 0) {
			name = std::string(nameBuffer);
			// If array, remove the "[0]"
			std::size_t idx = name.find("[");
			if (idx != std::string::npos) {
				name = name.substr(0, idx);
			}

			location = glGetUniformLocation(program, name.c_str());

			if (location >= 0) {
				uniforms[name] = {
					.name = name,
					.location = static_cast<GLuint>(location),
					.length = size,
					.type = type,
					.size = GetGLTypeSize(type) * size
				};
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

}	// namespace Zen
