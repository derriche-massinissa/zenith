/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_FRAME_H
#define ZEN_TEXTURES_FRAME_H

#include <string>
#include <cmath>
#include "frame_data.h"
#include "../math/math.h"

namespace Zen {
namespace Textures {

/**
 * A Frame is a section of a Texture.
 *
 * @class Frame
 * @since 0.0.0
 */
class Frame
{
public:
	/**
	 * @since 0.0.0
	 *
	 * @param texture_ The Texture this Frame is a part of.
	 * @param name_ The name of this Frame. The name is unique within the Texture.
	 * @param sourceIndex_ The index of the TextureSource that this Frame is a part of.
	 * @param x_ The x coordinate of the top-left of this Frame.
	 * @param y_ The y coordinate of the top-left of this Frame.
	 * @param width_ The width of this Frame.
	 * @param height_ The height of this Frame.
	 */
	Frame (Texture& texture_, std::string name_, int sourceIndex_, int x_, int y_, int width_, int height_);

	/**
	 * The Texture this Frame is a part of.
	 *
	 * @since 0.0.0
	 */
	Texture& texture;

	/**
	 * The name of this Frame.
	 * The name is unique within the Texture.
	 *
	 * @since 0.0.0
	 */
	std::string name;

	/**
	 * The TextureSource this Frame is part of.
	 *
	 * @since 0.0.0
	 */
	TextureSource& source;

	/**
	 * The index of the TextureSource in the Texture sources array.
	 *
	 * @since 0.0.0
	 */
	int sourceIndex;

	/**
	 * X position within the source image to cut from.
	 *
	 * @since 0.0.0
	 */
	int cutX;

	/**
	 * Y position within the source image to cut from.
	 *
	 * @since 0.0.0
	 */
	int cutY;

	/**
	 * The width of the area in the source image to cut.
	 *
	 * @since 0.0.0
	 */
	int cutWidth;

	/**
	 * The height of the area in the source image to cut.
	 *
	 * @since 0.0.0
	 */
	int cutHeight;

	/**
	 * The X rendering offset of this Frame, taking trim into account.
	 *
	 * @since 0.0.0
	 */
	int x = 0;

	/**
	 * The Y rendering offset of this Frame, taking trim into account.
	 *
	 * @since 0.0.0
	 */
	int y = 0;

	/**
	 * The rendering width of this Frame, taking trim into account.
	 *
	 * @since 0.0.0
	 */
	int width;

	/**
	 * The rendering height of this Frame, taking trim into account.
	 *
	 * @since 0.0.0
	 */
	int height;

	/**
	 * Half the width, floored.
	 * Precalculated for the renderer.
	 *
	 * @since 0.0.0
	 */
	int halfWidth;

	/**
	 * Half the height, floored.
	 * Precalculated for the renderer.
	 *
	 * @since 0.0.0
	 */
	int halfHeight;

	/**
	 * The x center of this frame, floored.
	 *
	 * @since 0.0.0
	 */
	int centerX;

	/**
	 * The y center of this frame, floored.
	 *
	 * @since 0.0.0
	 */
	int centerY;

	/**
	 * The horizontal pivot point of this Frame.
	 *
	 * @since 0.0.0
	 */
	int pivotX = 0;

	/**
	 * The vertical pivot point of this Frame.
	 *
	 * @since 0.0.0
	 */
	int pivotY = 0;

	/**
	 * Does this Frame have a custom pivot point?
	 *
	 * @since 0.0.0
	 */
	bool customPivot = false;

	/**
	 * **CURRENTLY UNSUPPORTED**
	 *
	 * Is this frame rotated or not in the Texture?
	 * Rotation allows you to use rotated frames in texture atlas packing.
	 * It has nothing to do with Sprite rotation.
	 *
	 * @since 0.0.0
	 */
	bool rotated = false;

	/**
	 * Any Frame specific custom data can be stored here.
	 *
	 * @since 0.0.0
	 */
	Data customData;

	/**
	 * OpenGL UV u0 value.
	 *
	 * @since 0.0.0
	 */
	double u0 = 0;

	/**
	 * OpenGL UV v0 value.
	 *
	 * @since 0.0.0
	 */
	double v0 = 0;

	/**
	 * OpenGL UV u1 value.
	 *
	 * @since 0.0.0
	 */
	double u1 = 0;

	/**
	 * OpenGL UV v1 value.
	 *
	 * @since 0.0.0
	 */
	double v1 = 0;

	/**
	 * Sets the width, height, x and y of this Frame.
	 *
	 * This is called automatically by the constructor
	 * and should rarely be changed on-the-fly.
	 *
	 * @since 0.0.0
	 *
	 * @param width_ The width of the frame before being trimmed.
	 * @param height_ The height of the frame before being trimmed.
	 * @param x_ The x coordinate of the top-left of this Frame.
	 * @param y_ The y coordinate of the top-left of this Frame.
	 *
	 * @return This Frame object.
	 */
	Frame& setSize (int width_, int height_, int x_ = 0, int y_ = 0);

