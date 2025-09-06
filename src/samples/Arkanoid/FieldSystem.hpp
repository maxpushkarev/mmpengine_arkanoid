#pragma once
#include <Arkanoid/System.hpp>
#include <Arkanoid/RoundObject.hpp>
#include <Arkanoid/FieldObject.hpp>

namespace Sample::Arkanoid
{
	class FieldSystem final : public MultiObjectSystem<FieldObject, RoundObject>, public BaseUpdateSystem
	{
	public:
		FieldSystem(WorldService* world, const IConfigService* config);
		void Update(std::float_t dt) override;
	};
}
