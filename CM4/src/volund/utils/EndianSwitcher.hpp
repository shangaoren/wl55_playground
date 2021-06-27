#pragma once
#include <cstdint>

namespace volund
{
	class Endian
	{
	public:
		constexpr static uint16_t change(const uint16_t t_data)
		{
			return (t_data >> 8) | ((t_data & 0xFF) << 8);
		}
	};
}
