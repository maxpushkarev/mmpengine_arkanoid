#pragma once
#include <cmath>
#include <map>
#include <Arkanoid/Object.hpp>

namespace Sample::Arkanoid
{
	class WorldService;
	class IConfigService;

	class BaseSystem
	{
	public:
		BaseSystem(WorldService* world, const IConfigService* config);
		BaseSystem();
		virtual ~BaseSystem();
		BaseSystem(const BaseSystem&) = delete;
		BaseSystem(BaseSystem&&) noexcept = delete;
		BaseSystem& operator=(const BaseSystem&) = delete;
		BaseSystem& operator=(BaseSystem&&) noexcept = delete;

		virtual void TryAttachObject(BaseObject* object) = 0;
		virtual void TryDetachObject(BaseObject::IdType id) = 0;
	protected:
		WorldService* const _world = nullptr;
		const IConfigService* const _config = nullptr;
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

	template<typename TObject>
	class ObjectSystem : public virtual BaseSystem
	{
		static_assert(std::is_base_of_v<BaseObject, TObject>);
	public:
		void TryAttachObject(BaseObject* object) override;
		void TryDetachObject(BaseObject::IdType id) override;
	protected:
		std::map<BaseObject::IdType, TObject*> _objectsMap;
	};

	template <typename ...TObjects>
	class MultiObjectSystem : public ObjectSystem<TObjects>...
	{
	public:
		void TryAttachObject(BaseObject* object) override;
		void TryDetachObject(BaseObject::IdType id) override;
	};

	template <typename TObject>
	void ObjectSystem<TObject>::TryAttachObject(BaseObject* object)
	{
		if (const auto castedObject = dynamic_cast<TObject*>(object))
		{
			_objectsMap.emplace(object->GetId(), castedObject);
		}
	}

	template <typename TObject>
	void ObjectSystem<TObject>::TryDetachObject(BaseObject::IdType id)
	{
		_objectsMap.erase(id);
	}

	template <typename... TObjects>
	void MultiObjectSystem<TObjects...>::TryAttachObject(BaseObject* object)
	{
		(ObjectSystem<TObjects>::TryAttachObject(object), ...);
	}

	template <typename... TObjects>
	void MultiObjectSystem<TObjects...>::TryDetachObject(BaseObject::IdType id)
	{
		(ObjectSystem<TObjects>::TryDetachObject(id), ...);
	}
}