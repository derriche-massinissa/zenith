/**
 * @file		frame.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef FRAME_H
#define FRAME_H

#include <string>
#include <cmath>
#include "frame_data.h"
#include "../math.h"

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
	 * @param texture The Texture this Frame is a part of.
	 * @param name The name of this Frame. The name is unique within the Texture.
	 * @param sourceIndex The index of the TextureSource that this Frame is a part of.
	 * @param x The x coordinate of the top-left of this Frame.
	 * @param y The y coordinate of the top-left of this Frame.
	 * @param width The width of this Frame.
	 * @param height The height of this Frame.
	 */
	Frame (Texture& texture, std::string name, int sourceIndex, int x, int y, int width, int height);

	/**
	 * The Texture this Frame is a part of.
	 *
	 * @since 0.0.0
	 */
	Texture& texture_;

	/**
	 * The name of this Frame.
	 * The name is unique within the Texture.
	 *
	 * @since 0.0.0
	 */
	std::string name_;

	/**
	 * The TextureSource this Frame is part of.
	 *
	 * @since 0.0.0
	 */
	TextureSource& source_;

	/**
	 * The index of the TextureSource in the Texture sources array.
	 *
	 * @since 0.0.0
	 */
	int sourceIndex_;

	/**
	 * X position within the source image to cut from.
	 *
	 * @since 0.0.0
	 */
	int cutX_;

	/**
	 * Y position within the source image to cut from.
	 *
	 * @since 0.0.0
	 */
	int cutY_;

	/**
	 * The width of the area in the source image to cut.
	 *
	 * @since 0.0.0
	 */
	int cutWidth_;

	/**
	 * The height of the area in the source image to cut.
	 *
	 * @since 0.0.0
	 */
	int cutHeight_;

	/**
	 * The X rendering offset of this Frame, taking trim into account.
	 *
	 * @since 0.0.0
	 */
	int x_ = 0;

	/**
	 * The Y rendering offset of this Frame, taking trim into account.
	 *
	 * @since 0.0.0
	 */
	int y_ = 0;

	/**
	 * The rendering width of this Frame, taking trim into account.
	 *
	 * @since 0.0.0
	 */
	int width_;

	/**
	 * The rendering height of this Frame, taking trim into account.
	 *
	 * @since 0.0.0
	 */
	int height_;

	/**
	 * Half the width, floored.
	 * Precalculated for the renderer.
	 *
	 * @since 0.0.0
	 */
	int halfWidth_;

	/**
	 * Half the height, floored.
	 * Precalculated for the renderer.
	 *
	 * @since 0.0.0
	 */
	int halfHeight_;

	/**
	 * The x center of this frame, floored.
	 *
	 * @since 0.0.0
	 */
	int centerX_;

	/**
	 * The y center of this frame, floored.
	 *
	 * @since 0.0.0
	 */
	int centerY_;

	/**
	 * The horizontal pivot point of this Frame.
	 *
	 * @since 0.0.0
	 */
	int pivotX_ = 0;

	/**
	 * The vertical pivot point of this Frame.
	 *
	 * @since 0.0.0
	 */
	int pivotY_ = 0;

	/**
	 * Does this Frame have a custom pivot point?
	 *
	 * @since 0.0.0
	 */
	bool customPivot_ = false;

	/**
	 * **CURRENTLY UNSUPPORTED**
	 *
	 * Is this frame is rotated or not in the Texture?
	 * Rotation allows you to use rotated frames in texture atlas packing.
	 * It has nothing to do with Sprite rotation.
	 *
	 * @since 0.0.0
	 */
	bool rotated_ = false;

	/**
	 * Any Frame specific custom data can be stored here.
	 *
	 * @since 0.0.0
	 */
	Data customData_;

	/**
	 * OpenGL UV u0 value.
	 *
	 * @since 0.0.0
	 */
	double u0_ = 0;

	/**
	 * OpenGL UV v0 value.
	 *
	 * @since 0.0.0
	 */
	double v0_ = 0;

	/**
	 * OpenGL UV u1 value.
	 *
	 * @since 0.0.0
	 */
	double u1_ = 0;

	/**
	 * OpenGL UV v1 value.
	 *
	 * @since 0.0.0
	 */
	double v1_ = 0;

	/**
	 * Sets the width, height, x and y of this Frame.
	 *
	 * This is called automatically by the constructor
	 * and should rarely be changed on-the-fly.
	 *
	 * @since 0.0.0
	 *
	 * @param width The width of the frame before being trimmed.
	 * @param height The height of the frame before being trimmed.
	 * @param x The x coordinate of the top-left of this Frame.
	 * @param y The y coordinate of the top-left of this Frame.
	 *
	 * @return This Frame object.
	 */
	Frame& setSize (int width, int height, int x = 0, int y = 0);

	/**
	 * If the frame was trimmed when added to the Texture Atlas, this records
	 * the trim and source data.
	 *
	 * @since 0.0.0
	 *
	 * @param actualWidth The width of the frame before being trimmed.
	 * @param actualHeight The height of the frame before being trimmed.
	 * @param destX The destination X position of the trimmed frame for display.
	 * @param destY The destination Y position of the trimmed frame for display.
	 * @param destWidth The destination width of the trimmed frame for display.
	 * @param destHeight The destination height of the trimmed frame for display.
	 *
	 * @return This Frame object.
	 */
	Frame& setTrim (int actualWidth, int actualHeight, int destX, int destY, int destWidth, int destHeight);

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
	 * @param crop The crop data object. This is the `GameObject.crop_` property.
	 * @param x The x coordinate to start the crop from. Cannot be negative or
	 * exceed the Frame width.
	 * @param y The y coordinate to start the crop from. Cannot be negative or
	 * exceed the Frame height.
	 * @param width The width of the crop rectangle. Cannot exceed the Frame width.
	 * @param height The height of the crop rectangle. Cannot exceed the Frame
	 * height.
	 * @param flipX Does the parent Game Object have flipX set?
	 * @param flipY Does the parent Game Object have flipY set?
	 *
	 * @return The updated crop data object.
	 */
	CropData setCropUVs (CropData crop, int x, int y, int width, int height, bool flipX, bool flipY);

	/**
	 * Takes a crop data object and recalculates the UVs based on the dimensions
	 * inside the crop object.
	 * Called automatically by `setFrame`.
	 *
	 * @since 0.0.0
	 *
	 * @param crop The crop data object. This is the `GameObject.crop_` property.
	 * @param flipX Does the parent Game Object have flipX set?
	 * @param flipY Does the parent Game Object have flipY set?
	 *
	 * @return The updated crop data object.
	 */
	CropData updateCropUVs (CropData crop, bool flipX, bool flipY);

	/**
	 * Directly sets the canvas and OpenGL UV data for this frame.
	 *
	 * Use this if you need to override the values that are generated automatically
	 * when the Frame is created.
	 *
	 * @since 0.0.0
	 *
	 * @param width Width of this frame for the Canvas data.
	 * @param height Height of this frame for the Canvas data.
	 * @param u0 UV u0 value.
	 * @param v0 UV v0 value.
	 * @param u1 UV u1 value.
	 * @param v1 UV v1 value.
	 *
	 * @return This Frame object.
	 */
	Frame& setUVs (int width, int height, double u0, double v0, double u1, double v1);

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
	FrameData data_ = {
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