	/**
	 * If the frame was trimmed when added to the Texture Atlas, this records
	 * the trim and source data.
	 *
	 * @since 0.0.0
	 *
	 * @param actualWidth_ The width of the frame before being trimmed.
	 * @param actualHeight_ The height of the frame before being trimmed.
	 * @param destX_ The destination X position of the trimmed frame for display.
	 * @param destY_ The destination Y position of the trimmed frame for display.
	 * @param destWidth_ The destination width of the trimmed frame for display.
	 * @param destHeight_ The destination height of the trimmed frame for display.
	 *
	 * @return This Frame object.
	 */
	Frame& setTrim (int actualWidth_, int actualHeight_, int destX_, int destY_, int destWidth_, int destHeight_);

	/**
	 * Takes a crop data object and, based on the rectangular region given,
	 * calculates the required UV coordinates in order to crop this Frame for
	 * OpenGL and Canvas rendering.
	 *
	 * This is called directly by the Game Object Texture Components `setCrop`
	 * method. Please use that method to crop a Game Object.
	 *
	 * @since 0.0.0
	 *
	 * @param crop_ The crop data object. This is the `GameObject.crop_` property.
	 * @param x_ The x coordinate to start the crop from. Cannot be negative or
	 * exceed the Frame width.
	 * @param y_ The y coordinate to start the crop from. Cannot be negative or
	 * exceed the Frame height.
	 * @param width_ The width of the crop rectangle. Cannot exceed the Frame width.
	 * @param height_ The height of the crop rectangle. Cannot exceed the Frame
	 * height.
	 * @param flipX_ Does the parent Game Object have flipX set?
	 * @param flipY_ Does the parent Game Object have flipY set?
	 *
	 * @return The updated crop data object.
	 */
	CropData setCropUVs (CropData crop_, int x_, int y_, int width_, int height_, bool flipX_, bool flipY_);

	/**
	 * Takes a crop data object and recalculates the UVs based on the dimensions
	 * inside the crop object.
	 * Called automatically by `setFrame`.
	 *
	 * @since 0.0.0
	 *
	 * @param crop_ The crop data object. This is the `GameObject.crop_` property.
	 * @param flipX_ Does the parent Game Object have flipX set?
	 * @param flipY_ Does the parent Game Object have flipY set?
	 *
	 * @return The updated crop data object.
	 */
	CropData updateCropUVs (CropData crop_, bool flipX_, bool flipY_);

	/**
	 * Directly sets the canvas and OpenGL UV data for this frame.
	 *
	 * Use this if you need to override the values that are generated automatically
	 * when the Frame is created.
	 *
	 * @since 0.0.0
	 *
	 * @param width_ Width of this frame for the Canvas data.
	 * @param height_ Height of this frame for the Canvas data.
	 * @param u0_ UV u0 value.
	 * @param v0_ UV v0 value.
	 * @param u1_ UV u1 value.
	 * @param v1_ UV v1 value.
	 *
	 * @return This Frame object.
	 */
	Frame& setUVs (int width_, int height_, double u0_, double v0_, double u1_, double v1_);

	/**
	 * Updates the internal WebGL UV cache and the drawImage cache.
	 *
	 * @since 0.0.0
	 *
	 * @return This Frame object.
	 */
	Frame& updateUVs ();

	/**
	 * Updates the internal WebGL UV cache.
	 *
	 * @since 0.0.0
	 *
	 * @return This Frame object.
	 */
	Frame& updateUVsInverted ();

	/**
	 * Clones this Frame into a new Frame object.
	 *
	 * @since 0.0.0
	 *
	 * @return A clone of this Frame.
	 */
	Frame& clone();

	/**
	 * The width of the Frame in its un-trimmed, un-padded state, as prepared
	 * in the art package, before being packed.
	 *
	 * @since 0.0.0
	 */
	int getRealWidth ();

	/**
	 * The height of the Frame in its un-trimmed, un-padded state, as prepared
	 * in the art package, before being packed.
	 *
	 * @since 0.0.0
	 */
	int getRealHeight ();

	/**
	 * The radius of the Frame (derived from sqrt(w * w + h * h) / 2)
	 *
	 * @since 0.0.0
	 */
	double getRadius ();

	/**
	 * Is the Frame trimmed or not?
	 *
	 * @since 0.0.0
	 */
	bool isTrimmed ();

	/**
	 * The drawImage data object.
	 *
	 * @since 0.0.0
	 */
	FrameDataDrawImage getDrawImageData ();

private:
	/**
	 * The un-modified source frame, trim and UV data.
	 *
	 * @since 0.0.0
	 */
	FrameData data = {
		{	// cut
			0,	// x
			0,	// y
			0,	// w
			0,	// h
			0,	// r
			0	// b
		},
		false,	// trim
		{	// sourceSize
			0,	// w
			0	// h
		},
		{	// spriteSourceSize
			0,	// x
			0,	// y
			0,	// w
			0,	// h
			0,	// r
			0	// b
		},
		0,	// radius
		{	// drawImage
			0,	// x
			0,	// y
			0,	// width
			0	// height
		}
	};
};	// class Frame

}	// namespace Textures
}	// namespace Zen

#endif
