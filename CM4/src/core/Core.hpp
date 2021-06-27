#pragma once
#include <cstdint>
#ifdef SYSVIEW
#include "core/vectors/SysviewVectorManager.hpp"
#else
#include "core/vectors/DynamicVectorManager.hpp"
#endif

#include "./drivers/Systick.hpp"
#include "./drivers/Clocks.hpp"
#include "yggdrasil/kernel/Task.hpp"
#include "yggdrasil/kernel/ServiceCall.hpp"
#include "volund/system/Listenable.hpp"


namespace kernel
{
	class Event;
	class Mutex;
}


namespace core
{
	class Core
	{
	public:
		friend class kernel::Scheduler;
		friend class kernel::Event;
		friend class kernel::Mutex;

		// Core must be initialized before kernel start 
		static void initCore();
		
		static void enterStopMode();
		
		static void exitFromStopMode();
		
		static volund::system::listenable::ListenableList s_deepSleepNotifyList;
				
#ifdef SYSVIEW
		static implementation::vectors::SysviewVectorManager vectorManager;
#else
		static implementation::vectors::DynamicVectorManager vectorManager;
#endif
		
		static inline auto& systemTimer = core::drivers::Systick::get();
		
		static inline auto& coreClocks = core::drivers::Clocks::get();
		
		// System Interrupts
		static constexpr interfaces::Irq supervisorIrqNumber = -5;
		static constexpr interfaces::Irq taskSwitchIrqNumber = -2;
		
		static void restoreTask(volatile uint32_t* stackPointer);
		
		// Function executed by Idle Task, used to put the processor in sleep mode
		static constexpr std::size_t k_idleStackSize = 128;
		static void idleFunc(uint32_t);
		// Idle Task, used to put the device in sleep mode
		static inline ::kernel::TaskWithStack<k_idleStackSize> idleTask = 
			::kernel::TaskWithStack<k_idleStackSize>(idleFunc, 0, 0,true, "idle") ;
		
		// trigger contextSwitch by setting pendSv
		static void contextSwitchTrigger();
		
		static void contextSwitchHandler();
		
		static void supervisorCallHandler();
		
		static void systemTimerHandler();
		
		// HardFault interrupt function
		static void hardFault();

		// NMI interrupt function
		static void nmi();

		// usage Fault interrupt function
		static void usageFault();

		// bus fault interrupt function
		static void busFault();

		// get the number of the current interrupt (0 if thread mode)
		static uint32_t getCurrentInterruptNumber();

		// HardFaultAnalyzer, called by Hard fault interrupt to investigate
		static void HardFaultAnalyzer(uint32_t *stackPointer);	
		template <kernel::ServiceCall::SvcNumber Number, typename Return>
			static Return __attribute__((naked)) supervisorCall()
			{
				asm volatile("PUSH {LR}\n\t"
							 "DSB \n\t"
							 "SVC %[immediate]\n\t"
							 "POP {LR}\n\t"
							 "BX LR"::[immediate] "I"(Number));
			}

		template <kernel::ServiceCall::SvcNumber Number,typename Return, typename Param0>
		static Return __attribute__((naked)) supervisorCall(Param0 t_param0)
		{
			asm volatile("PUSH {LR}\n\t"
						 "DSB \n\t"
						 "SVC %[immediate]\n\t"
						 "POP {LR}\n\t"
						 "BX LR" ::[immediate] "I"(Number));
		}
		
		
		template<kernel::ServiceCall::SvcNumber Number, typename Return, typename Param0, typename Param1>
			static Return __attribute((naked)) supervisorCall(Param0 t_param0, Param1 t_param1)
			{
				asm volatile("PUSH {LR}\n\t"
				"DSB \n\t"
				"SVC %[immediate]\n\t"
				"POP {LR}\n\t"
				"BX LR"::[immediate] "I"(Number));
			}
		
		template<kernel::ServiceCall::SvcNumber Number, typename Return, typename Param0, typename Param1, typename Param2>
			static Return __attribute((naked)) supervisorCall(Param0 t_param0, Param1 t_param1, Param2 t_param2)
			{
				asm volatile("PUSH {LR}\n\t"
				"DSB \n\t"
				"SVC %[immediate]\n\t"
				"POP {LR}\n\t"
				"BX LR"::[immediate] "I"(Number));
			}
	};	//End class Core
} //End namespace core
