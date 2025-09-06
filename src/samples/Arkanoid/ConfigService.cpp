#include <Arkanoid/ConfigService.hpp>

namespace Sample::Arkanoid
{
	SimpleConfigService::SimpleConfigService()
	{
		_general.simulationTicksPerSecond = 60U;

		_field.size = { 10.0f, 10.0f };
		ParseHexArgbColor(0x0324fc, _field.color);

		_camera.multiplier = 1.125f;
		ParseHexArgbColor(0x00000000, _camera.backgroundColor);

		_wall.thickness = 0.25f;
		ParseHexArgbColor(0x00c4c1b3, _wall.color);

		_brick.rows = 5;
		_brick.size = { 0.5f, 0.5f };
		_brick.colors.resize(3);
		ParseHexArgbColor(0x00fc2c03, _brick.colors[0]);
		ParseHexArgbColor(0x0003fc03, _brick.colors[1]);
		ParseHexArgbColor(0x00fcd303, _brick.colors[2]);

		_cart.speed = 10.0f;
		_cart.size = { 2.0f, 0.2f };
		ParseHexArgbColor(0x00403914, _cart.color);

		_ball.speed = 5.0f;
		_ball.radius = 0.05f;
		_ball.initialDirectionXMin = 0.7f;
		_ball.initialDirectionXMax = 1.5f;
		ParseHexArgbColor(0x00ffffff, _ball.color);
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

	const IConfigService::CameraConfig& SimpleConfigService::GetCameraConfig() const
	{
		return _camera;
	}

	const IConfigService::WallConfig& SimpleConfigService::GetWallConfig() const
	{
		return _wall;
	}

	const IConfigService::BrickConfig& SimpleConfigService::GetBrickConfig() const
	{
		return _brick;
	}

	const IConfigService::CartConfig& SimpleConfigService::GetCartConfig() const
	{
		return _cart;
	}

	const IConfigService::BallConfig& SimpleConfigService::GetBallConfig() const
	{
		return _ball;
	}
}