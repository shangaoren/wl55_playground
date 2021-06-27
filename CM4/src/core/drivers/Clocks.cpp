#include "Clocks.hpp"

namespace core
{
	namespace drivers
	{
		Clocks Clocks::s_instance;
		bool Clocks::s_initialized = false;
		uint32_t Clocks::s_clocks[] = { 4000000, 4000000, 4000000, 4000000 };    //default values at startup 4MHz for every clock
		uint32_t& Clocks::s_peripheralClock1 = s_clocks[ClocksID::apb1];
		uint32_t& Clocks::s_systemCoreClock = s_clocks[ClocksID::core];
		uint32_t& Clocks::s_peripheralClock2 = s_clocks[ClocksID::apb2];
		uint32_t& Clocks::s_hsi = s_clocks[ClocksID::hsi];
		
	} // namespace drivers	
} // namespace core
