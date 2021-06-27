#pragma once
#include <cstdint>

namespace registers
{
	template <class Underlying, class RegisterType>
	class Flags
	{
	  public:
		
		explicit constexpr Flags(Underlying value, Underlying mask) : m_value(value), m_mask(mask){}
		explicit constexpr Flags(Underlying value, uint32_t size, uint32_t position) : Flags(value << position, ((1U << size) - 1U) << position){}
		explicit constexpr Flags(RegisterType value, uint32_t size, uint32_t position) : Flags(static_cast<Underlying>(value), size, position) {}

		constexpr Underlying value() const
		{
			return m_value;
		}

		constexpr Underlying mask() const
		{
			return m_mask;
		}

		constexpr Flags operator| (const Flags& rhs) const 
		{
			return Flags(this->value() | rhs.value(), this->mask() | rhs.mask());
		}
		
		
		constexpr Flags operator~() const
		{
			return Flags(~this->value() & this->mask(), this->mask());
		}

	  private:
		const Underlying m_value;
		const Underlying m_mask; //0 s mask with ones on value positions
	};
}