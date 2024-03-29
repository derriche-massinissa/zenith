/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_UTILITY_HPP
#define ZEN_RENDERER_UTILITY_HPP

#include <string>
#include <array>
#include <SDL2/SDL.h>
#include <GL/glew.h>

namespace Zen {

std::size_t GetGLTypeSize (GLenum type);

std::string GetGLTypeEnumName (GLenum type);

std::string GetGLTypeName (GLenum type);

/**
 * Packs four floats on a range from 0.0 to 1.0 into a single Uint32
 *
 * @since 0.0.0
 *
 * @param r Red component in a range from 0.0 to 1.0
 * @param g Green component in a range from 0.0 to 1.0
 * @param b Blue component in a range from 0.0 to 1.0
 * @param a Alpha component in a range from 0.0 to 1.0
 *
 * @return The packed RGBA values as a Uint32.
 */
std::uint32_t GetTintFromFloats (float r, float g, float b, float a);

/**
 * Packs a Uint24, representing RGB components, with a Float32, representing
 * the alpha component, with a range between 0.0 and 1.0 and return a Uint32
 *
 * @since 0.0.0
 *
 * @param rgb Uint24 representing RGB components
 * @param a Float32 representing Alpha component
 *
 * @return Packed RGBA as Uint32
 */
std::uint32_t GetTintAppendFloatAlpha (int rgb, float a);

/**
 * Packs a Uint24, representing RGB components, with a Float32, representing
 * the alpha component, with a range between 0.0 and 1.0 and return a
 * swizzled Uint32
 *
 * @since 0.0.0
 *
 * @param rgb Uint24 representing RGB components
 * @param a Float32 representing Alpha component
 *
 * @return Packed RGBA as Uint32
 */
std::uint32_t GetTintAppendFloatAlphaAndSwap (int rgb, float a);

std::string ParseFragmentShaderMaxTextures (std::string fragmentShaderSource,
		size_t maxTextures);

std::array<GLenum, 2> GetTexGLFormatFromSDLFormat (SDL_Surface *surface, bool gamma = false);

/**
 * Flips a surface on the y-axis.
 *
 * https://stackoverflow.com/questions/65815332/flipping-a-surface-vertically-in-sdl2/65817254#65817254
 * - vvanpelt
 *
 * @since 0.0.0
 *
 * @param surface The SDL surface to flip vertically.
 */
void FlipSurface (SDL_Surface *surface, bool lockSurface = true);

}	// namespace Zen

#endif
