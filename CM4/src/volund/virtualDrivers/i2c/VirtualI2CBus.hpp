#pragma once
#include <cstdint>
#include "yggdrasil/kernel/Mutex.hpp"
#include "volund/interfaces/drivers/II2C.hpp"

namespace virtualDrivers
{ 
	namespace i2c
	{
		class I2CBus
		{
			friend class VirtualI2CDriver;
		public:
		  constexpr I2CBus(interfaces::drivers::II2c& driver) : m_driver(driver), m_peripheralMutex(driver)
			{
				
			}
			
			constexpr interfaces::drivers::II2c& directDriver()
			{
				return m_driver;
			}
			
			constexpr kernel::ObjectMutex<interfaces::drivers::II2c>& mutex()
			{
				return m_peripheralMutex;
			}
			
				
		private:
		  interfaces::drivers::II2c& m_driver;
		  kernel::ObjectMutex<interfaces::drivers::II2c> m_peripheralMutex; // mutex to lock the peripheral from being used elsewhere
		
		}; // end class I2CBus
	} // end namespace i2c
} // End namespace virtualDrivers