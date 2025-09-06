#pragma once
#include <Arkanoid/System.hpp>
#include <Arkanoid/RoundObject.hpp>
#include <Arkanoid/ColliderObject.hpp>

namespace Sample::Arkanoid
{
	class ColliderSystem final : public MultiObjectSystem<ColliderObject, Wall, Brick, RoundObject>, public BaseSimulationSystem, public BaseUpdateSystem
	{
	public:
		ColliderSystem(WorldService* world, const IConfigService* config);
		void Simulate(std::float_t dt) override;
		void Update(std::float_t dt) override;
	};
}
