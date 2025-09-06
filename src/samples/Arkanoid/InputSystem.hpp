#pragma once
#include <Arkanoid/System.hpp>
#include <Arkanoid/RoundObject.hpp>
#include <Arkanoid/InputObject.hpp>
#include <Feature/Input.hpp>

namespace Sample::Arkanoid
{
	class InputSystem final : public MultiObjectSystem<InputObject, RoundObject>, public BaseSimulationSystem
	{
	public:
		InputSystem(WorldService* world, const IConfigService* config, const MMPEngine::Feature::Input* input);
		void Simulate(std::float_t dt) override;
	private:
		const MMPEngine::Feature::Input* const _input;
	};
}
