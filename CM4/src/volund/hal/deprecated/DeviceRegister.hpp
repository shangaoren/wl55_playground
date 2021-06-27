#pragma once
#include <cstdint>
#include "RegisterFlags.hpp"

namespace registers
{
	template <class Underlying, class RegisterType, uint32_t t_size = sizeof(Underlying)>
	class DeviceRegister
	{
	  private:
		using Flag = Flags<Underlying, RegisterType>;

	  public:
		constexpr DeviceRegister() : m_reg(0)
		{
		}
		constexpr DeviceRegister(Underlying reg) : m_reg(reg)
		{
		}

		constexpr void set(const Flag value)
		{
			m_reg |= value.value();
		}

		constexpr bool isSet(const Flag value)
		{
			return (m_reg & value.mask()) == value.value();
		}

		constexpr void set(uint32_t bitPosition)
		{
			m_reg |= 1U << bitPosition;
		}

		constexpr bool isSet(uint32_t bitPosition)
		{
			return (m_reg & (1U << bitPosition)) != 0;
		}

		constexpr void clear(const Flag value)
		{
			m_reg &= ~value.mask();
		}

		constexpr void clear(const uint32_t bitPosition)
		{
			m_reg &= ~(1U << bitPosition);
		}

		constexpr void configure(const Flag value)
		{
			m_reg = (m_reg & ~value.mask()) | value.value();
		}

		constexpr Underlying rawValue()
		{
			return m_reg;
		}
		constexpr void rawValue(Underlying value)
		{
			m_reg = value;
		}

	  protected:
		Underlying m_reg; /// Processor best value for storage and reading value
	};
} // namespace registers
