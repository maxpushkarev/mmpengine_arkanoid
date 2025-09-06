#include <Arkanoid/Object.hpp>

namespace Sample::Arkanoid
{
	BaseObject::BaseObject()
	{
		static IdType idCounter = {};
		_id = idCounter++;
	}

	BaseObject::~BaseObject() = default;

	BaseObject::IdType BaseObject::GetId() const
	{
		return _id;
	}
}