/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_GROUP_H
#define ZEN_GAMEOBJECTS_GROUP_H

#include <iterator>
#include <vector>

#include "../gameobject.h"

namespace Zen {
namespace GameObjects {

/*
class Iterator
{
private:
	int value;

	class IntHolder
	{
	private:
		int value;
	public:
		IntHolder (int value_) : value (value_) {}
		int operator*() { return value; }
	};
public:
	// Iterator typedefs
	typedef int							value_type;
	typedef std::ptrdiff_t				difference_type;
	typedef int*						pointer;
	typedef int&						reference;
	typedef std::input_iterator_tag		iterator_category;

	explicit Iterator (int value_)
		: value (value_)
	{}
	
	int operator*() const
	{
		return value;
	}

	bool operator==(const Iterator& other_) const
	{
		return value == other_.value;
	}

	bool operator!=(const Iterator& other_) const
	{
		// Use the previously defined "==" operator
		return !(*this == other_);
	}

	// Prefix increment (++var)
	Iterator& operator++()
	{
		++value;				// Actual incrementation
		return *this;			// Return new value by reference
	}

	// Post increment (var++)
	IntHolder operator++(int)
	{
		IntHolder ret (value);	// Copy old value
		++*this;				// Prefix increment
		return ret;				// Return old value
	}
};
*/

class Group : public GameObject
{
public:
	std::vector<GameObjects::GameObject*> children;

	std::vector<GameObjects::GameObject*>::iterator begin ()
	{
		return children.begin();
	}

	std::vector<GameObjects::GameObject*>::iterator end ()
	{
		return children.end();
	}
};

}	// namespace GameObjects
}	// namespace Zen

#endif
