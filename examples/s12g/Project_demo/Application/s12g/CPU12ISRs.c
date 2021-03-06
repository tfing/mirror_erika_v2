/*****************************************************
 CPU12ISR.c - interrupt/exception handlers for HCS12    
 ----------------------------------------------------
  
 *****************************************************/

#include "hidef.h"       /* this file declares symbols user by the CodeWarrior environment */
#include "start12.h"     /* to call the C-startup code */
#include "CPU12ISRs.h"
#include "ee_s12regs.h"
#include "mcu/hs12xs/inc/ee_timer.h"
#include "inc/application.h"

extern volatile int timer_fired;
extern volatile int timer_divisor; 
extern volatile int button_fired;

/*************************************************************************************/

ISR2 ( SpuriousISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12ReservedISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12PortADISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12APIISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12ADCCompareISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12LVIISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12PortPISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12CAN0TxISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12CAN0RxISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12CAN0ErrISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12CAN0WakeISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12FlashCmdISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12FlashErrISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12SPI2ISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12SPI1ISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12SCI2ISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 (  CPU12PLLLockISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12OSCStatusISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12ACMPISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12PortJISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12ADCISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12SCI1ISR ) {
    _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12SCI0ISR ) {
    _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12SPI0ISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12PACCAIEISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12PACCAOVISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12TimerOVISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12TimerCh7ISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12TimerCh6ISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12TimerCh5ISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12TimerCh4ISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12TimerCh3ISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12TimerCh2ISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12TimerCh1ISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 (  CPU12TimerCh0ISR ) {
	StatusType Res;
	unsigned int val = EE_S12_TIMER_GET_TC0_REG();
	int diff;
	static int counter_tick;
	
	EE_timer_clear_ISRflag(EE_TIMER_0);
	_asm("cli");
	timer_divisor++;

	#ifndef __FP__
	
	if ( ( (signed)( EE_S12_TIMER_GET_CNT_REG() - EE_S12_TIMER_GET_TC0_REG() ) ) >= 0 ) { // to avoid spurious interrupts...
		do {
			counter_tick++;
			val += (unsigned int)EE_S12_TIMER_GET_INIT_PERIOD(EE_TIMER_0);
			EE_S12_TIMER_SET_TC0_REG(val); // to manage critical courses...
			diff = (signed)(EE_S12_TIMER_GET_CNT_REG() - val);
		} while( diff >= 0);
	}
	
	if (counter_tick == 15) {
		counter_tick = 0;
		timer_fired++;
		Res = ActivateTask(Task1);
	}
	
	#else
	
	if ( ( (signed)( EE_S12_TIMER_GET_CNT_REG() - EE_S12_TIMER_GET_TC0_REG() ) ) >= 0 ) { // to avoid spurious interrupts...
		do {
			counter_tick++;
			CounterTick(SystemTimer);
			val += (unsigned int)EE_S12_TIMER_GET_INIT_PERIOD(EE_TIMER_0);
			EE_S12_TIMER_SET_TC0_REG(val); // to manage critical courses...
			diff = (signed)(EE_S12_TIMER_GET_CNT_REG() - val);
		} while( diff >= 0);
	}
	
	#endif
	
	if( (counter_tick % 5) && EE_button_get_B1() ) {
		button_fired++;
		ActivateTask(Task2); 
	}
	
}

/*************************************************************************************/

ISR2 ( CPU12RTIISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12IRQISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12XIRQISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12SWIISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12TrapISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12COPISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

ISR2 ( CPU12ClockMonISR ) {
 _asm("bgnd");
}
/*************************************************************************************/

EE_S12_ISR void CPU12SystemReset(void) {
  asm jmp _Startup
 }	/* end CPU12SystemReset */
/*************************************************************************************/