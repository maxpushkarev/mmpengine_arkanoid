#pragma once
#include <Feature/App.hpp>

namespace Sample::Arkanoid
{
	class App : public MMPEngine::Feature::UserApp
	{
	public:
		App(const std::shared_ptr<MMPEngine::Feature::BaseLogger>& logger);
		void Initialize() override;
	};
}
