#include <Arkanoid/App.hpp>
#include <Arkanoid/InputSystem.hpp>
#include <Arkanoid/RoundSystem.hpp>
#include <Arkanoid/FieldSystem.hpp>
#include <Arkanoid/VisualSystem.hpp>

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

		_worldService->CreateSystem<RoundSystem>(_worldService.get(), _configService.get());
		_worldService->CreateSystem<InputSystem>(_worldService.get(), _configService.get(), GetInput().get());
		_worldService->CreateSystem<FieldSystem>(_worldService.get(), _configService.get());

		_worldService->CreateSystem<VisualSystem>(_worldService.get(), _configService.get(), GetContext());
	}

	void App::OnUpdate(std::float_t dt)
	{
		UserApp::OnUpdate(dt);

		if (GetInput()->IsButtonPressed(MMPEngine::Feature::KeyButton::Esc))
		{
			_state.readyToFinish = true;
		}

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

	bool App::IsReadyToFinish() const
	{
		return _state.readyToFinish;
	}
}