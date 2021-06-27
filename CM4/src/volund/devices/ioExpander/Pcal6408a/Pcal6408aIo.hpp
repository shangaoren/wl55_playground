#pragma once

#include <functional>

#include "volund/interfaces/drivers/IGpio.hpp"
#include "volund/interfaces/drivers/IExti.hpp"

#include "yggdrasil/kernel/Task.hpp"
#include "Pcal6408a.hpp"

namespace devices
{
	namespace io_expanders
	{
		class Pcal6408Ios;
		class Pcal6408Io : public interfaces::drivers::IGpio, public interfaces::drivers::IExti
		{
			friend class Pcal6408Ios;

		public:
			/* IIn functions*/
			bool inputState() final
			{
				return m_expander.getState(c_pinNumber);
			}

			/* IOut Functions*/
			void setup(PullUpPullDown pupd, OutputType ot) final
			{
				m_expander.pupdMode(c_pinNumber, pupd);
				m_expander.outputMode(c_pinNumber, ot);
				m_expander.pinMode(c_pinNumber, interfaces::drivers::IGpio::GpioMode::output);
			}
			void output(bool state) final
			{
				m_expander.setState(c_pinNumber, state);
			}
			bool output() final
			{
				return m_expander.getState(c_pinNumber);
			}

			/* IGpio Function*/
			void setup(GpioMode mode) final
			{
				m_expander.pinMode(c_pinNumber, mode);
			}
			void makeOutput(PullUpPullDown pupd, OutputType ot) final
			{
				m_expander.pupdMode(c_pinNumber, pupd);
				m_expander.outputMode(c_pinNumber, ot);
				m_expander.pinMode(c_pinNumber, interfaces::drivers::IGpio::GpioMode::output);
			}
			void makeInput() final
			{
				m_expander.pinMode(c_pinNumber, interfaces::drivers::IGpio::GpioMode::input);
			}

			/*IExti Functions*/
			void setTriggerEdge(TriggerEdge edge) final
			{
				if (edge == interfaces::drivers::IExti::TriggerEdge::both)
					m_expander.enableInterrupt(c_pinNumber);
				else
					m_expander.disableInterrupt(c_pinNumber);
			}

		private:
			Pcal6408Io(Pcal6408a &expander, uint8_t pinNumber)
				: m_expander(expander)
				, c_pinNumber(pinNumber)
			{
			}

			Pcal6408a &m_expander;
			const uint8_t c_pinNumber;
		}; 
	} // namespace io_expanders
} // namespace devices