#include <random>
#include <Arkanoid/BallSystem.hpp>
#include <Arkanoid/WorldService.hpp>
#include <Arkanoid/ConfigService.hpp>

namespace Sample::Arkanoid
{
	BallSystem::BallSystem(WorldService* world, const IConfigService* config, const std::shared_ptr<MMPEngine::Core::GlobalContext>& globalContext) : BaseSystem(world, config), _globalContext(globalContext)
	{

	}

	void BallSystem::Simulate(std::float_t dt)
	{
		const auto& balls = this->ObjectSystem<Ball>::_objectsMap;
		const auto& bricks = this->ObjectSystem<Brick>::_objectsMap;
		const auto& carts = this->ObjectSystem<Cart>::_objectsMap;
		const auto& rounds = this->ObjectSystem<RoundObject>::_objectsMap;
		const auto& colliders = this->ObjectSystem<ColliderObject>::_objectsMap;

		if (rounds.empty())
		{
			return;
		}

		const auto round = rounds.begin()->second;

		const auto fieldCfg = _config->GetFieldConfig();
		const auto& cartCfg = _config->GetCartConfig();
		const auto& ballCfg = _config->GetBallConfig();

		if (round->state == RoundObject::State::Start)
		{
			if (!carts.empty() && balls.empty())
			{
				auto ball = std::make_unique<Ball>();
				ball->node = std::make_shared<MMPEngine::Core::Node>();
				ball->node->localTransform.position = { 0.0f, -fieldCfg.size.y * 0.5f + cartCfg.size.y + 2.0f * ballCfg.radius, 0.0f };
				ball->node->localTransform.scale = { ballCfg.radius * 2.0f, ballCfg.radius * 2.0f, 1.0f };

				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_real_distribution<std::float_t> dist(ballCfg.initialDirectionXMin, ballCfg.initialDirectionXMax);

				MMPEngine::Core::Vector3Float dir = { 0.0f, 1.0f, 0.0f };
				dir.x = dist(gen);

				const auto rndDir = dist(gen);
				if (rndDir >= 0.5f * (ballCfg.initialDirectionXMin + ballCfg.initialDirectionXMax))
				{
					dir.x *= -1.0f;
				}

				_globalContext->math->Normalize(dir);
				ball->movementDirection = dir;
				ball->instanceData.color = ballCfg.color;

				_world->AddObject(std::move(ball));
			}

			return;
		}

		if (round->state == RoundObject::State::Running)
		{
			CollisionInfo collisionInfo{};

			for (const auto& b : balls)
			{
				const auto currentBallPosition = b.second->node->localTransform.position;
				bool hasCollision = false;
				auto nextBallPosition = currentBallPosition;
				nextBallPosition.z = 0.0f;

				nextBallPosition.x += b.second->movementDirection.x * dt * ballCfg.speed;
				nextBallPosition.y += b.second->movementDirection.y * dt * ballCfg.speed;

				for (const auto& c : colliders)
				{
					if (b.second->GetId() == c.second->GetId())
					{
						continue;
					}

					if (CheckCollision(currentBallPosition, nextBallPosition, c.second, collisionInfo))
					{
						hasCollision = true;

						b.second->node->localTransform.position = collisionInfo.Position;

						const auto dot = _globalContext->math->Dot(b.second->movementDirection, collisionInfo.Normal);
						const MMPEngine::Core::Vector3Float nrmDot = { collisionInfo.Normal.x * dot, collisionInfo.Normal.y * dot, 0.0f };
						const MMPEngine::Core::Vector3Float proj = { b.second->movementDirection.x - nrmDot.x, b.second->movementDirection.y - nrmDot.y, 0.0f };

						b.second->movementDirection.x = -nrmDot.x + proj.x;
						b.second->movementDirection.y = -nrmDot.y + proj.y;

						_globalContext->math->Normalize(b.second->movementDirection);
						b.second->node->localTransform.position = collisionInfo.Position;
						b.second->node->localTransform.position.z = 0.0f;

						if (bricks.find(c.first) != bricks.cend())
						{
							bricks.at(c.first)->hit = true;
						}

						break;
					}
				}

				if (!hasCollision)
				{
					b.second->node->localTransform.position = nextBallPosition;
				}
			}
		}

	}

