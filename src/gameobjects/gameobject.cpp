/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "gameobject.h"

#include "../scene/scene.h"

namespace Zen {
namespace GameObjects {

GameObject::GameObject (Scene* scene_)
	: scene (scene_)
{
	on("added-to-scene", &GameObject::addedToScene, this);
	on("removed-from-scene", &GameObject::removedFromScene, this);

	// Tell the Scene to re-sort the children
	scene->sys.queueDepthSort();
}

GameObject::~GameObject ()
{
	destroy();
}

bool hasComponent (int compMask_)
{
	//return compMask_ | COMPONENT_MASK;
	return true;
}

void GameObject::setActive (bool value_)
{
	active = value_;
}

void GameObject::setName (std::string name_)
{
	name = name_;
}

void GameObject::setState (std::string state_)
{
	state = state_;
}

void GameObject::addedToScene ()
{}

void GameObject::removedFromScene ()
{}

void GameObject::preUpdate (Uint32 time_, Uint32 delta_)
{}

void GameObject::update (Uint32 time_, Uint32 delta_)
{}

bool GameObject::willRender (Cameras::Scene2D::Camera& camera_)
{
	return !(
		0b1111 != renderFlags ||
		cameraFilter != 0 &&
		(cameraFilter & camera_.id)
		);
}

std::vector<int> GameObject::getIndexList ()
{
	GameObject* child_ = this;
	GameObject* parent_ = parentContainer;

	std::vector<int> indices_;

	while (parent_)
	{
		indices_.emplace(indices_.begin(), parent_->getIndex(child_));

		child_ = parent_;

		if (!parent_->parentContainer)
		{
			break;
		}
		else
		{
			parent_ = parent_->parentContainer;
		}
	}

	if (displayList)
	{
		indices_.emplace(indices_.begin(), displayList->getIndex(child_));
	}
	else
	{
		indices_.emplace(indices_.begin(), scene->children.getIndex(child_));
	}

	return indices_;
}

int getIndex (GameObject* child_)
{
	return 0;
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
		displayList->queueDepthSort();
		displayList->remove(this);
	}

	/**
	 * @todo TODO input
	 */
	/*
	if (input)
	{
		scene->input.clear(this);
	}
	*/
}

}	// namespace GameObjects
}	// namespace Zen
