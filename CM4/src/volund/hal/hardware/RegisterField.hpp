#pragma once
#include <cstdint>
#include <cstddef>

namespace hardware
{
	template <std::size_t t_fieldBitsSize, std::size_t t_fieldBitsOffset, typename ValuesType>
	class RegisterField
	{
	  public:
		constexpr RegisterField(volatile uint32_t* t_peripheralRegister) : m_register(*t_peripheralRegister)
		{
		}

		// set a value in a field
		constexpr void operator=(ValuesType value)
		{
			m_register = (m_register & ~m_mask) | (value << t_fieldBitsOffset);
		}

		
		constexpr ValuesType operator=(RegisterField &field)
		{
			return reinterpret_cast<ValuesType>((m_register & m_mask) >> t_fieldBitsOffset);
		}
		
		constexpr operator ValuesType ()
		{
			return ValuesType(static_cast<ValuesType>((m_register & m_mask) >> t_fieldBitsOffset));
		}
		
		constexpr bool operator==(ValuesType value)
		{
			if (static_cast<ValuesType>((m_register & m_mask) >> t_fieldBitsOffset) == value)
				return true;
			else
				return false;
		}
		
		constexpr bool operator!=(ValuesType value)
		{
			if (static_cast<ValuesType>((m_register & m_mask) >> t_fieldBitsOffset) != value)
				return true;
			else
				return false;
		}

	  private:
		static constexpr uint32_t m_mask = ((1U << t_fieldBitsSize) - 1U) << t_fieldBitsOffset;
		volatile uint32_t &m_register;
	}; // class Field


	template<std::size_t t_fieldBitsSize, std::size_t t_fieldBitsOffset, typename ValuesType, std::size_t t_fieldBitsSteps>
	class RegisterFieldArray
	{
	public:
		constexpr RegisterFieldArray(volatile uint32_t * peripheralRegister, uint8_t number) : m_register(*peripheralRegister), m_number(number)
		{}

		constexpr void operator=(ValuesType value)
		{
			m_register = (m_register & ~mask()) | (static_cast<uint32_t>(value) << (t_fieldBitsSteps * m_number));
		}

		constexpr operator ValuesType()
		{
			return static_cast<ValuesType>((m_register & mask()) >> (m_number * t_fieldBitsSteps));
		}

	private:

		volatile uint32_t& m_register;
		const uint8_t m_number;
		static constexpr uint32_t m_baseMask = ((1U << t_fieldBitsSize) - 1U) << (t_fieldBitsOffset*t_fieldBitsSize);

		constexpr uint32_t mask()
		{
			return m_baseMask << (t_fieldBitsSteps * m_number);
		}
	};
} // namespace hardware
