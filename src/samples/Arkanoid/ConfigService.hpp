#pragma once
#include <Arkanoid/Service.hpp>
#include <cstdint>

namespace Sample::Arkanoid
{
	class IConfigService : public IService
	{
	public:
		struct GeneralConfig final
		{
			std::uint32_t simulationTicksPerSecond;
		};

		virtual const GeneralConfig& GetGeneralConfig() const = 0;
	};

	class SimpleConfigService final : public IConfigService
	{
	public:
		SimpleConfigService();
		const GeneralConfig& GetGeneralConfig() const override;
	private:
		GeneralConfig _general;
	};
}