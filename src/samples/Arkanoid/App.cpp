#include <Arkanoid/App.hpp>

namespace Sample::Arkanoid
{
	App::App(const std::shared_ptr<MMPEngine::Feature::BaseLogger>& logger) : UserApp(logger)
	{
	}

	void App::Initialize()
	{
		UserApp::Initialize();

		_configService = std::make_shared<SimpleConfigService>();
		_worldService = std::make_shared<WorldService>();
	}

	void App::OnUpdate(std::float_t dt)
	{
		UserApp::OnUpdate(dt);

		_state.simulationTime += dt;

		const auto fixedDt = 1.0f / static_cast<std::float_t>(_configService->GetGeneralConfig().simulationTicksPerSecond);
		while (_state.simulationTime - fixedDt >= 0)
		{
			_state.simulationTime -= fixedDt;
			_worldService->Simulate(fixedDt);
		}

		_worldService->Update(dt);
	}

	void App::OnNativeWindowUpdated()
	{
		UserApp::OnNativeWindowUpdated();
		_worldService->OnViewportUpdated();
	}

	void App::OnRender()
	{
		UserApp::OnRender();
		_worldService->OnRender();
	}

}