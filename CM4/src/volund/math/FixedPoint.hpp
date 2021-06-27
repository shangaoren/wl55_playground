#pragma once
#include <cstdint>

namespace math
{
	template <class Underlying, uint32_t fractionnalBits>
	class FixedPoint
	{
	  public:
		constexpr FixedPoint(const Underlying rawValue) : m_value(rawValue)
		{
			
		}

		constexpr FixedPoint(const float value) : m_value(static_cast<Underlying>(value*kOne))
		{
			
		}

		constexpr float toFloat()
		{
			return (static_cast<float>(m_value) / static_cast<float>(kOne));
		}

		constexpr Underlying rawValue()
		{
			return m_value;
		}

	  private:
		Underlying m_value;
		static constexpr Underlying kOne = 1U << fractionnalBits;
	};
} // namespace utils
