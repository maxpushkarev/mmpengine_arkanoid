#pragma once
#include <Arkanoid/System.hpp>
#include <Arkanoid/ColliderObject.hpp>
#include <Arkanoid/RoundObject.hpp>
#include <Core/Context.hpp>

namespace Sample::Arkanoid
{
	class BallSystem final : public MultiObjectSystem<Ball, RoundObject, ColliderObject, Cart, Brick>, public BaseSimulationSystem
	{
	public:
		BallSystem(WorldService* world, const IConfigService* config, const std::shared_ptr<MMPEngine::Core::GlobalContext>& globalContext);
		void Simulate(std::float_t dt) override;
	private:
		static bool CheckLineIntersection(const MMPEngine::Core::Vector3Float& p1, const MMPEngine::Core::Vector3Float& p2, const MMPEngine::Core::Vector3Float& q1, const MMPEngine::Core::Vector3Float& q2, CollisionInfo& res);
		static bool CheckCollision(const MMPEngine::Core::Vector3Float& from, const MMPEngine::Core::Vector3Float& to, const MMPEngine::Core::Vector3Float& collisionNrm);
		static bool CheckCollision(const MMPEngine::Core::Vector3Float& from, const MMPEngine::Core::Vector3Float& to, const ColliderObject* collider, CollisionInfo& res);
	private:
		std::shared_ptr<MMPEngine::Core::GlobalContext> _globalContext;
	};
}
