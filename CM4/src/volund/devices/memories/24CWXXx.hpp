#pragma once

#include <cstdint>
#include <array>

#include "volund/interfaces/drivers/II2C.hpp"
#include "yggdrasil/kernel/Api.hpp"
#include "yggdrasil/kernel/Event.hpp"

namespace devices
{
	namespace memories
	{
		class eeprom24CW
		{
		public:
			enum class MemorySize
			{
				_16K,
				_32K,
				_64K,
				_128K,
			};
		
			eeprom24CW(const MemorySize size, interfaces::drivers::II2c& i2cDriver, const uint8_t i2cAddress = 0b01010000) // default address
				: kMemorySize(getMemorySizeBytes(size))
				, m_i2c(i2cDriver)
				, kslaveAddress(i2cAddress)
			{
			
			}
		
			void init()
			{
				m_i2c.onTransferEnd(m_i2cTransferEndCallback);
			}
		
			bool write(const uint8_t *data, const uint16_t startAddress, const uint16_t size)
			{
				uint16_t writtenBytesCount = 0;
				uint16_t leftToWrite = size;
				while (leftToWrite > 0)
				{
					uint16_t address = (startAddress + writtenBytesCount);
					uint16_t page = address / kBytesPerPage;
					uint16_t offset = address % kBytesPerPage;
					uint16_t numberInPage = 0;
					if ((leftToWrite + offset) > 32)
						numberInPage = 32 - offset;
					else
						numberInPage = leftToWrite;
				
					writtenBytesCount  += writePage(data + writtenBytesCount, page, numberInPage, offset);
					leftToWrite = size - writtenBytesCount;
				}
				return true;
			}
		
			bool read(uint8_t *data, uint16_t startAddress, uint16_t size)
			{
				if (data == nullptr | size == 0)
					return false;
				m_i2c.read(data, size, kslaveAddress, startAddress);
				m_i2cTransferEnd.wait();
				return true;
			}
		
			/* Write a page inside the eeprom */
			uint16_t writePage(const uint8_t *buffer, uint8_t page, uint8_t size, uint8_t offset)
			{			
				uint16_t address = page*kBytesPerPage + offset;  	//get the data address in the eeprom
				uint16_t sizeToWrite = size;  	//compute and correct if necessary the size to be written
				if(size + offset > kBytesPerPage)
					sizeToWrite = kBytesPerPage - offset;
				if (page > kMemorySize / kBytesPerPage)
					return 0;
				if (offset > 31)
					return 0;
				bool result = m_i2c.write(buffer, sizeToWrite, kslaveAddress, address);
				m_i2cTransferEnd.wait();
				kernel::Api::sleep(6);    //wait for memory to write page (maximum is 5ms) the other way is to poll for Ack

				
				return result == true ? sizeToWrite : 0;
	
			}
		private:
		
			static constexpr uint32_t getMemorySizeBytes(MemorySize size)
			{
				switch (size)
				{
				case eeprom24CW::MemorySize::_16K:
					return 2048;
					break;
				case eeprom24CW::MemorySize::_32K:
					return 4096;
					break;
				case eeprom24CW::MemorySize::_64K:
					return 8192;
					break;
				case eeprom24CW::MemorySize::_128K:
					return 16384;
					break;
				default:
					return 2048;
				}
			}
			interfaces::drivers::II2c& m_i2c;
			const uint8_t kslaveAddress;
			const uint32_t kMemorySize;
			static constexpr uint32_t kBytesPerPage = 32;
			kernel::Event m_i2cTransferEnd;
			interfaces::drivers::II2c::TransferEndCode m_lastCode;
			std::function<void(interfaces::drivers::II2c::TransferEndCode code)> m_i2cTransferEndCallback = [&](interfaces::drivers::II2c::TransferEndCode code) { m_i2cTransferEnd.signal(); };

		}; // end class eeprom24CW
	} // end namespace memories
} // end namespace devices
