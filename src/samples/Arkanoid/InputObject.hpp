#pragma once
#include <Arkanoid/Object.hpp>

namespace Sample::Arkanoid
{
	class InputObject : public BaseObject
	{
	public:
		std::int8_t HorizontalAxisMovementDirection = 0;
	};
}