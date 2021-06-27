#pragma once
#include <cstdint>
#include <limits>


namespace hal
{
	class RegisterConfig
	{
	public:
		
		constexpr RegisterConfig(): m_value(0), m_mask(0)
		{
			
		}

		/* Construct a config with a given value, mask is set */
		constexpr RegisterConfig(uint32_t p_value): m_value(p_value), m_mask(std::numeric_limits<uint32_t>::max())
		{
		}


		constexpr uint32_t getMask() const
		{
			return m_mask;
		}
		
		constexpr uint32_t getValue() const
		{
			return m_value;
		}
	protected:
		uint32_t m_value;
		uint32_t m_mask;
		
	}; // class RegisterConfig
	} // namespace hal
