#include <Arkanoid/CartSystem.hpp>
#include <Arkanoid/WorldService.hpp>
#include <Arkanoid/ConfigService.hpp>

namespace Sample::Arkanoid
{
	CartSystem::CartSystem(WorldService* world, const IConfigService* config) : BaseSystem(world, config)
	{

	}

	void CartSystem::Simulate(std::float_t dt)
	{
		const auto& carts = this->ObjectSystem<Cart>::_objectsMap;
		const auto& rounds = this->ObjectSystem<RoundObject>::_objectsMap;
		const auto& inputs = this->ObjectSystem<InputObject>::_objectsMap;

		if (rounds.empty())
		{
			return;
		}

		const auto round = rounds.begin()->second;

		const auto fieldCfg = _config->GetFieldConfig();
		const auto& cartCfg = _config->GetCartConfig();

		if (round->state == RoundObject::State::Start)
		{
			if (carts.empty())
			{
				auto cart = std::make_unique<Cart>();
				cart->node = std::make_shared<MMPEngine::Core::Node>();
				cart->node->localTransform.position = { 0.0f, -fieldCfg.size.y * 0.5f + cartCfg.size.y * 0.5f, 0.0f };
				cart->node->localTransform.scale = { cartCfg.size.x, cartCfg.size.y, 1.0f };

				_world->AddObject(std::move(cart));
			}
		}

		if (!inputs.empty() && !carts.empty())
		{
			const auto input = inputs.begin()->second;
			for (const auto& c : carts)
			{
				const auto direction = static_cast<std::float_t>(input->HorizontalAxisMovementDirection);
				c.second->node->localTransform.position.x += direction * cartCfg.speed * dt;

				c.second->node->localTransform.position.x = (std::max)(c.second->node->localTransform.position.x, -fieldCfg.size.x * 0.5f + cartCfg.size.x * 0.5f);
				c.second->node->localTransform.position.x = (std::min)(c.second->node->localTransform.position.x, +fieldCfg.size.x * 0.5f - cartCfg.size.x * 0.5f);
				c.second->instanceData.color = _config->GetCartConfig().color;
			}
		}
	}

}
