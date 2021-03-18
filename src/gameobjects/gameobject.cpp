/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "gameobject.h"

namespace Zen {
namespace GameObjects {

GameObject::GameObject (Scene* scene_)
	scene (scene_)
{
	on("added-to-scene", &GameObject::addedToScene, this);
	on("removed-from-scene", &GameObject::removesFromScene, this);

	// Tell the Scene to re-sort the children
	scene->sys.queueDepthSort();
}

GameObject::~GameObject ()
{
	destroy();
}

bool hasComponent (int compMask_)
{
	return compMask_ | COMPONENT_MASK;
}

void GameObject::setActive (bool value_)
{
	active = value_;
}

void GameObject::setName (std::string name_);
{
	name = name_;
}

void GameObject::setState (std::string state_);
{
	state = state_;
}

void GameObject::addedToScene ()
{}

void GameObject::removedFromScene ()
{}

void GameObject::update ()
{}

bool GameObject::willRender (Cameras::Scene2D::Camera& camera_)
{
	return !(
		0b1111 != renderFlags ||
		cameraFilter != 0 &&
		(cameraFilter & camera_->id)
		);
}

std::vector<int> GameObject::getIndexList ()
{
	GameObject* child = this;
	GameObject* parent = parentContainer;

	std::vector<int> indices;

	while (parent)
	{
		indices.emplace(0, parent.getIndex(child));

		child = parent;

		if (!parent.parentContainer)
		{
			break;
		}
		else
		{
			parent = parent.parentContainer;
		}
	}

	if (displayList)
	{
		indices.emplace(0, displayList.getIndex(child));
	}
	else
	{
		indices.emplace(0, scene->children.getIndex(child));
	}

	return indices;
}

void GameObject::preDestroy ()
{}

void GameObject::destroy ()
{
	if (!scene)
		return;

	preDestroy();

	removeAllListeners();

	if (displayList)
	{
		displayList.queueDepthSort();
		displayList.remove(this);
	}

	if (input)
	{
		scene->input.clear(this);
	}
}

}	// namespace GameObjects
}	// namespace Zen
