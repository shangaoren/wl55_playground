#pragma once
#include <cstdint> 
#include "yggdrasil/interfaces/IVectorsManager.hpp"
#include "yggdrasil/interfaces/ISystemTimer.hpp"

#include "core/vendor/st/stm32wl55xx.h"
#include "core/vendor/arm/core_cm4.h"

namespace core
{
	namespace drivers
	{
		class Systick : public core::interfaces::ISystemTimer
		{
		public:
			virtual void initSystemTimer(uint32_t coreFrequency, uint32_t ticksFrequency) final
			{
				SysTick->LOAD = (SysTick->LOAD & ~SysTick_LOAD_RELOAD_Msk) | (((coreFrequency / ticksFrequency) - 1u) << SysTick_LOAD_RELOAD_Pos);
				SysTick->VAL = (SysTick->VAL & ~SysTick_VAL_CURRENT_Msk) | (0 << SysTick_VAL_CURRENT_Pos);
				SysTick->CTRL = (SysTick->CTRL & ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk )) | ((1U << SysTick_CTRL_CLKSOURCE_Pos)| (1U << SysTick_CTRL_TICKINT_Pos));
			}
			
			virtual void startSystemTimer() final
			{
				SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
			}
			
			virtual void stopSystemTimer() final
			{
				SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
			}
			
			virtual core::interfaces::Irq getIrq() final
			{
				return SysTick_IRQn;
			}
			
			constexpr static Systick& get()
			{
				return s_self;
			}
			
		private:
			static Systick s_self;
			
		};
	}	//End namespace drivers
}	//End namespace core
