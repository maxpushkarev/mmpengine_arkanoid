#include <Arkanoid/ColliderSystem.hpp>
#include <Arkanoid/WorldService.hpp>
#include <Arkanoid/ConfigService.hpp>
#include <random>

namespace Sample::Arkanoid
{
	ColliderSystem::ColliderSystem(WorldService* world, const IConfigService* config) : BaseSystem(world, config)
	{
	}

	void ColliderSystem::Simulate(std::float_t)
	{
		const auto& colliders = this->ObjectSystem<ColliderObject>::_objectsMap;
		const auto& rounds = this->ObjectSystem<RoundObject>::_objectsMap;
		const auto& walls = this->ObjectSystem<Wall>::_objectsMap;
		const auto& bricks = this->ObjectSystem<Brick>::_objectsMap;

		if (rounds.empty())
		{
			return;
		}

		const auto round = rounds.begin()->second;

		if (round->state == RoundObject::State::Start)
		{
			
			const auto fieldCfg = _config->GetFieldConfig();
			const auto& wallCfg = _config->GetWallConfig();

			auto ceil = std::make_unique<Wall>();
			ceil->node = std::make_shared<MMPEngine::Core::Node>();
			ceil->node->localTransform.position = { 0.0f, fieldCfg.size.y * 0.5f + wallCfg.thickness * 0.5f, 0.0f };
			ceil->node->localTransform.scale = { fieldCfg.size.x, wallCfg.thickness, 1.0f };

			auto leftWall = std::make_unique<Wall>();
			leftWall->node = std::make_shared<MMPEngine::Core::Node>();
			leftWall->node->localTransform.position = { -fieldCfg.size.x * 0.5f - wallCfg.thickness * 0.5f, wallCfg.thickness * 0.5f, 0.0f };
			leftWall->node->localTransform.scale = { wallCfg.thickness, fieldCfg.size.y + wallCfg.thickness, 1.0f };

			auto rightWall = std::make_unique<Wall>();
			rightWall->node = std::make_shared<MMPEngine::Core::Node>();
			rightWall->node->localTransform.position = { +fieldCfg.size.x * 0.5f + wallCfg.thickness * 0.5f, wallCfg.thickness * 0.5f, 0.0f };
			rightWall->node->localTransform.scale = { wallCfg.thickness, fieldCfg.size.y + wallCfg.thickness, 1.0f };


			ceil->instanceData.color = wallCfg.color;
			leftWall->instanceData.color = wallCfg.color;
			rightWall->instanceData.color = wallCfg.color;

			_world->AddObject(std::move(ceil));
			_world->AddObject(std::move(leftWall));
			_world->AddObject(std::move(rightWall));
			

			if (bricks.empty())
			{
				const auto& brickCfg = _config->GetBrickConfig();

				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<std::uint32_t> dist(0U, static_cast<std::uint32_t>(brickCfg.colors.size() - 1));

				auto leftFieldX = -fieldCfg.size.x * 0.5f;
				auto rightFieldX = +fieldCfg.size.x * 0.5f;
				auto leftBrickX = leftFieldX;

				while (true)
				{
					const auto rightBrickX = leftBrickX + brickCfg.size.x;
					if (rightBrickX > rightFieldX)
					{
						break;
					}

					const auto brickX = (leftBrickX + rightBrickX) * 0.5f;

					for (auto row = 0U; row < brickCfg.rows; ++row)
					{
						const auto brickY = fieldCfg.size.y * 0.5f - brickCfg.size.y * 0.5f - (static_cast<std::float_t>(row) * brickCfg.size.y);

						auto brick = std::make_unique<Brick>();
						brick->hit = false;
						brick->node = std::make_shared<MMPEngine::Core::Node>();
						brick->node->localTransform.position = { brickX, brickY, 0.0f };
						brick->node->localTransform.scale ={ brickCfg.size.x, brickCfg.size.y };
						brick->instanceData.color = brickCfg.colors.at(dist(gen));

						_world->AddObject(std::move(brick));
					}

					leftBrickX = rightBrickX;
				}
			}
		}
	}

	void ColliderSystem::Update(std::float_t dt)
	{
		const auto& bricks = this->ObjectSystem<Brick>::_objectsMap;


		for (auto it = bricks.begin(); it != bricks.end();)
		{
			auto current = it;
			it = std::next(it);
			if (current->second->hit)
			{
				_world->RemoveObject(current->second->GetId());
			}
		}
	}

}