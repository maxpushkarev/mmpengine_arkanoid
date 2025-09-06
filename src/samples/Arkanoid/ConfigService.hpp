#pragma once
#include <Arkanoid/Service.hpp>
#include <Core/Base.hpp>
#include <cstdint>
#include <vector>

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

		struct CameraConfig final
		{
			MMPEngine::Core::Vector4Float backgroundColor;
			std::float_t multiplier;
		};

		struct WallConfig final
		{
			MMPEngine::Core::Vector4Float color;
			std::float_t thickness;
		};

		struct BrickConfig final
		{
			MMPEngine::Core::Vector2Float size;
			std::uint32_t rows;
			std::vector<MMPEngine::Core::Vector4Float> colors;
		};

		struct CartConfig final
		{
			std::float_t speed;
			MMPEngine::Core::Vector2Float size;
			MMPEngine::Core::Vector4Float color;
		};

		struct BallConfig final
		{
			std::float_t speed;
			std::float_t radius;
			std::float_t initialDirectionXMin;
			std::float_t initialDirectionXMax;
			MMPEngine::Core::Vector4Float color;
		};

		virtual const GeneralConfig& GetGeneralConfig() const = 0;
		virtual const FieldConfig& GetFieldConfig() const = 0;
		virtual const CameraConfig& GetCameraConfig() const = 0;
		virtual const WallConfig& GetWallConfig() const = 0;
		virtual const BrickConfig& GetBrickConfig() const = 0;
		virtual const CartConfig& GetCartConfig() const = 0;
		virtual const BallConfig& GetBallConfig() const = 0;
	};

	class SimpleConfigService final : public IConfigService
	{
	public:
		SimpleConfigService();
		const GeneralConfig& GetGeneralConfig() const override;
		const FieldConfig& GetFieldConfig() const override;
		const CameraConfig& GetCameraConfig() const override;
		const WallConfig& GetWallConfig() const override;
		const BrickConfig& GetBrickConfig() const override;
		const CartConfig& GetCartConfig() const override;
		const BallConfig& GetBallConfig() const override;
	private:
		static void ParseHexArgbColor(std::int32_t hexColor, MMPEngine::Core::Vector4Float& res);
		GeneralConfig _general;
		FieldConfig _field;
		CameraConfig _camera;
		WallConfig _wall;
		BrickConfig _brick;
		CartConfig _cart;
		BallConfig _ball;
	};
}