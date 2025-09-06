#pragma once
#include <Arkanoid/RoundObject.hpp>
#include <Arkanoid/ColliderObject.hpp>
#include <Arkanoid/System.hpp>

namespace Sample::Arkanoid
{
	class RoundSystem final : public MultiObjectSystem<RoundObject, BaseObject, Ball>, public BaseSimulationSystem
	{
	public:
		RoundSystem(WorldService* world, const IConfigService* config);
		void Simulate(std::float_t dt) override;
	};
}
