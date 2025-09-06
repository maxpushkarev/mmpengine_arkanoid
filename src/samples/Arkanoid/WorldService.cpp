#include <algorithm>
#include <cassert>
#include <Arkanoid/WorldService.hpp>

namespace Sample::Arkanoid
{
	void WorldService::AddObject(std::unique_ptr<BaseObject>&& object)
	{
		const auto id = object->GetId();
		const auto it = _objects.emplace(id, std::move(object));

		assert(it.second);

		std::for_each(_systems.begin(), _systems.end(), [&it](const auto& sys)
			{
				sys->TryAttachObject(it.first->second.get());
			});
	}

	void WorldService::RemoveObject(BaseObject::IdType id)
	{
		std::for_each(_systems.begin(), _systems.end(), [id](const auto& sys)
			{
				sys->TryDetachObject(id);
			});

		const auto eraseRes = _objects.erase(id);
		assert(eraseRes);
	}

	void WorldService::AddSystem(std::unique_ptr<BaseSystem>&& system)
	{
		_systems.emplace_back(std::move(system));
		auto systemPtr = _systems.back().get();

		if (const auto castedSystem = dynamic_cast<BaseSimulationSystem*>(systemPtr))
		{
			_simulationSystems.push_back(castedSystem);
		}

		if (const auto castedSystem = dynamic_cast<BaseUpdateSystem*>(systemPtr))
		{
			_updateSystems.push_back(castedSystem);
		}

		if (const auto castedSystem = dynamic_cast<BaseRenderSystem*>(systemPtr))
		{
			_renderSystems.push_back(castedSystem);
		}
	}

	void WorldService::Simulate(std::float_t dt)
	{
		std::for_each(_simulationSystems.begin(), _simulationSystems.end(), [dt](const auto system)
			{
				system->Simulate(dt);
			});
	}

	void WorldService::Update(std::float_t dt)
	{
		std::for_each(_updateSystems.begin(), _updateSystems.end(), [dt](const auto system)
			{
				system->Update(dt);
			});
	}

	void WorldService::OnViewportUpdated()
	{
		std::for_each(_renderSystems.begin(), _renderSystems.end(), [](const auto system)
			{
				system->OnViewportUpdated();
			});
	}

	void WorldService::OnRender()
	{
		std::for_each(_renderSystems.begin(), _renderSystems.end(), [](const auto system)
			{
				system->OnRender();
			});
	}
}