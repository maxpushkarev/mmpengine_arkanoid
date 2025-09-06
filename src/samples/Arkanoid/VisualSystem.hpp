#pragma once
#include <Arkanoid/VisualObject.hpp>
#include <Arkanoid/System.hpp>
#include <Core/Context.hpp>

namespace Sample::Arkanoid
{
	class VisualSystem final : public ObjectSystem<VisualObject>, public BaseUpdateSystem
	{
	public:
		VisualSystem(WorldService* world, const IConfigService* config, const std::shared_ptr<MMPEngine::Core::GlobalContext>& globalContext);
		void Update(std::float_t dt) override;
	private:
		std::shared_ptr<MMPEngine::Core::GlobalContext> _globalContext;
	};
}
