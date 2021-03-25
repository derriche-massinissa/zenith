/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_SYSTEMS_FRAME_HPP
#define ZEN_TEXTURES_SYSTEMS_FRAME_HPP

#include "../../ecs/entity.hpp"
#include "../../geom/rectangle.h"
#include "../frame_data.h"
#include "../crop_data.h"

namespace Zen {

CreateFrame (Entity texture, const char* name, Entity source, int x, int y, int width, int height);

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
void SetFrameSize (Entity frame, int width, int height, int x = 0, int y = 0);

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
void SetFrameTrim (Entity frame, int actualWidth, int actualHeight, int destX, int destY, int destWidth, int destHeight);

/**
 * Takes a crop data object and, based on the rectangular region given,
 * calculates the required UV coordinates in order to crop this Frame.
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
CropData SetFrameCropUVs (Entity frame, CropData crop, int x, int y, int width, int height, bool flipX, bool flipY);

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
CropData UpdateFrameCropUVs (Entity frame, CropData crop, bool flipX, bool flipY);

/**
 * Directly sets UV data for this frame.
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
void SetFrameUVs (Entity frame, int width, int height, double u0, double v0, double u1, double v1);

/**
 * Updates the internal WebGL UV cache and the drawImage cache.
 *
 * @since 0.0.0
 *
 * @return This Frame object.
 */
void UpdateFrameUVs (Entity frame);

/**
 * Updates the internal WebGL UV cache.
 *
 * @since 0.0.0
 *
 * @return This Frame object.
 */
void UpdateFrameUVsInverted (Entity frame);

/**
 * The width of the Frame in its un-trimmed, un-padded state, as prepared
 * in the art package, before being packed.
 *
 * @since 0.0.0
 */
int GetFrameRealWidth (Entity frame);

/**
 * The height of the Frame in its un-trimmed, un-padded state, as prepared
 * in the art package, before being packed.
 *
 * @since 0.0.0
 */
int GetFrameRealHeight (Entity frame);

/**
 * The radius of the Frame (derived from sqrt(w * w + h * h) / 2)
 *
 * @since 0.0.0
 */
double GetFrameRadius (Entity frame);

/**
 * Is the Frame trimmed or not?
 *
 * @since 0.0.0
 */
bool IsFrameTrimmed (Entity frame);

/**
 * The drawImage data object.
 *
 * @since 0.0.0
 */
Geom::Rectangle GetFrameDrawImageData (Entity frame);

}	// namespace Zen

#endif
