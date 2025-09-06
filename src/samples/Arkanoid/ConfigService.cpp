#include <Arkanoid/ConfigService.hpp>

namespace Sample::Arkanoid
{
	SimpleConfigService::SimpleConfigService()
	{
		_general.simulationTicksPerSecond = 60U;
	}

	const IConfigService::GeneralConfig& SimpleConfigService::GetGeneralConfig() const
	{
		return _general;
	}

}