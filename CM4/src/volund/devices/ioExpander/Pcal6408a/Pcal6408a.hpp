#pragma once

#include <cstdint>
#include <functional>

#include "yggdrasil/kernel/Event.hpp"
#include "yggdrasil/kernel/Api.hpp"

#include "volund/interfaces/drivers/II2C.hpp"
#include "volund/interfaces/drivers/IGpio.hpp"
#include "volund/interfaces/drivers/IExti.hpp"
namespace devices
{
	namespace io_expanders
	{
	
		class Pcal6408a
		{
		public:
			Pcal6408a(interfaces::drivers::II2c &i2c, bool addressBit = false)
				: m_i2c(i2c)
				, m_exti(nullptr)
				, m_i2cCallback([&](interfaces::drivers::II2c::TransferEndCode endCode) { this->m_i2cEvent.signal(); })
				, c_deviceAddress(kAddressBase | addressBit)
			{
			}

			Pcal6408a(interfaces::drivers::II2c &i2c, interfaces::drivers::IExti *exti, bool addressBit = false)
				: m_i2c(i2c)
				, m_exti(exti)
				, m_i2cCallback([&](interfaces::drivers::II2c::TransferEndCode endCode) { 
				this->m_i2cEvent.signal(); 
			})
				, c_deviceAddress(kAddressBase | addressBit)
			{
			}

			/* Init Device Must be called before use*/
			void init()
			{
				m_i2c.onTransferEnd(m_i2cCallback);
			}

			/* Configure the pin given by the number (0 -> 7) in a gpio Mode*/
			void pinMode(uint8_t pinNumber, interfaces::drivers::IGpio::GpioMode mode)
			{
				uint8_t activeMode = 0;
				get(Pcal6408a::Addresses::configuration, activeMode);
				if (mode == interfaces::drivers::IGpio::GpioMode::input)
					set(Pcal6408a::Addresses::configuration, activeMode | (1U << pinNumber));
				else
					set(Pcal6408a::Addresses::configuration, activeMode & ~(1U << pinNumber));
			}

		
			void pupdMode(uint8_t pinNumber, interfaces::drivers::IOut::PullUpPullDown pupdMode)
			{
				switch (pupdMode)
				{
				case interfaces::drivers::IOut::PullUpPullDown::none:
					break;
				case interfaces::drivers::IOut::PullUpPullDown::pullUp:
					break;
				case interfaces::drivers::IOut::PullUpPullDown::pullDown:
					break;

				}
			}

			void outputMode(uint8_t pinNumber, interfaces::drivers::IOut::OutputType type)
			{
				uint8_t currentPortMode;
				get(Pcal6408a::Addresses::outputPortConfig, currentPortMode);
				if (type == interfaces::drivers::IOut::OutputType::pushPull)
					set(Pcal6408a::Addresses::outputPortConfig, currentPortMode & ~(1U << pinNumber));
				else
					set(Pcal6408a::Addresses::outputPortConfig, currentPortMode | (1U << pinNumber));
			}

			void outputStrength(uint8_t pinNumber)
			{
			
			}

			bool setState(uint8_t pinNumber, bool state)
			{
				uint8_t portState;
				get(Pcal6408a::Addresses::ouputPort, portState);
				if (state == true)
					set(Pcal6408a::Addresses::ouputPort, portState | (1 << pinNumber));
				else
					set(Pcal6408a::Addresses::ouputPort, portState & ~(1 << pinNumber));
				return true;
			}

			bool getState(uint8_t pinNumber)
			{
				uint8_t state;
				get(Pcal6408a::Addresses::inputPort, state);
				return (state >> pinNumber) & 1U;
			}

			bool enableInterrupt(uint8_t pinNumber)
			{
				uint8_t interruptStatus;
				get(Pcal6408a::Addresses::interruptMask, interruptStatus);
				interruptStatus &= ~(1 << pinNumber);
				set(Pcal6408a::Addresses::interruptMask, interruptStatus);
				return true;
			}

			bool disableInterrupt(uint8_t pinNumber)
			{
				uint8_t interruptStatus;
				get(Pcal6408a::Addresses::interruptMask, interruptStatus);
				interruptStatus |= (1 << pinNumber);
				set(Pcal6408a::Addresses::interruptMask, interruptStatus);
				return true;
			}

			void onInterrupt(std::function<void(bool state)> callback)
			{
				if (m_exti != nullptr)
				{
					m_exti->onExtiTrigger(callback);
					if (callback != nullptr)
						m_exti->setTriggerEdge(interfaces::drivers::IExti::TriggerEdge::both);
					else
						m_exti->setTriggerEdge(interfaces::drivers::IExti::TriggerEdge::none);
				}
			}

			uint8_t interruptStatus()
			{
				uint8_t status;
				get(Pcal6408a::Addresses::interruptStatus, status);
				return status;
			}

			bool enableInputLatch(bool isEnabled)
			{
				if (isEnabled)
					return set(devices::io_expanders::Pcal6408a::Addresses::inputLatch, 0); // all latch
				else
					return set(devices::io_expanders::Pcal6408a::Addresses::inputLatch, 0xFF);
			}

		  private:
			interfaces::drivers::II2c &m_i2c;
			interfaces::drivers::IExti *m_exti;
			kernel::Event m_i2cEvent;
			const std::function<void(interfaces::drivers::II2c::TransferEndCode endCode)> m_i2cCallback;
			static constexpr uint8_t kAddressBase = 0b00100000;
			const uint8_t c_deviceAddress;
			uint8_t m_oldInputValue = 0;

			enum class Addresses : uint8_t
			{
				inputPort         = 0x00,
				ouputPort         = 0x01,
				polarityInvertion = 0x02,
				configuration     = 0x03,
				outputStrength0   = 0x40,
				outputStrength1   = 0x41,
				inputLatch        = 0x42,
				pupdEn0           = 0x43,
				pupdEn1           = 0x44,
				interruptMask     = 0x45,
				interruptStatus   = 0x46,
				outputPortConfig  = 0x4F,
			};

			bool set(Addresses address, const uint8_t value)
			{
				if (m_i2c.write(&value, 1, c_deviceAddress, static_cast<const uint8_t>(address)) == false)
					return false;
				if (m_i2cEvent.wait() < 1)
					return false;
				return true;
			}
			bool get(Addresses address, uint8_t &value)
			{
				if (m_i2c.read(&value, 1, c_deviceAddress, static_cast<const uint8_t>(address)) == false)
					return false;
				if (m_i2cEvent.wait() < 1)
					return false;
				return true;
			}
		};
	} // namespace io_expanders
} // namespace devices
