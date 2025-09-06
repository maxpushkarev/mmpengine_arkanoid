#include <Arkanoid/App.hpp>
#include <Feature/AppContainer.hpp>

namespace
{
	std::int32_t Run(MMPEngine::Core::BackendType backendType)
	{
		MMPEngine::Core::GlobalContext::Settings appContextSettings{};

#if (defined(DEBUG) || defined(_DEBUG))
		appContextSettings.isDebug = true;
#else
		appContextSettings.isDebug = false;
#endif

		appContextSettings.backend = backendType;

#ifdef MMPENGINE_WIN
		const auto logger = std::make_shared<MMPEngine::Feature::Win::OutputLogger>("Arkanoid_Logger");
#endif

#ifdef MMPENGINE_MAC
		const auto logger = std::make_shared<MMPEngine::Feature::Mac::OutputLogger>("Arkanoid_Logger");
#endif

		auto userApp = std::make_unique<Sample::Arkanoid::App>(logger);
		auto rootApp = MMPEngine::Feature::App::BuildRootApp(appContextSettings, std::move(userApp), nullptr, logger);

		MMPEngine::Feature::Shared::AppContainer::Settings appContainerSettings{};
		///width and heights from GameTemplate
		appContainerSettings.base.initialWindowWidth = 1024;
		appContainerSettings.base.initialWindowHeight = 768;

		appContainerSettings.base.targetFps = 60;
		appContainerSettings.base.showBackendType = true;
		appContainerSettings.base.windowCaption = "Arkanoid";

		const auto appContainer = std::make_unique<MMPEngine::Feature::Shared::AppContainer>(std::move(appContainerSettings), std::move(rootApp));
		return appContainer->Run();
	}
}

#ifdef MMPENGINE_WIN
std::int32_t WINAPI WinMain(HINSTANCE hInstance, HINSTANCE,
	PSTR, std::int32_t)
{
	return Run(MMPEngine::Core::BackendType::Dx12);
}
#endif

#ifdef MMPENGINE_MAC
std::int32_t main()
{
	return Run(MMPEngine::Core::BackendType::Metal);
}
#endif