#pragma once
#include <Feature/App.hpp>
#include <Arkanoid/ConfigService.hpp>
#include <Arkanoid/WorldService.hpp>

namespace Sample::Arkanoid
{
	class App : public MMPEngine::Feature::UserApp
	{
	private:
		class State final
		{
		public:
			std::float_t simulationTime = 0;
			bool readyToFinish = false;
		};
	public:
		App(const std::shared_ptr<MMPEngine::Feature::BaseLogger>& logger);
		void Initialize() override;
		void OnNativeWindowUpdated() override;
		void OnUpdate(std::float_t dt) override;
		void OnRender() override;
		bool IsReadyToFinish() const override;
	private:
		State _state;
		std::shared_ptr<IConfigService> _configService;
		std::shared_ptr<WorldService> _worldService;
	};
}
