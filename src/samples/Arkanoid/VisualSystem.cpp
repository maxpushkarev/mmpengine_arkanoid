#include <Arkanoid/VisualSystem.hpp>
#include <Arkanoid/WorldService.hpp>
#include <Arkanoid/ConfigService.hpp>

namespace Sample::Arkanoid
{
	VisualSystem::VisualSystem(WorldService* world, const IConfigService* config, const std::shared_ptr<MMPEngine::Core::GlobalContext>& globalContext)
		: BaseSystem(world, config), _globalContext(globalContext)
	{
		
	}

	void VisualSystem::Update(std::float_t dt)
	{
		const auto& visualObjects = this->_objectsMap;

		for (const auto& vo : visualObjects)
		{
			_globalContext->math->CalculateLocalToWorldSpaceMatrix(vo.second->instanceData.local2World, vo.second->node);
		}
	}

}