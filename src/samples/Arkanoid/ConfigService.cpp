#include <Arkanoid/ConfigService.hpp>

namespace Sample::Arkanoid
{
	SimpleConfigService::SimpleConfigService()
	{
		_general.simulationTicksPerSecond = 60U;

		_field.size = { 10.0f, 10.0f };
		ParseHexArgbColor(0x0324fc, _field.color);
	}

	void SimpleConfigService::ParseHexArgbColor(std::int32_t hexColor, MMPEngine::Core::Vector4Float& res)
	{
		const auto a = (hexColor >> 24) & 0xFF;
		const auto r = (hexColor >> 16) & 0xFF;
		const auto g = (hexColor >> 8) & 0xFF;
		const auto b = hexColor & 0xFF;

		res.x = static_cast<std::float_t>(r) / static_cast<std::float_t>((std::numeric_limits<std::uint8_t>::max)());
		res.y = static_cast<std::float_t>(g) / static_cast<std::float_t>((std::numeric_limits<std::uint8_t>::max)());
		res.z = static_cast<std::float_t>(b) / static_cast<std::float_t>((std::numeric_limits<std::uint8_t>::max)());
		res.w = static_cast<std::float_t>(a) / static_cast<std::float_t>((std::numeric_limits<std::uint8_t>::max)());
	}

	const IConfigService::GeneralConfig& SimpleConfigService::GetGeneralConfig() const
	{
		return _general;
	}

	const IConfigService::FieldConfig& SimpleConfigService::GetFieldConfig() const
	{
		return _field;
	}

}