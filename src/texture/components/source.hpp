struct TextureSourceComponent
{
	/**
	 * The texture to which this Source belongs to.
	 *
	 * @property
	 * @since 0.0.0
	 */
	Entity texture;

	/**
	 * The source used to create this Source.
	 *
	 * Typically a path to an image file, but can also be Base64 image data.
	 *
	 * @property
	 * @since 0.0.0
	 */
	const char* source;

	/**
	 * The index of this source in the owning Texture.
	 *
	 * @property
	 * @since 0.0.0
	 */
	int index;

	/**
	 * The resolution of this Source.
	 *
	 * @property
	 * @since 0.0.0
	 */
	double resolution;

	/**
	 * The SDL_Texture loaded in memory that this Source represents.
	 *
	 * @property
	 * @since 0.0.0
	 */
	SDL_Texture *sdlTexture;

	/**
	 * The dimensions of this Source.
	 *
	 * @property
	 * @since 0.0.0
	 */
	int width, height;
}
