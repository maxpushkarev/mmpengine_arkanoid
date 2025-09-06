#pragma once
#include <Arkanoid/GameObject.hpp>
#include <Arkanoid/VisualObject.hpp>

namespace Sample::Arkanoid
{
	struct CollisionInfo final
	{
		MMPEngine::Core::Vector3Float Normal;
		MMPEngine::Core::Vector3Float Position;
	};

	class ColliderObject : public virtual GameObject
	{
	};

	class Brick final : public ColliderObject, public VisualObject
	{
	public:
		bool hit = false;
	};

	class Wall final : public ColliderObject, public VisualObject
	{
	};

	class Cart final : public ColliderObject, public VisualObject
	{
	};

	class Ball final : public ColliderObject, public VisualObject
	{
	public:
		MMPEngine::Core::Vector3Float movementDirection;
	};
}
