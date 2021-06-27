#pragma once
#include <cstdint>

namespace hal
{
	template<typename FieldType, std::size_t FieldSize, std::size_t FieldOffset, typename FieldValues>
	class RegisterConfigField
	{
	public:
		constexpr RegisterConfigField(FieldType& t_underlying, FieldType& t_fieldMask) : m_underlying(t_underlying) , m_fieldMask(t_fieldMask)
		{
		}
		
		constexpr void operator=(FieldValues t_value)
		{
			m_fieldMask |= static_cast<FieldType>(getMask());
			m_underlying = (m_underlying & ~static_cast<FieldType>(getMask())) | static_cast<FieldType>(t_value << FieldOffset);
		}
		
		
		constexpr void operator|(FieldValues t_value)
		{
			m_underlying |= static_cast<FieldType>(t_value << FieldOffset);
		}
		
		constexpr void operator&(FieldValues t_value)
		{
			m_fieldMask |= static_cast<FieldType>(t_value << FieldOffset);
		}


		constexpr operator FieldType()
		{
			return static_cast<FieldType>((m_underlying >> FieldOffset) & ((1U << FieldSize) - 1U));
		}
		
		// reset part of mask corresponding to this field
		constexpr void clearMask()
		{
			m_fieldMask & ~getMask();
		}
		
	private:
		FieldType& m_underlying;
		FieldType& m_fieldMask;
		
		constexpr FieldType getMask()
		{
			return (((1U << FieldSize) - 1U) << FieldOffset);
		}
	}; // Field
	}
