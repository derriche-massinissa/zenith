/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_COMPONENTS_TRANSFORM_H
#define ZEN_GAMEOBJECTS_COMPONENTS_TRANSFORM_H

#define FLAG 4 // 0100

#include "../../const.h"
#include "../../math/const.h"
#include "transform_matrix.h"
#include "../../math/transform_xy.h"
#include "../../math/angle/wrap.h"
#include "../../math/angle/wrap_degrees.h"
#include "../../math/vector2.h"
#include "../../cameras/2d/camera.h"

namespace Zen {
namespace GameObjects {
namespace Components {

template <typename T>
class Transform
{
public:
	/**
	 * Returns a cast `this` pointer, with the type of the descendant class.
	 *
	 * @since 0.0.0
	 * @return Pointer to this instance of T.
	 */
	T* This ()
	{
		return static_cast<T*>(this);
	}

	T& setScale (double value_)
	{
		scaleX = value_;
		scaleY = value_;

		if (value_ == 0)
		{
			This()->renderFlags &= ~FLAG;
		}
		else
		{
			This()->renderFlags |= FLAG;
		}
		return *This();
	}

	double getScale ()
	{
		return (scaleX + scaleY) / 2.0;
	}

	T& setScaleX (double value_)
	{
		scaleX = value_;

		if (value_ == 0)
		{
			This()->renderFlags &= ~FLAG;
		}
		else
		{
			This()->renderFlags |= FLAG;
		}

		return *This();
	}

	double getScaleX ()
	{
		return scaleX;
	}

	T& setScaleY (double value_)
	{
		scaleY = value_;

		if (value_ == 0)
		{
			This()->renderFlags &= ~FLAG;
		}
		else
		{
			This()->renderFlags |= FLAG;
		}

		return *This();
	}

	double getScaleY ()
	{
		return scaleY;
	}

	T& setAngle (double value_)
	{
		rotation = Math::Angle::wrapDegrees(value_ * Math::DEG_TO_RAD);

		return *This();
	}

	double getAngle ()
	{
		return Math::Angle::wrapDegrees(rotation * Math::RAD_TO_DEG);
	}

	T& setRotation (double value_ = 0.0)
	{
		rotation = Math::Angle::wrap(value_);

		return *This();
	}

	double getRotation ()
	{
		return rotation;
	}

	T& setPosition (int x_ = 0, int y_ = 0, int z_ = 0, int w_ = 0)
	{
		x = x_;
		y = y_;
		z = z_;
		w = w_;

		return *This();
	}

	T& setPosition (int x_)
	{
		return setPosition(x_, x_, 0, 0);
	}

	T& copyPosition (Math::Vector2 source_)
	{
		return setPosition(source_.x, source_.y);
	}

	/**
	 * TODO
	 * @todo Vector3 & Vector4
	T& copyPosition (Math::Vector3 source_)
	{
		return setPosition(source_.x, source_.y, source_.z);
	}

	T& copyPosition (Math::Vector4 source_)
	{
		return setPosition(source_.x, source_.y, source_.z, source_.w);
	}
	*/

	T& setRandomPosition (int x_ = 0, int y_ = 0, int width_ = 0, int height_ = 0)
	{
		if (width_ == 0)
			width_ = This()->scene->scale->width;

		if (height_ == 0)
			height_ = This()->scene->scale->height;

		x = Math::random.between(x_, width_);
		y = Math::random.between(y_, height_);

		return *This();
	}

	T& setX (int value_ = 0)
	{
		x = value_;

		return *This();
	}

	T& setY (int value_ = 0)
	{
		y = value_;

		return *This();
	}

	T& setZ (int value_ = 0)
	{
		z = value_;

		return *This();
	}

	T& setW (int value_ = 0)
	{
		w = value_;

		return *This();
	}

	int getX ()
	{
		return x;
	}

	int getY ()
	{
		return y;
	}

	int getZ ()
	{
		return z;
	}

	int getW ()
	{
		return w;
	}

	TransformMatrix getLocalTransformMatrix ()
	{
		TransformMatrix out;

		return out.applyITRS(x, y, rotation, scaleX, scaleY);
	}

	TransformMatrix getWorldTransformMatrix ()
	{
		GameObject* parent_ = This()->parentContainer;

		if (!parent)
		{
			return getLocalTransformMatrix();
		}

		TransformMatrix out_, parentMatrix_;

		out_.applyITRS(x, y, rotation, scaleX, scaleY);

		while (parent_)
		{
			parentMatrix_.applyITRS(parent_->x, parent_->y, parent_->rotation, parent_->scaleX, parent_->scaleY);

			out_ = parentMatrix_.multiply(out_);

			parent_ = parent_->parentContainer;
		}

		return out_;
	}

	Math::Vector2 getLocalPoint (int x_, int y_, Camera* camera_ = nullptr)
	{
		Math::Vector2 out;

		if (!camera_)
			camera_ = This()->scene->cameras.main;

		int csx_ = camera_.getScrollX;
		int csy_ = camera_.getScrollY;

		int px_ = x_ + (csx_ * This()->getScrollFactorX()) - csx_;
		int py_ = y_ + (csy_ * This()->getScrollFactorY()) - csy_;

		if (This()->parentContainer)
		{
			out = getWorldTransformMatrix().applyInverse(px_, py_);
		}
		else
		{
			out = transformXY(px_, py_, x, y, rotation, scaleX, scaleY);
		}

		// Normalize origin
		if constexpr (This()->hasComponent(COMPONENT_MASK_ORIGIN))
		{
			out.setX(out.x + This()->getDisplayOriginX());
			out.setY(out.y + This()->getDisplayOriginY());
		}

		return out;
	}

	getParentRotation ()
	{
		double rotation_ = 0;
		GameObject* parent_ = This()->parentContainer;

		while (parent_)
		{
			rotation_ += parent_->rotation;
			parent_ = parent_->parentContainer;
		}

		return rotation_;
	}

private:
	double scaleX = 1.0;
	double scaleY = 1.0;
	double rotation = 0.0;
	int x = 0;
	int y = 0;
	int z = 0;
	int w = 0;
};

}	// namespace Components
}	// namespace GameObjects
}	// namespace Zen

#undef FLAG

#endif
