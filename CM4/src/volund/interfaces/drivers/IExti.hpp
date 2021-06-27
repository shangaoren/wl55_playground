#pragma once
#include <cstdint>
#include <functional>
#include "volund/interfaces/drivers/IIn.hpp"

namespace interfaces
{
	namespace drivers
	{
		class IExti : public IIn
		{
		  public:
			enum class TriggerEdge : uint8_t
			{
				none,
				falling,
				rising,
				both,
			};
			virtual void setTriggerEdge(TriggerEdge edge) = 0;
			void onExtiTrigger(std::function<void(bool state)> callback)
			{
				m_callback = callback;
			}

		  protected:
			void extiTrigger(bool pinState)
			{
				if (m_callback != nullptr)
					m_callback(pinState);
			}

		  private:
			std::function<void(bool)> m_callback;
		};
	} // namespace drivers
} // namespace interfaces
