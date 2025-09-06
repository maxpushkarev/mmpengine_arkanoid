#pragma once
#include <Feature/App.hpp>
#include <Arkanoid/ConfigService.hpp>

namespace Sample::Arkanoid
{
	class App : public MMPEngine::Feature::UserApp
	{
	private:
		class State final
		{
		public:
			std::float_t simulationTime = 0;
		};
	public:
		App(const std::shared_ptr<MMPEngine::Feature::BaseLogger>& logger);
		void Initialize() override;
	private:
		State _state;
		std::shared_ptr<IConfigService> _configService;
	};
}
