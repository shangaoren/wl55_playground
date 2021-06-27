#pragma once
#include <cstdint>
#include <array>
#include "volund/interfaces/drivers/II2C.hpp"
#include "yggdrasil/kernel/Event.hpp"

namespace utils
{
	class I2cScanner
	{
	  public:
		// scan all 7 bits adresses of a given port 
		std::array<uint8_t, 128> &scan(interfaces::drivers::II2c &driver)
		{
			uint8_t dummy = 0;
			bool result = false;
			m_adresses.fill(0);
			auto transmissionEnd = kernel::Event(false, "i2c scan transaction");
			driver.onTransferEnd([&](interfaces::drivers::II2c::TransferEndCode endCode) {
				if (endCode == interfaces::drivers::II2c::TransferEndCode::noErrors)
					result = true;
				else
					result = false;
				transmissionEnd.signal();});
			for (auto i = 0U; i < m_adresses.size(); i++)
			{
				driver.write(&dummy, 1, i);
				transmissionEnd.wait(0);
				if (result == true)
				{
					m_adresses[i] = 0xFF;
				}
				else
				{
					m_adresses[i] = 0;
				}
			}
			return m_adresses;
		}

	  private:
		std::array<uint8_t, 128> m_adresses;
	};
} // namespace utils
