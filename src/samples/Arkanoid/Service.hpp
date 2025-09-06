#pragma once

namespace Sample::Arkanoid
{
	class IService
	{
	public:
		IService();
		virtual ~IService();
		IService(const IService&) = delete;
		IService(IService&&) noexcept = delete;
		IService& operator=(const IService&) = delete;
		IService& operator=(IService&&) noexcept = delete;
	};
}