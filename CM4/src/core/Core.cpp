#include "Core.hpp"
#include "yggdrasil/kernel/Scheduler.hpp"
#include "yggdrasil/kernel/Api.hpp"

namespace core
{

#ifdef SYSVIEW
	implementation::vectors::SysviewVectorManager Core::vectorManager;
#else
	implementation::vectors::DynamicVectorManager Core::vectorManager;
#endif
	
	void Core::initCore()
	{
		vectorManager.registerHandler(HardFault_IRQn, hardFault);
		vectorManager.registerHandler(NonMaskableInt_IRQn, nmi);
		vectorManager.registerHandler(UsageFault_IRQn, usageFault);
		vectorManager.registerHandler(BusFault_IRQn, busFault);
			
		coreClocks.init();
	}
	
	void Core::enterStopMode()
	{
		systemTimer.stopSystemTimer();
		coreClocks.setClockDeInitialized();
	}
	
	void Core::exitFromStopMode()
	{
		coreClocks.init();
		systemTimer.startSystemTimer();

	}
	
	/********************************************************
	*						PRIVATE MEMBERS					*
	********************************************************/
	volund::system::listenable::ListenableList Core::s_deepSleepNotifyList;
	
	/********************************************************
	 *						PRIVATE FUNCTIONS				*
	 ********************************************************/
	
	
	void __attribute__((Optimize("O0"))) Core::restoreTask(volatile uint32_t* stackPointer)
	{
		asm volatile(
		"MOV R0,%0\n\t"			 //load stack pointer from task.stackPointer
		"LDMIA R0!,{R2-R11}\n\t" //restore registers R4 to R11
		"MOV LR,R2\n\t"			 //reload Link register
		"MSR CONTROL,R3\n\t"	 //reload CONTROL register
		"ISB\n\t"				 //Instruction synchronisation Barrier is recommended after changing CONTROL
		"MSR PSP,R0\n\t"		 //reload process stack pointer with task's stack pointer
		"BX LR"::"r"(stackPointer));
	}
	
	void Core::idleFunc(uint32_t)
	{
		while (true)
		{
			__WFI();
		}
	}
	
	void Core::contextSwitchTrigger()
	{
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
		//__ISB();
	}
	
	void __attribute__((naked, Optimize("O0"))) Core::contextSwitchHandler()
	{
		__asm volatile(
			"CPSID I\n\t"
			"DSB\n\t"
			"MRS R0, PSP\n\t"				   //store Process Stack pointer into R0
#ifdef FPU
			//"TST LR, #0x10\n\t"				   // test Bit 4 of Link return to know if floating point was used , if 0 save FP
			//"IT EQ\n\t"
			//"VSTMDBEQ R0!, {S16-S31}\n\t" // save floating point registers
#endif
			"MOV R2,LR\n\t"				  //store Link Register into R2
			"MRS R3,CONTROL\n\t"		  //store CONTROL register into R3
			"STMDB R0!,{R2-R11}\n\t"	  //store R2 to R11 memory pointed by R0 (stack), increment memory and rewrite the new adress to R0
			"bl %[changeTask]\n\t"			// go to change task
			"LDMIA R0!,{R2-R11}\n\t"	  //restore registers R4 to R11 (R0 was loaded by change task with the new stackpointer
			"MOV LR,R2\n\t"				  //reload Link register
			"MSR CONTROL,R3\n\t"		  //reload CONTROL register
			"ISB\n\t"					  //Instruction synchronisation Barrier is recommended after changing CONTROL
#ifdef FPU
			//"TST LR, #0x10\n\t"			  // test Bit 4 to know if FPU has to be restored, unstack if 0
			//"IT EQ\n\t"
			//"VLDMIAEQ R0!,{S16-S31}\n\t" //restore floating point registers
#endif
			"MSR PSP,R0\n\t"			 //reload process stack pointer with task's stack pointer
			"CPSIE I\n\t" //unlock Interrupts
			"ISB \n\t"
			"BX LR"
					:   // no outputs
		: [changeTask]"g"(kernel::Scheduler::taskSwitch)
			: "memory");
		}
	
	void Core::systemTimerHandler()
	{
		kernel::Scheduler::systemTimerTick();
	}
	

	void __attribute__((naked, Optimize("O0"))) Core::supervisorCallHandler()
	{
		__asm volatile(
			"TST LR,#4\n\t"		//test bit 2 of EXC_RETURN to know if MSP or PSP
			"ITE EQ\n\t"		//was used for stacking
			"MRSEQ R1, MSP\n\t" //place msp or psp in R0 as parameter for SvcHandler
			"MRSNE R1, PSP\n\t"
			"LDR R0,[R1,#24]\n\t"
			"LDRB R0, [R0, #-2]\n\t"
			"PUSH {LR}\n\t" //stack LR to be able to return for exception
			"bl %[svc]\n\t"
			"POP {LR}\n\t"
			"BX LR"
			:    // no outputs
			: [svc] "g"(kernel::Scheduler::supervisorCall) :);
	}
	
	
	void Core::hardFault()
	{
		uint32_t *stackedPointer;
		asm volatile(
			"TST LR,#4\n\t"		//test bit 2 of EXC_RETURN to know if MSP or PSP
			"ITE EQ\n\t"		//was used for stacking
			"MRSEQ R0, MSP\n\t" //place msp or psp in R0 as parameter for HardfaultAnalyser
			"MRSNE R0, PSP\n\t"
			"MOV %0,R0\n\t"
			"PUSH {LR}\n\t" //stack LR to be able to return for exception
			: "=r"(stackedPointer)
			:);  
		HardFaultAnalyzer(stackedPointer);
		asm volatile("POP {PC}");    //return from exception
	}
	
