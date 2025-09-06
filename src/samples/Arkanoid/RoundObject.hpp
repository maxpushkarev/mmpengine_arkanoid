#pragma once
#include <Arkanoid/Object.hpp>

namespace Sample::Arkanoid
{
	class RoundObject : public BaseObject
	{
	public:
		enum class State : std::uint8_t
		{
			Start,
			Running,
			Finish
		};
		State state = State::Start;
	};
}
