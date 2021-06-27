#pragma once
#include "RegisterFlags.hpp"

namespace registers
{
	template <class Underlying,class RegisterType,uint32_t t_address>
	class Register
	{
	  private:
		using Flag = Flags<Underlying, RegisterType>;

	  public:

		volatile static constexpr void set(const Flag value)
		{
			*reinterpret_cast<volatile Underlying*>(t_address) |= value.value();
		}

		volatile static constexpr bool isSet(const Flag value)
		{
			return (*reinterpret_cast<volatile Underlying *>(t_address) & value.mask()) == value.value();
		}

		volatile static constexpr void set(uint32_t bitPosition)
		{
			*reinterpret_cast<volatile Underlying *>(t_address) |= 1U << bitPosition;
		}

		volatile static constexpr bool isSet(uint32_t bitPosition)
		{
			return (*reinterpret_cast<volatile Underlying *>(t_address) & (1U << bitPosition)) != 0;
		}

		volatile static constexpr void clear(const Flag value)
		{
			*reinterpret_cast<volatile Underlying *>(t_address) &= ~value.mask();
		}

		volatile static constexpr void clear(const uint32_t bitPosition)
		{
			*reinterpret_cast<volatile Underlying *>(t_address) &= ~(1U << bitPosition);
		}

		volatile static constexpr void configure(const Flag value)
		{
			*reinterpret_cast<volatile Underlying *>(t_address) = (*reinterpret_cast<volatile Underlying *>(t_address) & ~value.mask()) | value.value();
		}

		volatile static constexpr Underlying rawValue()
		{
			return *reinterpret_cast<volatile Underlying *>(t_address);
		}
		volatile static constexpr void rawValue(Underlying value)
		{
			*reinterpret_cast<volatile Underlying *>(t_address) = value;
		}
		
		static constexpr uint32_t address()
		{
			return t_address;
		}
	};
}