	void Core::nmi()
	{
		__BKPT(0); 	//you fucked up
	}
	
	void Core::usageFault()
	{
		__BKPT(0); 	//you fucked up
	}
	
	void Core::busFault()
	{
		__BKPT(0); 	//you fucked up
	}

	uint32_t Core::getCurrentInterruptNumber()
	{
		return __get_IPSR();
	}

	void Core::HardFaultAnalyzer(uint32_t *stackPointer)
	{
		volatile uint32_t R0, R1, R2, R3, R12, LR, PC, xPSR, previousSp;
		R0 = stackPointer[0];
		R1 = stackPointer[1];
		R2 = stackPointer[2];
		R3 = stackPointer[3];
		R12 = stackPointer[4];
		LR = stackPointer[5];
		PC = stackPointer[6];
		xPSR = stackPointer[7];
		previousSp = stackPointer[8];

		uint32_t *MMAFaultAddress, *BusFaultAddress;

		// Mem Manage Fault
		if((SCB->CFSR & SCB_CFSR_MMARVALID_Msk) == SCB_CFSR_MMARVALID_Msk)
		{
			MMAFaultAddress = (uint32_t *)(SCB->MMFAR);
			__BKPT(0);   // MMAR holds Valid Address Fault
		}

		if ((SCB->CFSR & SCB_CFSR_MSTKERR_Msk) == SCB_CFSR_MSTKERR_Msk)
			__BKPT(0);   // MemManage Fault on stacking for exception entry

		if((SCB->CFSR & SCB_CFSR_MUNSTKERR_Msk) == SCB_CFSR_MUNSTKERR_Msk)
			__BKPT(0);   // MemManage Fault on unstacking for return from exception

		if((SCB->CFSR & SCB_CFSR_DACCVIOL_Msk) == SCB_CFSR_DACCVIOL_Msk)
			__BKPT(0);   // Data Access Violation Flag

		if((SCB->CFSR & SCB_CFSR_IACCVIOL_Msk) == SCB_CFSR_IACCVIOL_Msk)
			__BKPT(0);   // Instruction access Violation Flag

		//Bus Fault
		if((SCB->CFSR & SCB_CFSR_BFARVALID_Msk) == SCB_CFSR_BFARVALID_Msk)
		{
			BusFaultAddress = (uint32_t *)(SCB->BFAR);
			__BKPT(0);   // BFAR Hold Valid Fault Address
		}

		if ((SCB->CFSR & SCB_CFSR_STKERR_Msk) == SCB_CFSR_STKERR_Msk)
			__BKPT(0);   // BUsFault on Stacking

		if((SCB->CFSR & SCB_CFSR_UNSTKERR_Msk) == SCB_CFSR_UNSTKERR_Msk)
			__BKPT(0);   // BusFault on Unstacking

		if((SCB->CFSR & SCB_CFSR_IMPRECISERR_Msk) == SCB_CFSR_IMPRECISERR_Msk)
			__BKPT(0);   // Imprecise Data Bus Error

		if((SCB->CFSR & SCB_CFSR_PRECISERR_Msk) == SCB_CFSR_PRECISERR_Msk)
			__BKPT(0);   // Precise Data Bus Error

		if((SCB->CFSR & SCB_CFSR_IBUSERR_Msk) == SCB_CFSR_IBUSERR_Msk)
			__BKPT(0);   // Instruction Bus Error

		// Usage Fault
		if((SCB->CFSR & SCB_CFSR_DIVBYZERO_Msk) == SCB_CFSR_DIVBYZERO_Msk)
			__BKPT(0);   // Divide By Zero Fault

		if((SCB->CFSR & SCB_CFSR_UNALIGNED_Msk) == SCB_CFSR_UNALIGNED_Msk)
			__BKPT(0);   // Unaligned Memory Access

		if((SCB->CFSR & SCB_CFSR_NOCP_Msk) == SCB_CFSR_NOCP_Msk)
			__BKPT(0);   // No Coprocessor Usage Fault

		if((SCB->CFSR & SCB_CFSR_INVPC_Msk) == SCB_CFSR_INVPC_Msk)
			__BKPT(0);   // Invalid PC UsageFault

		if((SCB->CFSR & SCB_CFSR_INVSTATE_Msk) == SCB_CFSR_INVSTATE_Msk)
			__BKPT(0);   // Invalid state

		if((SCB->CFSR & SCB_CFSR_UNDEFINSTR_Msk) == SCB_CFSR_UNDEFINSTR_Msk)
			__BKPT(0);   // Undefined Instruction
	}
}// End namespace core
