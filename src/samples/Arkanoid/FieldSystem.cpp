#include <Arkanoid/FieldSystem.hpp>
#include <Arkanoid/WorldService.hpp>
#include <Arkanoid/ConfigService.hpp>

namespace Sample::Arkanoid
{
	FieldSystem::FieldSystem(WorldService* world, const IConfigService* config) : BaseSystem(world, config)
	{
	}

	void FieldSystem::Update(std::float_t dt)
	{
		const auto& fields = this->ObjectSystem<FieldObject>::_objectsMap;
		const auto& rounds = this->ObjectSystem<RoundObject>::_objectsMap;

		if (rounds.empty())
		{
			return;
		}

		const auto round = rounds.begin()->second;

		if (round->state == RoundObject::State::Start)
		{
			if (fields.empty())
			{
				auto field = std::make_unique<FieldObject>();

				const auto& fieldSize = _config->GetFieldConfig().size;

				field->node = std::make_shared<MMPEngine::Core::Node>();
				field->node->localTransform.scale = { fieldSize.x, fieldSize.y, 1.0f };
				field->node->localTransform.position.z += 0.1f; //to guarantee correct depth order
				field->instanceData.color = _config->GetFieldConfig().color;

				_world->AddObject(std::move(field));
			}
		}
	}

}