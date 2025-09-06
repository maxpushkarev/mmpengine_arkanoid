#include <Arkanoid/RoundSystem.hpp>
#include <Arkanoid/WorldService.hpp>
#include <Arkanoid/ConfigService.hpp>

namespace Sample::Arkanoid
{
	RoundSystem::RoundSystem(WorldService* world, const IConfigService* config) : BaseSystem(world, config)
	{
	}

	void RoundSystem::Simulate(std::float_t dt)
	{
		const auto& objects = this->ObjectSystem<BaseObject>::_objectsMap;
		const auto& rounds = this->ObjectSystem<RoundObject>::_objectsMap;

		if (rounds.empty())
		{
			_world->AddObject(std::make_unique<RoundObject>());
			rounds.begin()->second->state = RoundObject::State::Start;
			return;
		}

		auto round = rounds.begin()->second;
		if (round->state == RoundObject::State::Start)
		{
			round->state = RoundObject::State::Running;
			return;
		}

		if (round->state == RoundObject::State::Running)
		{
			//TODO: check finish
			return;
		}

		if (round->state == RoundObject::State::Finish)
		{
			for (auto it = objects.begin(); it != objects.end();)
			{
				auto current = it;
				it = std::next(it);
				_world->RemoveObject(current->second->GetId());
			}
		}
	}
}
