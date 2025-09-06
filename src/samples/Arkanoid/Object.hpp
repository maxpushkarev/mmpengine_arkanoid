#pragma once
#include <cstdint>

namespace Sample::Arkanoid
{
	class BaseObject
	{
	public:

		using IdType = std::uint64_t;

		BaseObject();
		virtual ~BaseObject();
		BaseObject(const BaseObject&) = delete;
		BaseObject(BaseObject&&) noexcept = delete;
		BaseObject& operator=(const BaseObject&) = delete;
		BaseObject& operator=(BaseObject&&) noexcept = delete;

		IdType GetId() const;
	private:
		IdType _id;
	};
}