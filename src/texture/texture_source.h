/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_TEXTURE_SOURCE_H
#define ZEN_TEXTURES_TEXTURE_SOURCE_H

#include <string>
#include <SDL2/SDL.h>

#include "../window/window.fwd.h"
#include "texture.fwd.h"

namespace Zen {
namespace Textures {

/**
 * A TextureSource is the encapsulation of the actual source data for a Texture.
 *
 * It is simply an image file.
 *
 * A Texture can contain multiple TextureSource, which only happens when a multi-atlas is loaded.
 *
 * @class TextureSource
 * @since 0.0.0
 */
class TextureSource
{
public:
	/**
	 * @since 0.0.0
	 *
	 * @param texture_ The Texture this TextureSource belongs to.
	 * @param source_ The path to the source image.
	 * @param width_ Optional width of the source image. If not given it's derived
	 * from the source itself.
	 * @param height_ Optional height of the source image. If not given it's derived
	 * from the source itself.
	 */
	TextureSource(Texture* texture_, std::string source_);

	/**
	 * @since 0.0.0
	 */
	~TextureSource();

	/**
	 * The renderer of the Game.
	 *
	 * @since 0.0.0
	 */
	Window* window;

	/**
	 * The Texture object this TextureSource belongs to.
	 *
	 * @since 0.0.0
	 */
	Texture* texture;

	/**
	 * The source of the image loaded in this source. It is either a file path
	 * to an image file, or a Base64 Image Data string.
	 *
	 * @since 0.0.0
	 */
	std::string source;

	/**
	 * The resolution of the source image.
	 *
	 * @since 0.0.0
	 */
	float resolution = 1.0;

	/**
	 * Setup the texture source by loading the image, if a path is given, or
	 * creates a Texture from the given Base64 Image Data.
	 *
	 * @since 0.0.0
	 */
	void init ();

	/**
	 * For OpenGL textures... Maybe one day...
	 *
	 * @since 0.0.0
	 */
	void update ();

	/**
	 * The SDL_Texture instance, representing the image loaded in the GPU.
	 *
	 * @since 0.0.0
	 */
	SDL_Texture *sdlTexture = nullptr;

	/**
	 * The width of the source image. If not specified in the constructor, it will
	 * use the image's width.
	 *
	 * @since 0.0.0
	 */
	int width = 0;

	/**
	 * The height of the source image. If not specified in the constructor, it will
	 * use the image's height.
	 *
	 * @since 0.0.0
	 */
	int height = 0;
};

}	// namespace Textures
}	// namespace Zen

#endif
