#pragma once
#include <Arkanoid/Object.hpp>
#include <Core/Node.hpp>

namespace Sample::Arkanoid
{
	class GameObject : public BaseObject
	{
	public:
		std::shared_ptr<MMPEngine::Core::Node> node;
	};
}
