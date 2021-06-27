#pragma once
#include <cstdint>
#include "yggdrasil/kernel/Mutex.hpp"
#include "volund/interfaces/drivers/II2C.hpp"
#include "volund/virtualDrivers/i2c/VirtualI2CBus.hpp"

namespace virtualDrivers
{
	namespace i2c
	{

		template<virtualDrivers::i2c::I2CBus& i2cBusWrapper>
		class VirtualI2cDriver : public interfaces::drivers::II2c
		{
	
		public:
		
			VirtualI2cDriver()
			{
			}
			
			static void init()
			{
				i2cBusWrapper.directDriver().onTransferEnd(transferEndDispatcher);
			}
			
			static void transferEndDispatcher(interfaces::drivers::II2c::TransferEndCode endCode)
			{
				Y_ASSERT(s_active != nullptr);
				s_active->transferEnd(endCode);
				i2cBusWrapper.mutex().release(s_active->m_driver);
			}
							
			virtual bool write(const uint8_t* buffer, const uint16_t size, const uint8_t slave) override
			{
				m_driver = i2cBusWrapper.mutex().get();
				if (m_driver == nullptr)
					return false;
				s_active = this;
				return m_driver->write(buffer, size, slave);
			}
			virtual bool write(const uint8_t* buffer, const uint16_t size, const uint8_t slave, const uint8_t reg) override
			{
				m_driver = i2cBusWrapper.mutex().get();
				if (m_driver == nullptr)
					return false;
				s_active = this;
				return m_driver->write(buffer, size, slave, reg);
			}
			virtual bool write(const uint8_t* buffer, const uint16_t size, const uint8_t slave, const uint16_t reg) override
			{
				m_driver = i2cBusWrapper.mutex().get();
				if (m_driver == nullptr)
					return false;
				s_active = this;
				return m_driver->write(buffer, size, slave, reg);
			}
			virtual bool read(uint8_t* buffer, const uint16_t size, const uint8_t slave, const uint8_t reg) override
			{
				m_driver = i2cBusWrapper.mutex().get();
				if (m_driver == nullptr)
					return false;
				s_active = this;
				return m_driver->read(buffer, size, slave, reg);
			}
			virtual bool read(uint8_t* buffer, const uint16_t size, const uint8_t slave, const uint16_t reg) override
			{
				m_driver = i2cBusWrapper.mutex().get();
				if (m_driver == nullptr)
					return false;
				s_active = this;
				return m_driver->read(buffer, size, slave, reg);
			}

			void reset() override
			{
				m_driver = i2cBusWrapper.mutex().get();
				if (m_driver == nullptr)
					return;
				s_active = this;
				m_driver->reset();
				return;
			}

		  private:
			
			
			interfaces::drivers::II2c *m_driver = nullptr;
			static inline VirtualI2cDriver *s_active = nullptr;
		}; // end class VirtualI2CDriver
	}// End namespace i2c
} // End namespace virtualDrivers
