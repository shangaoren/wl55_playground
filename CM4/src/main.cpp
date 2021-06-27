#include "yggdrasil/kernel/Api.hpp"

using namespace kernel;

auto led = TaskWithStack<128>([](uint32_t) {
	//pb15
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODE15_Msk) | GPIO_MODER_MODE15_0;
	while (true) {
		GPIOB->BSRR |= GPIO_BSRR_BR15;
		Api::sleep(500);
		GPIOB->BSRR |= GPIO_BSRR_BS15;
		Api::sleep(500);
	}
}, 3, 0, true, "ledTest");

int main() {
	Api::setupKernel(2);
	core::Core::initCore();
	led.start();
	Api::startKernel();
}