	bool BallSystem::CheckCollision(
		const MMPEngine::Core::Vector3Float& from,
		const MMPEngine::Core::Vector3Float& to,
		const MMPEngine::Core::Vector3Float& collisionNrm)
	{
		const auto dot = (to.x - from.x) * collisionNrm.x + (to.y - from.y) * collisionNrm.y;
		return dot < 0;
	}

	bool BallSystem::CheckLineIntersection(
		const MMPEngine::Core::Vector3Float& p1,
		const MMPEngine::Core::Vector3Float& p2,
		const MMPEngine::Core::Vector3Float& q1,
		const MMPEngine::Core::Vector3Float& q2,
		CollisionInfo& res
	)
	{
		const auto denom = (p1.x - p2.x) * (q1.y - q2.y) -
			(p1.y - p2.y) * (q1.x - q2.x);


		if (std::fabs(denom) <= 0.001f)
		{
			return false;
		}

		const auto t = ((p1.x - q1.x) * (q1.y - q2.y) -
			(p1.y - q1.y) * (q1.x - q2.x)) / denom;

		const auto u = ((p1.x - q1.x) * (p1.y - p2.y) -
			(p1.y - q1.y) * (p1.x - p2.x)) / denom;

		if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
			res.Position = {
				p1.x + t * (p2.x - p1.x),
				p1.y + t * (p2.y - p1.y)
			};
			return true;
		}

		return false;
	}



	bool BallSystem::CheckCollision(
		const MMPEngine::Core::Vector3Float& from,
		const MMPEngine::Core::Vector3Float& to,
		const ColliderObject* collider,
		CollisionInfo& res
	)
	{
		const MMPEngine::Core::Vector3Float tl = { collider->node->localTransform.position.x - collider->node->localTransform.scale.x * 0.5f, collider->node->localTransform.position.y + collider->node->localTransform.scale.y * 0.5f, 0.0f };
		const MMPEngine::Core::Vector3Float tr = { collider->node->localTransform.position.x + collider->node->localTransform.scale.x * 0.5f, collider->node->localTransform.position.y + collider->node->localTransform.scale.y * 0.5f, 0.0f };
		const MMPEngine::Core::Vector3Float bl = { collider->node->localTransform.position.x - collider->node->localTransform.scale.x * 0.5f, collider->node->localTransform.position.y - collider->node->localTransform.scale.y * 0.5f, 0.0f };
		const MMPEngine::Core::Vector3Float br = { collider->node->localTransform.position.x + collider->node->localTransform.scale.x * 0.5f, collider->node->localTransform.position.y - collider->node->localTransform.scale.y * 0.5f, 0.0f };

		MMPEngine::Core::Vector3Float collisionNrm{};

		collisionNrm = { 0.0f, 1.0f, 0.0f };
		if (CheckLineIntersection(from, to, tl, tr, res) && CheckCollision(from, to, collisionNrm))
		{
			res.Normal = collisionNrm;
			return true;
		}

		collisionNrm = { 0.0f, -1.0f, 0.0f };
		if (CheckLineIntersection(from, to, bl, br, res) && CheckCollision(from, to, collisionNrm))
		{
			res.Normal = collisionNrm;
			return true;
		}

		collisionNrm = { -1.0f, 0.0f, 0.0f };
		if (CheckLineIntersection(from, to, tl, bl, res) && CheckCollision(from, to, collisionNrm))
		{
			res.Normal = collisionNrm;
			return true;
		}

		collisionNrm = { 1.0f, 0.0f, 0.0f };
		if (CheckLineIntersection(from, to, tr, br, res) && CheckCollision(from, to, collisionNrm))
		{
			res.Normal = collisionNrm;
			return true;
		}

		return false;
	}


}
