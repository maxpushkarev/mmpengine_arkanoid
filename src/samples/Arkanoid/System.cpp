#include <Arkanoid/System.hpp>

namespace Sample::Arkanoid
{
	BaseSystem::BaseSystem(WorldService* world, const IConfigService* config) : _world(world), _config(config)
	{
	}

	BaseSystem::BaseSystem() = default;
	BaseSystem::~BaseSystem() = default;
}