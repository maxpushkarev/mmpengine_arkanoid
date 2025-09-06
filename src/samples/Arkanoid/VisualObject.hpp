#pragma once
#include <Arkanoid/GameObject.hpp>

namespace Sample::Arkanoid
{
	class VisualObject : public virtual GameObject
	{
	public:

		struct InstanceData final
		{
			MMPEngine::Core::Matrix4x4 local2World;
			MMPEngine::Core::Vector4Float color;
		};

		InstanceData instanceData {};
	};
}
