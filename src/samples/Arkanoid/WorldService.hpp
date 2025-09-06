#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include <Arkanoid/Service.hpp>
#include <Arkanoid/System.hpp>
#include <Arkanoid/Object.hpp>

namespace Sample::Arkanoid
{
	class WorldService : public IService
	{
	public:
		void AddObject(std::unique_ptr<BaseObject>&& object);
		void RemoveObject(BaseObject::IdType id);
		void AddSystem(std::unique_ptr<BaseSystem>&& system);

		void Simulate(std::float_t dt);
		void Update(std::float_t dt);

		void OnViewportUpdated();
		void OnRender();

		template<typename TSystem, typename ...TArg>
		void CreateSystem(TArg&&...);

	private:
		std::vector<std::unique_ptr<BaseSystem>> _systems;
		std::unordered_map<BaseObject::IdType, std::unique_ptr<BaseObject>> _objects;

		std::vector<BaseSimulationSystem*> _simulationSystems;
		std::vector<BaseUpdateSystem*> _updateSystems;
		std::vector<BaseRenderSystem*> _renderSystems;
	};

	template <typename TSystem, typename... TArg>
	void WorldService::CreateSystem(TArg&&... args)
	{
		static_assert(std::is_base_of_v<BaseSystem, TSystem>);
		AddSystem(std::make_unique<TSystem>(std::forward<TArg>(args)...));
	}
}