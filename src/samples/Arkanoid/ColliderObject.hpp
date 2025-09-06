#pragma once
#include <Arkanoid/GameObject.hpp>

namespace Sample::Arkanoid
{
	class ColliderObject : public virtual GameObject
	{
	};

	class Brick final : public ColliderObject
	{
	};

	class Wall final : public ColliderObject
	{
	};

	class Cart final : public ColliderObject
	{
	};

	class Ball final : public ColliderObject
	{
	public:
		MMPEngine::Core::Vector2Float movementDirection;
	};
}
