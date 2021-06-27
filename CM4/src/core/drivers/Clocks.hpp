#pragma once
#include <cstdint>

#include "../vendor/st/stm32wl55xx.h"
#include "yggdrasil/interfaces/IClocks.hpp"

namespace core {
namespace drivers {
class Clocks: public interfaces::IClocks {
public:
	enum ClocksID {
		core = 0, apb1 = 1, apb2 = 2, apb3 = 3, hsi = 4,
	};

	bool init() override final
	{
		//test if was already initialized
		if (s_initialized != false)
			return true;

		FLASH->ACR =
				(FLASH->ACR & ~FLASH_ACR_LATENCY_Msk) | FLASH_ACR_LATENCY_1;
		while ((FLASH->ACR & FLASH_ACR_LATENCY_Msk) != FLASH_ACR_LATENCY_1) {
		}
		PWR->CR1 = (PWR->CR1 & ~PWR_CR1_VOS_Msk) | PWR_CR1_VOS_0;
		RCC->CR |= RCC_CR_HSION;
		/* Wait till HSI is ready */
		while ((RCC->CR & RCC_CR_HSIRDY_Msk) != RCC_CR_HSIRDY) {
		};

		RCC->ICSCR = (RCC->ICSCR & ~RCC_ICSCR_HSITRIM)
				| ((64) << RCC_ICSCR_HSITRIM_Pos);
		RCC->PLLCFGR = (RCC->PLLCFGR
				& ~(RCC_PLLCFGR_PLLSRC_Msk | RCC_PLLCFGR_PLLM_Msk
						| RCC_PLLCFGR_PLLN_Msk | RCC_PLLCFGR_PLLR_Msk))
				| (RCC_PLLCFGR_PLLSRC_1 | (12 << RCC_PLLCFGR_PLLN_Pos)
						| (0b011 << RCC_PLLCFGR_PLLR_Pos) | RCC_PLLCFGR_PLLREN);
		RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;
		RCC->CR |= RCC_CR_PLLON;
		/* Wait till PLL is ready */
		while ((RCC->CR & RCC_CR_PLLRDY) == RCC_CR_PLLRDY) {
		}

		RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW)
				| (RCC_CFGR_SW_1 | RCC_CFGR_SW_0);
		while ((RCC->CFGR & RCC_CFGR_SWS) != (RCC_CFGR_SWS_1 | RCC_CFGR_SWS_0)) {
		}

		RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_HPRE) | 0;
		RCC->EXTCFGR = (RCC->EXTCFGR & ~RCC_EXTCFGR_SHDHPRE) | (0 >> 4);
		RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE1) | 0;
		RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE2) | 0;

		s_clocks[ClocksID::core] = 48000000;
		s_clocks[ClocksID::apb1] = 48000000;
		s_clocks[ClocksID::apb2] = 48000000;
		s_clocks[ClocksID::apb3] = 48000000;
		s_initialized = true;     //initialisation finished without errors
		return true;
	}

	virtual uint32_t getSystemCoreFrequency() override final
	{
		return s_clocks[ClocksID::core];
	}

	virtual uint32_t getClockFrequency(uint32_t clockID) override final
	{
		return s_clocks[clockID];
	}

	uint32_t getPeripheralClock1Frequency() {
		return s_peripheralClock1;
	}

	uint32_t getPeripheralClock2Frequency() {
		return s_peripheralClock2;
	}

	uint32_t getHsiFrequency() {
		return s_hsi;
	}

	static Clocks& get() {
		return s_instance;
	}

	void setClockDeInitialized() {
		s_initialized = false;
	}

private:

	static uint32_t s_clocks[];
	static uint32_t &s_systemCoreClock;
	static uint32_t &s_peripheralClock1;
	static uint32_t &s_peripheralClock2;
	static uint32_t &s_peripheralClock3;
	static uint32_t &s_hsi;

	static Clocks s_instance;
	static bool s_initialized;
};
//End class Clocks
}// namespace drivers
} // End namespace core

