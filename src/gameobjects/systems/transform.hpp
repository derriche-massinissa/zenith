/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_SYSTEMS_TRANSFORM_HPP
#define ZEN_GAMEOBJECTS_SYSTEMS_TRANSFORM_HPP

#include "../components/transform_matrix.h"
#include "../../math/vector2.h"

namespace Zen {

void SetScale (Entity entity, double value);

double GetScale (Entity entity);

void SetScaleX (Entity entity, double value);

double GetScaleX (Entity entity);

void SetScaleY (Entity entity, double value);

double GetScaleY (Entity entity);

void SetAngle (Entity entity, double value);

double GetAngle (Entity entity);

void SetRotation (Entity entity, double value = 0.0);

double GetRotation (Entity entity);

void SetPosition (Entity entity, double x = 0., double y = 0., double z = 0., double w = 0.);

void SetPosition (Entity entity, Math::Vector2 source);

/**
 * TODO
 * @todo Vector3 & Vector4
 void CopyPosition (Entity entity, Math::Vector3 source);

 void CopyPosition (Entity, entity, Math::Vector4 source);
 */

void SetX (Entity entity, double value = 0.);

void SetY (Entity entity, double value = 0.);

void SetZ (Entity entity, double value = 0.);

void SetW (Entity entity, double value = 0.);

double GetX (Entity entity);

double GetY (Entity entity);

double GetZ (Entity entity);

double GetW (Entity entity);

TransformMatrix GetLocalTransformMatrix (Entity entity);

TransformMatrix GetWorldTransformMatrix (Entity entity);

Math::Vector2 GetLocalPoint (Entity entity, double x, double y, Entity camera = entt::null);

double GetParentRotation (Entity entity);

}	// namespace Zen

#endif
