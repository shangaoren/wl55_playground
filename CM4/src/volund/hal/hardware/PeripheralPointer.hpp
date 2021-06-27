#pragma once
#include <cstdint>

namespace processor
{
	/*class PeripheralPointer
	{
	public:
		constexpr PeripheralPointer(uint32_t address) : m_peripheralAddress(address)
		{}

		constexpr volatile uint32_t const * operator=(PeripheralPointer& periph)
		{
			return reinterpret_cast<volatile uint32_t const*>(m_peripheralAddress);
		}

		constexpr operator volatile uint32_t const* () const
		{
			return reinterpret_cast<volatile uint32_t const*>(m_peripheralAddress);
		}
	private:
		const uint32_t m_peripheralAddress;
	};*/
}