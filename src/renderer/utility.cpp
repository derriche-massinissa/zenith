/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "utility.hpp"
#include <map>
#include <cstring>
#include "../utils/string/replace.hpp"
#include "../utils/assert.hpp"

namespace Zen {

const static std::map<GLenum, size_t> gl_type_size {
	{ GL_UNSIGNED_BYTE, 1 },
	{ GL_BYTE, 1 },

	{ GL_FLOAT, 4 },
	{ GL_INT, 4 },
	{ GL_UNSIGNED_INT, 4 },
	{ GL_BOOL, 4 },

	{ GL_FLOAT_VEC2, 8 },
	{ GL_INT_VEC2, 8 },
	{ GL_UNSIGNED_INT_VEC2, 8 },
	{ GL_BOOL_VEC2, 8 },

	{ GL_FLOAT_VEC3, 12 },
	{ GL_INT_VEC3, 12 },
	{ GL_UNSIGNED_INT_VEC3, 12 },
	{ GL_BOOL_VEC3, 12 },

	{ GL_FLOAT_VEC4, 16 },
	{ GL_INT_VEC4, 16 },
	{ GL_UNSIGNED_INT_VEC4, 16 },
	{ GL_BOOL_VEC4, 16 },

	{ GL_FLOAT_MAT2, 16 },
	{ GL_FLOAT_MAT3, 32 },
	{ GL_FLOAT_MAT4, 64 },

	{ GL_FLOAT_MAT2x3, 24 },
	{ GL_FLOAT_MAT2x4, 32 },

	{ GL_FLOAT_MAT3x2, 24 },
	{ GL_FLOAT_MAT3x4, 48 },

	{ GL_FLOAT_MAT4x2, 32 },
	{ GL_FLOAT_MAT4x3, 48 },

	{ GL_SAMPLER_1D, 4 },
	{ GL_SAMPLER_2D, 4 },
	{ GL_SAMPLER_3D, 4 },
};

const static std::map<GLenum, std::string> gl_type_enum_name {
	{ GL_FLOAT, "GL_FLOAT" },
	{ GL_FLOAT_VEC2, "GL_FLOAT_VEC2" },
	{ GL_FLOAT_VEC3, "GL_FLOAT_VEC3" },
	{ GL_FLOAT_VEC4, "GL_FLOAT_VEC4" },
	{ GL_DOUBLE, "GL_DOUBLE" },
	{ GL_DOUBLE_VEC2, "GL_DOUBLE_VEC2" },
	{ GL_DOUBLE_VEC3, "GL_DOUBLE_VEC3" },
	{ GL_DOUBLE_VEC4, "GL_DOUBLE_VEC4" },
	{ GL_INT, "GL_INT" },
	{ GL_INT_VEC2, "GL_INT_VEC2" },
	{ GL_INT_VEC3, "GL_INT_VEC3" },
	{ GL_INT_VEC4, "GL_INT_VEC4" },
	{ GL_UNSIGNED_INT, "GL_UNSIGNED_INT" },
	{ GL_UNSIGNED_INT_VEC2, "GL_UNSIGNED_INT_VEC2" },
	{ GL_UNSIGNED_INT_VEC3, "GL_UNSIGNED_INT_VEC3" },
	{ GL_UNSIGNED_INT_VEC4, "GL_UNSIGNED_INT_VEC4" },
	{ GL_BOOL, "GL_BOOL" },
	{ GL_BOOL_VEC2, "GL_BOOL_VEC2" },
	{ GL_BOOL_VEC3, "GL_BOOL_VEC3" },
	{ GL_BOOL_VEC4, "GL_BOOL_VEC4" },
	{ GL_FLOAT_MAT2, "GL_FLOAT_MAT2" },
	{ GL_FLOAT_MAT3, "GL_FLOAT_MAT3" },
	{ GL_FLOAT_MAT4, "GL_FLOAT_MAT4" },
	{ GL_FLOAT_MAT2x3, "GL_FLOAT_MAT2x3" },
	{ GL_FLOAT_MAT2x4, "GL_FLOAT_MAT2x4" },
	{ GL_FLOAT_MAT3x2, "GL_FLOAT_MAT3x2" },
	{ GL_FLOAT_MAT3x4, "GL_FLOAT_MAT3x4" },
	{ GL_FLOAT_MAT4x2, "GL_FLOAT_MAT4x2" },
	{ GL_FLOAT_MAT4x3, "GL_FLOAT_MAT4x3" },
	{ GL_DOUBLE_MAT2, "GL_DOUBLE_MAT2" },
	{ GL_DOUBLE_MAT3, "GL_DOUBLE_MAT3" },
	{ GL_DOUBLE_MAT4, "GL_DOUBLE_MAT4" },
	{ GL_DOUBLE_MAT2x3, "GL_DOUBLE_MAT2x3" },
	{ GL_DOUBLE_MAT2x4, "GL_DOUBLE_MAT2x4" },
	{ GL_DOUBLE_MAT3x2, "GL_DOUBLE_MAT3x2" },
	{ GL_DOUBLE_MAT3x4, "GL_DOUBLE_MAT3x4" },
	{ GL_DOUBLE_MAT4x2, "GL_DOUBLE_MAT4x2" },
	{ GL_DOUBLE_MAT4x3, "GL_DOUBLE_MAT4x3" },
	{ GL_SAMPLER_1D, "GL_SAMPLER_1D" },
	{ GL_SAMPLER_2D, "GL_SAMPLER_2D" },
	{ GL_SAMPLER_3D, "GL_SAMPLER_3D" },
	{ GL_SAMPLER_CUBE, "GL_SAMPLER_CUBE" },
	{ GL_SAMPLER_1D_SHADOW, "GL_SAMPLER_1D_SHADOW" },
	{ GL_SAMPLER_2D_SHADOW, "GL_SAMPLER_2D_SHADOW" },
	{ GL_SAMPLER_1D_ARRAY, "GL_SAMPLER_1D_ARRAY" },
	{ GL_SAMPLER_2D_ARRAY, "GL_SAMPLER_2D_ARRAY" },
	{ GL_SAMPLER_1D_ARRAY_SHADOW, "GL_SAMPLER_1D_ARRAY_SHADOW" },
	{ GL_SAMPLER_2D_ARRAY_SHADOW, "GL_SAMPLER_2D_ARRAY_SHADOW" },
	{ GL_SAMPLER_2D_MULTISAMPLE, "GL_SAMPLER_2D_MULTISAMPLE" },
	{ GL_SAMPLER_2D_MULTISAMPLE_ARRAY, "GL_SAMPLER_2D_MULTISAMPLE_ARRAY" },
	{ GL_SAMPLER_CUBE_SHADOW, "GL_SAMPLER_CUBE_SHADOW" },
	{ GL_SAMPLER_BUFFER, "GL_SAMPLER_BUFFER" },
	{ GL_SAMPLER_2D_RECT, "GL_SAMPLER_2D_RECT" },
	{ GL_SAMPLER_2D_RECT_SHADOW, "GL_SAMPLER_2D_RECT_SHADOW" },
	{ GL_INT_SAMPLER_1D, "GL_INT_SAMPLER_1D" },
	{ GL_INT_SAMPLER_2D, "GL_INT_SAMPLER_2D" },
	{ GL_INT_SAMPLER_3D, "GL_INT_SAMPLER_3D" },
	{ GL_INT_SAMPLER_CUBE, "GL_INT_SAMPLER_CUBE" },
	{ GL_INT_SAMPLER_1D_ARRAY, "GL_INT_SAMPLER_1D_ARRAY" },
	{ GL_INT_SAMPLER_2D_ARRAY, "GL_INT_SAMPLER_2D_ARRAY" },
	{ GL_INT_SAMPLER_2D_MULTISAMPLE, "GL_INT_SAMPLER_2D_MULTISAMPLE" },
	{ GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, "GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY" },
	{ GL_INT_SAMPLER_BUFFER, "GL_INT_SAMPLER_BUFFER" },
	{ GL_INT_SAMPLER_2D_RECT, "GL_INT_SAMPLER_2D_RECT" },
	{ GL_UNSIGNED_INT_SAMPLER_1D, "GL_UNSIGNED_INT_SAMPLER_1D" },
	{ GL_UNSIGNED_INT_SAMPLER_2D, "GL_UNSIGNED_INT_SAMPLER_2D" },
	{ GL_UNSIGNED_INT_SAMPLER_3D, "GL_UNSIGNED_INT_SAMPLER_3D" },
	{ GL_UNSIGNED_INT_SAMPLER_CUBE, "GL_UNSIGNED_INT_SAMPLER_CUBE" },
	{ GL_UNSIGNED_INT_SAMPLER_1D_ARRAY, "GL_UNSIGNED_INT_SAMPLER_1D_ARRAY" },
	{ GL_UNSIGNED_INT_SAMPLER_2D_ARRAY, "GL_UNSIGNED_INT_SAMPLER_2D_ARRAY" },
	{ GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE, "GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE" },
	{ GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, "GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY" },
	{ GL_UNSIGNED_INT_SAMPLER_BUFFER, "GL_UNSIGNED_INT_SAMPLER_BUFFER" },
	{ GL_UNSIGNED_INT_SAMPLER_2D_RECT, "GL_UNSIGNED_INT_SAMPLER_2D_RECT" },
	{ GL_IMAGE_1D, "GL_IMAGE_1D" },
	{ GL_IMAGE_2D, "GL_IMAGE_2D" },
	{ GL_IMAGE_3D, "GL_IMAGE_3D" },
	{ GL_IMAGE_2D_RECT, "GL_IMAGE_2D_RECT" },
	{ GL_IMAGE_CUBE, "GL_IMAGE_CUBE" },
	{ GL_IMAGE_BUFFER, "GL_IMAGE_BUFFER" },
	{ GL_IMAGE_1D_ARRAY, "GL_IMAGE_1D_ARRAY" },
	{ GL_IMAGE_2D_ARRAY, "GL_IMAGE_2D_ARRAY" },
	{ GL_IMAGE_2D_MULTISAMPLE, "GL_IMAGE_2D_MULTISAMPLE" },
	{ GL_IMAGE_2D_MULTISAMPLE_ARRAY, "GL_IMAGE_2D_MULTISAMPLE_ARRAY" },
	{ GL_INT_IMAGE_1D, "GL_INT_IMAGE_1D" },
	{ GL_INT_IMAGE_2D, "GL_INT_IMAGE_2D" },
	{ GL_INT_IMAGE_3D, "GL_INT_IMAGE_3D" },
	{ GL_INT_IMAGE_2D_RECT, "GL_INT_IMAGE_2D_RECT" },
	{ GL_INT_IMAGE_CUBE, "GL_INT_IMAGE_CUBE" },
	{ GL_INT_IMAGE_BUFFER, "GL_INT_IMAGE_BUFFER" },
	{ GL_INT_IMAGE_1D_ARRAY, "GL_INT_IMAGE_1D_ARRAY" },
	{ GL_INT_IMAGE_2D_ARRAY, "GL_INT_IMAGE_2D_ARRAY" },
	{ GL_INT_IMAGE_2D_MULTISAMPLE, "GL_INT_IMAGE_2D_MULTISAMPLE" },
	{ GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY, "GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY" },
	{ GL_UNSIGNED_INT_IMAGE_1D, "GL_UNSIGNED_INT_IMAGE_1D" },
	{ GL_UNSIGNED_INT_IMAGE_2D, "GL_UNSIGNED_INT_IMAGE_2D" },
	{ GL_UNSIGNED_INT_IMAGE_3D, "GL_UNSIGNED_INT_IMAGE_3D" },
	{ GL_UNSIGNED_INT_IMAGE_2D_RECT, "GL_UNSIGNED_INT_IMAGE_2D_RECT" },
	{ GL_UNSIGNED_INT_IMAGE_CUBE, "GL_UNSIGNED_INT_IMAGE_CUBE" },
	{ GL_UNSIGNED_INT_IMAGE_BUFFER, "GL_UNSIGNED_INT_IMAGE_BUFFER" },
	{ GL_UNSIGNED_INT_IMAGE_1D_ARRAY, "GL_UNSIGNED_INT_IMAGE_1D_ARRAY" },
	{ GL_UNSIGNED_INT_IMAGE_2D_ARRAY, "GL_UNSIGNED_INT_IMAGE_2D_ARRAY" },
	{ GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE, "GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE" },
	{ GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY, "GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY" },
	{ GL_UNSIGNED_INT_ATOMIC_COUNTER, "GL_UNSIGNED_INT_ATOMIC_COUNTER" }
};

const static std::map<GLenum, std::string> gl_type_name {
	{ GL_FLOAT, "float" },
	{ GL_FLOAT_VEC2, "vec2" },
	{ GL_FLOAT_VEC3, "vec3" },
	{ GL_FLOAT_VEC4, "vec4" },
	{ GL_DOUBLE, "double" },
	{ GL_DOUBLE_VEC2, "dvec2" },
	{ GL_DOUBLE_VEC3, "dvec3" },
	{ GL_DOUBLE_VEC4, "dvec4" },
	{ GL_INT, "int" },
	{ GL_INT_VEC2, "ivec2" },
	{ GL_INT_VEC3, "ivec3" },
	{ GL_INT_VEC4, "ivec4" },
	{ GL_UNSIGNED_INT, "unsigned int" },
	{ GL_UNSIGNED_INT_VEC2, "uvec2" },
	{ GL_UNSIGNED_INT_VEC3, "uvec3" },
	{ GL_UNSIGNED_INT_VEC4, "uvec4" },
	{ GL_BOOL, "bool" },
	{ GL_BOOL_VEC2, "bvec2" },
	{ GL_BOOL_VEC3, "bvec3" },
	{ GL_BOOL_VEC4, "bvec4" },
	{ GL_FLOAT_MAT2, "mat2" },
	{ GL_FLOAT_MAT3, "mat3" },
	{ GL_FLOAT_MAT4, "mat4" },
	{ GL_FLOAT_MAT2x3, "mat2x3" },
	{ GL_FLOAT_MAT2x4, "mat2x4" },
	{ GL_FLOAT_MAT3x2, "mat3x2" },
	{ GL_FLOAT_MAT3x4, "mat3x4" },
	{ GL_FLOAT_MAT4x2, "mat4x2" },
	{ GL_FLOAT_MAT4x3, "mat4x3" },
	{ GL_DOUBLE_MAT2, "dmat2" },
	{ GL_DOUBLE_MAT3, "dmat3" },
	{ GL_DOUBLE_MAT4, "dmat4" },
	{ GL_DOUBLE_MAT2x3, "dmat2x3" },
	{ GL_DOUBLE_MAT2x4, "dmat2x4" },
	{ GL_DOUBLE_MAT3x2, "dmat3x2" },
	{ GL_DOUBLE_MAT3x4, "dmat3x4" },
	{ GL_DOUBLE_MAT4x2, "dmat4x2" },
	{ GL_DOUBLE_MAT4x3, "dmat4x3" },
	{ GL_SAMPLER_1D, "sampler1D" },
	{ GL_SAMPLER_2D, "sampler2D" },
	{ GL_SAMPLER_3D, "sampler3D" },
	{ GL_SAMPLER_CUBE, "samplerCube" },
	{ GL_SAMPLER_1D_SHADOW, "sampler1DShadow" },
	{ GL_SAMPLER_2D_SHADOW, "sampler2DShadow" },
	{ GL_SAMPLER_1D_ARRAY, "sampler1DArray" },
	{ GL_SAMPLER_2D_ARRAY, "sampler2DArray" },
	{ GL_SAMPLER_1D_ARRAY_SHADOW, "sampler1DArrayShadow" },
	{ GL_SAMPLER_2D_ARRAY_SHADOW, "sampler2DArrayShadow" },
	{ GL_SAMPLER_2D_MULTISAMPLE, "sampler2DMS" },
	{ GL_SAMPLER_2D_MULTISAMPLE_ARRAY, "sampler2DMSArray" },
	{ GL_SAMPLER_CUBE_SHADOW, "samplerCubeShadow" },
	{ GL_SAMPLER_BUFFER, "samplerBuffer" },
	{ GL_SAMPLER_2D_RECT, "sampler2DRect" },
	{ GL_SAMPLER_2D_RECT_SHADOW, "sampler2DRectShadow" },
	{ GL_INT_SAMPLER_1D, "isampler1D" },
	{ GL_INT_SAMPLER_2D, "isampler2D" },
	{ GL_INT_SAMPLER_3D, "isampler3D" },
	{ GL_INT_SAMPLER_CUBE, "isamplerCube" },
	{ GL_INT_SAMPLER_1D_ARRAY, "isampler1DArray" },
	{ GL_INT_SAMPLER_2D_ARRAY, "isampler2DArray" },
	{ GL_INT_SAMPLER_2D_MULTISAMPLE, "isampler2DMS" },
	{ GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, "isampler2DMSArray" },
	{ GL_INT_SAMPLER_BUFFER, "isamplerBuffer" },
	{ GL_INT_SAMPLER_2D_RECT, "isampler2DRect" },
	{ GL_UNSIGNED_INT_SAMPLER_1D, "usampler1D" },
	{ GL_UNSIGNED_INT_SAMPLER_2D, "usampler2D" },
	{ GL_UNSIGNED_INT_SAMPLER_3D, "usampler3D" },
	{ GL_UNSIGNED_INT_SAMPLER_CUBE, "usamplerCube" },
	{ GL_UNSIGNED_INT_SAMPLER_1D_ARRAY, "usampler2DArray" },
	{ GL_UNSIGNED_INT_SAMPLER_2D_ARRAY, "usampler2DArray" },
	{ GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE, "usampler2DMS" },
	{ GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, "usampler2DMSArray" },
	{ GL_UNSIGNED_INT_SAMPLER_BUFFER, "usamplerBuffer" },
	{ GL_UNSIGNED_INT_SAMPLER_2D_RECT, "usampler2DRect" },
	{ GL_IMAGE_1D, "image1D" },
	{ GL_IMAGE_2D, "image2D" },
	{ GL_IMAGE_3D, "image3D" },
	{ GL_IMAGE_2D_RECT, "image2DRect" },
	{ GL_IMAGE_CUBE, "imageCube" },
	{ GL_IMAGE_BUFFER, "imageBuffer" },
	{ GL_IMAGE_1D_ARRAY, "image1DArray" },
	{ GL_IMAGE_2D_ARRAY, "image2DArray" },
	{ GL_IMAGE_2D_MULTISAMPLE, "image2DMS" },
	{ GL_IMAGE_2D_MULTISAMPLE_ARRAY, "image2DMSArray" },
	{ GL_INT_IMAGE_1D, "iimage1D" },
	{ GL_INT_IMAGE_2D, "iimage2D" },
	{ GL_INT_IMAGE_3D, "iimage3D" },
	{ GL_INT_IMAGE_2D_RECT, "iimage2DRect" },
	{ GL_INT_IMAGE_CUBE, "iimageCube" },
	{ GL_INT_IMAGE_BUFFER, "iimageBuffer" },
	{ GL_INT_IMAGE_1D_ARRAY, "iimage1DArray" },
	{ GL_INT_IMAGE_2D_ARRAY, "iimage2DArray" },
	{ GL_INT_IMAGE_2D_MULTISAMPLE, "iimage2DMS" },
	{ GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY, "iimage2DMSArray" },
	{ GL_UNSIGNED_INT_IMAGE_1D, "uimage1D" },
	{ GL_UNSIGNED_INT_IMAGE_2D, "uimage2D" },
	{ GL_UNSIGNED_INT_IMAGE_3D, "uimage3D" },
	{ GL_UNSIGNED_INT_IMAGE_2D_RECT, "uimage2DRect" },
	{ GL_UNSIGNED_INT_IMAGE_CUBE, "uimageCube" },
	{ GL_UNSIGNED_INT_IMAGE_BUFFER, "uimageBuffer" },
	{ GL_UNSIGNED_INT_IMAGE_1D_ARRAY, "uimage1DArray" },
	{ GL_UNSIGNED_INT_IMAGE_2D_ARRAY, "uimage2DArray" },
	{ GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE, "uimage2DMS" },
	{ GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY, "uimage2DMSArray" },
	{ GL_UNSIGNED_INT_ATOMIC_COUNTER, "atomic_uint" }
};

std::size_t GetGLTypeSize (GLenum type)
{
	size_t size = 0;

	ZEN_ASSERT(gl_type_size.find(type) != gl_type_size.end(),
			std::string("Requested GL type is not supported (") +
			GetGLTypeEnumName(type) + ")");

	size = gl_type_size.at(type);

	return size;
}

std::string GetGLTypeEnumName (GLenum type)
{
	std::string name;

	if (gl_type_enum_name.find(type) != gl_type_enum_name.end())
		name = gl_type_enum_name.at(type);

	return name;
}

std::string GetGLTypeName (GLenum type)
{
	std::string name;

	if (gl_type_name.find(type) != gl_type_name.end())
		name = gl_type_name.at(type);

	return name;
}

std::uint32_t GetTintFromFloats (float r, float g, float b, float a)
{
	std::uint32_t ur = static_cast<int>(r * 255) & 0xff;
	std::uint32_t ug = static_cast<int>(g * 255) & 0xff;
	std::uint32_t ub = static_cast<int>(b * 255) & 0xff;
	std::uint32_t ua = static_cast<int>(a * 255) & 0xff;

	return ((ua << 24) | (ur << 16) | (ug << 8) | ub);
}

std::uint32_t GetTintAppendFloatAlpha (int rgb, float a)
{
	std::uint32_t ua = static_cast<int>(a * 255) & 0xff;

	return (ua << 24 | rgb);
}

std::uint32_t GetTintAppendFloatAlphaAndSwap (int rgb, float a)
{
	std::uint32_t ur = (rgb >> 16) & 0xff;
	std::uint32_t ug = (rgb >> 8) & 0xff;
	std::uint32_t ub = rgb & 0xff;
	std::uint32_t ua = static_cast<int>(a * 255) & 0xff;

	return (ua << 24) | (ub << 16) | (ug << 8) | ur;
}

std::string ParseFragmentShaderMaxTextures (std::string fragmentShaderSource,
		size_t maxTextures)
{
	if (fragmentShaderSource.empty())
		return "";

	std::string src;

	for (size_t i = 0; i < maxTextures; i++) {
		if (i > 0)
			src += "\n\telse ";

		if (i < maxTextures - 1)
			src += "if (TexId < " + std::to_string(i) + ".5f)";

		src += "\n\t{";
		src += "\n\t\ttexture = texture2D(uMainSampler[" + std::to_string(i) +
			"], TexCoord);";
		src += "\n\t}";
	}

	fragmentShaderSource = Replace(fragmentShaderSource, "%count%",
			std::to_string(maxTextures));

	fragmentShaderSource = Replace(fragmentShaderSource, "%forloop%", src);

	return fragmentShaderSource;
}

std::array<GLenum, 2> GetTexGLFormatFromSDLFormat (SDL_Surface *surface, bool gamma)
{
	GLenum format;
	GLenum gformat;

	switch (surface->format->format) {
		case SDL_PIXELFORMAT_BGR24:
		case SDL_PIXELFORMAT_BGR444:
		case SDL_PIXELFORMAT_BGR555:
		case SDL_PIXELFORMAT_BGR565:
		case SDL_PIXELFORMAT_BGR888:
			format = GL_BGR;
			gformat = (gamma) ? GL_SRGB : GL_RGB;
			break;

		case SDL_PIXELFORMAT_BGRA32:
		case SDL_PIXELFORMAT_BGRA4444:
		case SDL_PIXELFORMAT_BGRA5551:
		case SDL_PIXELFORMAT_BGRA8888:
		case SDL_PIXELFORMAT_BGRX8888:
			format = GL_BGRA;
			gformat = (gamma) ? GL_SRGB_ALPHA : GL_RGBA;
			break;

		case SDL_PIXELFORMAT_RGB24:
		case SDL_PIXELFORMAT_RGB444:
		case SDL_PIXELFORMAT_RGB555:
		case SDL_PIXELFORMAT_RGB565:
		case SDL_PIXELFORMAT_RGB888:
			format = GL_RGB;
			gformat = (gamma) ? GL_SRGB : GL_RGB;
			break;

		case SDL_PIXELFORMAT_RGBA32:
		case SDL_PIXELFORMAT_RGBA4444:
		case SDL_PIXELFORMAT_RGBA5551:
		case SDL_PIXELFORMAT_RGBA8888:
		case SDL_PIXELFORMAT_RGBX8888:
			format = GL_RGBA;
			gformat = (gamma) ? GL_SRGB_ALPHA : GL_RGBA;
			break;

		default:
			format = GL_RGB;
			gformat = (gamma) ? GL_SRGB : GL_RGB;
			break;
	}

	return {format, gformat};
}

void FlipSurface (SDL_Surface *surface, bool lockSurface)
{
	if (surface == nullptr)
		return;

	if (lockSurface)
		SDL_LockSurface(surface);

	int pitch = surface->pitch;
	std::uint8_t *temp = new std::uint8_t[pitch];
	std::uint8_t *pixels = static_cast<std::uint8_t*>(surface->pixels);
	std::uint8_t *row1, *row2;

	for (int i = 0; i < (surface->h / 2); i++) {
		// Get pointers to the two rows to swap
		row1 = pixels + i * pitch;
		row2 = pixels + (surface->h - i - 1) * pitch;

		// Swap rows
		std::memcpy(temp, row1, pitch);
		std::memcpy(row1, row2, pitch);
		std::memcpy(row2, temp, pitch);
	}

	delete[] temp;

	if (lockSurface)
		SDL_UnlockSurface(surface);
}

}	// namespace Zen
