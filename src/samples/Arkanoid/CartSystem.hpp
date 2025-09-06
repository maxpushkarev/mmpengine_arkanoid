#pragma once
#include <Arkanoid/System.hpp>
#include <Arkanoid/ColliderObject.hpp>
#include <Arkanoid/FieldObject.hpp>
#include <Arkanoid/RoundObject.hpp>
#include <Arkanoid/InputObject.hpp>

namespace Sample::Arkanoid
{
	class CartSystem final : public MultiObjectSystem<Cart, RoundObject, InputObject>, public BaseSimulationSystem
	{
	public:
		CartSystem(WorldService* world, const IConfigService* config);
		void Simulate(std::float_t dt) override;
	};
}
