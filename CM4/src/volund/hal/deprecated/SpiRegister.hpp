#pragma once

#include <cstdint>
#include "yggdrasil/interfaces/ISpi.hpp"
#include <array>


namespace registers
{
	template <class InterfaceType, class AddressType>
	class SpiRegister
	{
	  public:
		constexpr SpiRegister(AddressType address) : m_address(address)
		{
		}

		virtual bool write(InterfaceType *buffer, uint16_t bufferSize)
		{
			__BKPT(0);
		}

		virtual bool read(InterfaceType *buffer, uint16_t bufferSize)
		{
			__BKPT(0);
		}

		constexpr AddressType address()
		{
			return m_address;
		}

	  private :
		AddressType m_address;
	};
} // namespace registers
