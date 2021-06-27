#pragma once

#include <cstdint>
#include <array>
#include "yggdrasil/kernel/Api.hpp"
#include "yggdrasil/kernel/Event.hpp"
#include "volund/interfaces/drivers/II2C.hpp"


namespace devices
{
	namespace screens
	{
		template<uint8_t width, uint8_t height>
		class Ssd1306
		{
		public:
			constexpr Ssd1306(interfaces::drivers::II2c& i2c, uint8_t i2cAddress = 0b0111100)
				: m_i2c(i2c), selfAddress(i2cAddress)
			{
				m_screenBuffer.fill(0x00);
			}
			
			void init()
			{
				m_i2c.onTransferEnd([&](interfaces::drivers::II2c::TransferEndCode code)
				{this->m_transferEnded.signal(); });	// set I2c end callback
				muxRatio();
				displayOffset(0U);
				displayStartLine();
				segmentRemap(true);
				outputScanDirectionLeft(true);
				outputComPinHardwareConfiguration(0x02);
				setContrast(175U);
				forceEntireDisplayOn(false);
				inverseDisplayColor(false);
				oscilatorFrequency(0x80);
				chargePumpRegulator(0x14);
				displayState(true);
				
				setPageAddress(0, 3);
				memoryAddressingMode(MemoryAddressingMode::horizontal);
			}
			
			void clearAll()
			{
				m_screenBuffer.fill(0x00);
			}
			
			void contrast(float percent)
			{
				if (percent < 0.f)
					percent = 0.f;
				if (percent > 100.f)
					percent = 100.f;
				setContrast(static_cast<uint8_t>(percent * 2.55f));
			}
			
			inline void print()
			{
				setcolumnAddress(0, 127);
				setPageAddress(0, 3);
				sendData(m_screenBuffer.data(), m_screenBuffer.size());
			}

			
			inline void invert(bool isInverted)
			{
				inverseDisplayColor(isInverted);
			}
			
			inline void forceOn(bool isForced)
			{
				forceEntireDisplayOn(isForced);
			}
			
			inline void showDisplay()
			{
				displayState(true);
			}
			
			inline void hideDisplay()
			{
				displayState(false);
			}
			
			void setPixel(uint8_t column, uint8_t row, bool value)
			{
				if (value == true)
					m_screenBuffer[column + 128*(row / 8)] |= (1U << (row % 8));
				else
					m_screenBuffer[column + 128*(row / 8)] &= ~(1U << (row % 8));
			}
			
		private:
			interfaces::drivers::II2c& m_i2c;	// I2c interface used to communicate with the screen
			kernel::Event m_transferEnded;	// The event used to notify or wait for I2c transfer end
			const uint8_t selfAddress;	//the address of the I2c device 
			static constexpr uint8_t kWidth = width;	// The width of the screen in pixels
			static constexpr uint8_t kHeight = height;	// The height of the screen in pixels
			
			static constexpr uint8_t kDataAddress = 0b01000000;
			
			std::array<uint8_t, width*(height / 8)> m_screenBuffer;
			
			enum class SingleByteCommand : uint8_t
			{
				setDisplayStartLine = 0x40,
				setSegmentRemapFalse = 0xA0,
				setSegmentRemapTrue = 0xA1,
				setDisplayOutputScanDirectionToRight = 0xC0, 	//a verifier
				setDisplayOutputScanDirectionToLeft = 0xC8,		
				disableForceEntireDisplayOn = 0xA4,
				forceEntireDisplayOn = 0xA5,
				inverseDisplayOff = 0xA6,
				inverseDisplayOn = 0xA7,
				displayOff = 0xAE,
				displayOn = 0xAF,
			};
			
			enum class DualByteCommand : uint8_t
			{
				muxRatio = 0xA8,
				setDisplayOffset = 0xD3,
				setComPinHardwareConfiguration = 0xDA,
				setContrastControl = 0x81,
				setOscillatorFrequency = 0xD5,
				enableChargePumpRegulator = 0x8D,
				setMemoryAddressingMode = 0x20,
			};
			
			enum class MemoryAddressingMode : uint8_t
			{
				horizontal = 0,
				vertical = 1,
				page = 2,
			};
			
			
			bool sendData(const uint8_t *data, const uint16_t size)
			{
				m_i2c.write(data, size, selfAddress, kDataAddress);
				m_transferEnded.wait();
				return true;
			}
			
