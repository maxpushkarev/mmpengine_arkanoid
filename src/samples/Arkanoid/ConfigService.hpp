#pragma once
#include <Arkanoid/Service.hpp>
#include <Core/Base.hpp>
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

		struct FieldConfig final
		{
			MMPEngine::Core::Vector2Float size;
			MMPEngine::Core::Vector4Float color;
		};

		virtual const GeneralConfig& GetGeneralConfig() const = 0;
		virtual const FieldConfig& GetFieldConfig() const = 0;
	};

	class SimpleConfigService final : public IConfigService
	{
	public:
		SimpleConfigService();
		const GeneralConfig& GetGeneralConfig() const override;
		const FieldConfig& GetFieldConfig() const override;
	private:
		static void ParseHexArgbColor(std::int32_t hexColor, MMPEngine::Core::Vector4Float& res);
		GeneralConfig _general;
		FieldConfig _field;
	};
}