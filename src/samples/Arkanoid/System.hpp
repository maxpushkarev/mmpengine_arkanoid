#pragma once
#include <cmath>
#include <Arkanoid/Object.hpp>

namespace Sample::Arkanoid
{
	class BaseSystem
	{
	public:
		BaseSystem();
		virtual ~BaseSystem();
		BaseSystem(const BaseSystem&) = delete;
		BaseSystem(BaseSystem&&) noexcept = delete;
		BaseSystem& operator=(const BaseSystem&) = delete;
		BaseSystem& operator=(BaseSystem&&) noexcept = delete;

		virtual void TryAttachObject(BaseObject* object) = 0;
		virtual void TryDetachObject(BaseObject::IdType id) = 0;
	protected:
	};

	class BaseSimulationSystem : public virtual BaseSystem
	{
	public:
		virtual void Simulate(std::float_t dt) = 0;
	};

	class BaseUpdateSystem : public virtual BaseSystem
	{
	public:
		virtual void Update(std::float_t dt) = 0;
	};

	class BaseRenderSystem : public virtual BaseSystem
	{
	public:
		virtual void OnViewportUpdated() = 0;
		virtual void OnRender() = 0;
	};
}