#include <Arkanoid/InputSystem.hpp>
#include <Arkanoid/WorldService.hpp>
#include <Arkanoid/ConfigService.hpp>

namespace Sample::Arkanoid
{
	InputSystem::InputSystem(WorldService* world, const IConfigService* config, const MMPEngine::Feature::Input* input) : BaseSystem(world, config), _input(input)
	{
	}

	void InputSystem::Simulate(std::float_t)
	{
		const auto& inputs = this->ObjectSystem<InputObject>::_objectsMap;
		const auto& rounds = this->ObjectSystem<RoundObject>::_objectsMap;

		if (rounds.empty())
		{
			return;
		}

		const auto round = rounds.begin()->second;

		if (round->state == RoundObject::State::Start)
		{
			if (inputs.empty())
			{
				_world->AddObject(std::make_unique<InputObject>());
			}
		}

		if (inputs.empty())
		{
			return;
		}

		std::int8_t val = 0;

		if (_input->IsButtonPressed(MMPEngine::Feature::KeyButton::A) || _input->IsButtonPressed(MMPEngine::Feature::KeyButton::LeftArrow))
		{
			val = -1;
		}

		if (_input->IsButtonPressed(MMPEngine::Feature::KeyButton::D) || _input->IsButtonPressed(MMPEngine::Feature::KeyButton::RightArrow))
		{
			val = +1;
		}

		inputs.begin()->second->HorizontalAxisMovementDirection = val;
	}
}