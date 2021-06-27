#pragma once

#include <functional>

#include "volund/interfaces/drivers/IGpio.hpp"
#include "volund/interfaces/drivers/IExti.hpp"
#include "yggdrasil/kernel/Event.hpp"

#include "yggdrasil/kernel/Task.hpp"
#include "Pcal6408a.hpp"
#include "Pcal6408aIo.hpp"

namespace devices
{
	namespace io_expanders
	{
		class Pcal6408Ios
		{
		public:
			Pcal6408Ios(Pcal6408a &expander)
				: m_expander(expander)
				, m_io0(expander, 0)
				, m_io1(expander, 1)
				, m_io2(expander, 2)
				, m_io3(expander, 3)
				, m_io4(expander, 4)
				, m_io5(expander, 5)
				, m_io6(expander, 6)
				, m_io7(expander, 7)
				, m_interruptExti([&](bool) 
																																																						  {
m_interrupt.signal(); 
})
			{
			}

			void init()
			{
				m_expander.init();
				m_expander.onInterrupt(m_interruptExti);
			}

			bool isInterruptPending()
			{
				return m_interrupt.isAlreadyUp();
			}
			
			int16_t waitEvent(uint32_t timeout = 0)
			{
				return m_interrupt.wait(timeout);
			}

			void readInterrupts()
			{
				uint8_t interruptsState = m_expander.interruptStatus();
				for (auto i = 0U; i < 8; i++)
				{
					if (((interruptsState >> i) & 0b1) == true )
						(*this)[i].extiTrigger((*this)[i].inputState());
				}
				m_interrupt.reset();
			}

			inline bool inputLatchMode(bool isEnabled)
			{
				return m_expander.enableInputLatch(isEnabled);
			}

			Pcal6408Io& operator[](std::size_t index)
			{
				switch (index)
				{
				case 0:
					return m_io0;
					break;
				case 1:
					return m_io1;
					break;
				case 2:
					return m_io2;
					break;
				case 3:
					return m_io3;
					break;
				case 4:
					return m_io4;
					break;
				case 5:
					return m_io5;
					break;
				case 6:
					return m_io6;
					break;
				case 7:
					return m_io7;
					break;
				default :
					return m_io0;
					break;
				}
			}

		private:
			Pcal6408a &m_expander;
			Pcal6408Io m_io0;
			Pcal6408Io m_io1;
			Pcal6408Io m_io2;
			Pcal6408Io m_io3;
			Pcal6408Io m_io4;
			Pcal6408Io m_io5;
			Pcal6408Io m_io6;
			Pcal6408Io m_io7;
			std::function<void(bool)> m_interruptExti;
			kernel::Event m_interrupt = kernel::Event(false, "Expander Event");

		};
	} //namespace io_expanders
} // namespace device