			/*Send a single byte command to the screen*/
			bool sendCommand(const SingleByteCommand command)
			{
				uint8_t buffer[] = { 0b00000000, static_cast<const uint8_t>(command)};
				uint16_t size = 2;
				m_i2c.write(buffer, size , selfAddress);
				m_transferEnded.wait();
				return true;
			}
			
			/*Send a dual byte command to screen (comand + parameter)*/
			bool sendCommand(const DualByteCommand command, const uint8_t parameter)
			{
				uint8_t buffer[] = {0b00000000, static_cast<const uint8_t>(command), parameter };
				m_i2c.write(buffer, 3, selfAddress);
				m_transferEnded.wait();
				return true;
			} 
			
			/* Set the Multiplexer ratio for rows, equals to rows -1*/
			inline void muxRatio()
			{
				sendCommand(DualByteCommand::muxRatio, kHeight-1);
			}
			
			/*Set Display Offset for driver*/
			inline void displayOffset(const uint8_t offset)
			{
				sendCommand(DualByteCommand::setDisplayOffset, offset);
			}
			
			/* set Display Start line to zero*/
			inline void displayStartLine()
			{
				sendCommand(SingleByteCommand::setDisplayStartLine);
			}
			
			/* Set Display start Line to a specific value*/
			void displayStartLine(const uint8_t startLineNumber)
			{
				std::array<uint8_t, 2> buffer = { 0b00000000, reinterpret_cast<const uint8_t>(SingleByteCommand::setStartLine | (startLineNumber & 0b00111111)) };
				m_i2c.write(buffer.data(), buffer.size(), selfAddress);
				m_transferEnded.wait();
			}
			
			
			void segmentRemap(const bool state)
			{
				if (state)
					sendCommand(SingleByteCommand::setSegmentRemapTrue);
				else
					sendCommand(SingleByteCommand::setSegmentRemapFalse);
			}
			
			void outputScanDirectionLeft(const bool isLeft)
			{
				if (isLeft)
					sendCommand(SingleByteCommand::setDisplayOutputScanDirectionToLeft);
				else
					sendCommand(SingleByteCommand::setDisplayOutputScanDirectionToRight);
			}
			
			inline void outputComPinHardwareConfiguration(const uint8_t configuration)
			{
				sendCommand(DualByteCommand::setComPinHardwareConfiguration, configuration);
			}
			
			/* Set screen contrast 0~255 */
			inline void setContrast(const uint8_t contrast)
			{
				sendCommand(DualByteCommand::setContrastControl, contrast);
			}
			
			void forceEntireDisplayOn(const bool isOn)
			{
				if (isOn)
					sendCommand(SingleByteCommand::forceEntireDisplayOn);
				else
					sendCommand(SingleByteCommand::disableForceEntireDisplayOn);
			}
			
			/* Inverse the color of the screen */
			void inverseDisplayColor(const bool isOn)
			{
				if (isOn)
					sendCommand(SingleByteCommand::inverseDisplayOn);
				else
					sendCommand(SingleByteCommand::inverseDisplayOff);
			}
			
			/* Set the Oscillator Frequency */
			void oscilatorFrequency(const uint8_t frequency)
			{
				sendCommand(DualByteCommand::setOscillatorFrequency, frequency);
			}
			
			void chargePumpRegulator(const uint8_t value)
			{
				sendCommand(DualByteCommand::enableChargePumpRegulator, value);
			}
			
			void displayState(const bool isOn)
			{
				if (isOn)
					sendCommand(SingleByteCommand::displayOn);
				else
					sendCommand(SingleByteCommand::displayOff);
			}
			
			inline void memoryAddressingMode(const MemoryAddressingMode mode)
			{
				sendCommand(DualByteCommand::setMemoryAddressingMode, static_cast<const uint8_t>(mode));
			}
			
			void setcolumnAddress(const uint8_t columnStartAddress, const uint8_t columnEndAddress)
			{
				std::array<uint8_t, 4> buffer = { 0b00000000, 0x21, columnStartAddress, columnEndAddress};
				m_i2c.write(buffer.data(), buffer.size(), selfAddress);
				m_transferEnded.wait();
			}
			
			void setPageAddress(const uint8_t pageAddressStart, const uint8_t pageAddressEnd)
			{
				std::array<uint8_t, 4> buffer = { 0b00000000, 0x22, pageAddressStart, pageAddressEnd};
				m_i2c.write(buffer.data(), buffer.size(), selfAddress);
				m_transferEnded.wait();
			}
		};	// Class SSD1306
	}	// namespace screens
}	// namespace devices