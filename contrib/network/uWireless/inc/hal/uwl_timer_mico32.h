/** 
* @file uwl_timer_mico32.h
* @brief Timer Implementation for mico32
* @author Christian Nastasi
* @author Daniele Alessandrelli
* @version 0.1
* @date 2010-02-08
*/
#ifndef __uwl_timer_mico32_h__
#define __uwl_timer_mico32_h__

#include <hal/uwl_timer.h>
#include "mcu/mico32/inc/ee_timer.h"
#include <MicoMacros.h>

#ifndef ISR2
#define ISR2(func) void func(void)
#endif

#define UWL_HAL_TIMER_ID 2
#define UWL_TIMER_INTERRUPT_NAME uwl_timer_ISR_cbk
#define UWL_TIMER_INTERRUPT_FLAG_CLEAR()

#define DECLARE_FUNC_UWL_TIMER(uc, lc) \
COMPILER_INLINE int uwl_hal_timer_start(void) { \
	return cat3(EE_, lc, _on)(); } \
COMPILER_INLINE int uwl_hal_timer_stop(void) { \
	return cat3(EE_, lc, _off)(); } \
COMPILER_INLINE int uwl_hal_timer_set_isr_callback(void (*func)(void)) { \
	return cat3(EE_, lc, _set_ISR_callback)(func); } \
COMPILER_INLINE int uwl_hal_timer_init(int per, int set) { \
	return cat3(EE_, lc, _init)(per, set); }

#if (UWL_HAL_TIMER_ID == 1)
DECLARE_FUNC_UWL_TIMER(EE_TIMER1_NAME_UC, EE_TIMER1_NAME_LC)
#elif (UWL_HAL_TIMER_ID == 2)
DECLARE_FUNC_UWL_TIMER(EE_TIMER2_NAME_UC, EE_TIMER2_NAME_LC)
#elif (UWL_HAL_TIMER_ID == 3)
DECLARE_FUNC_UWL_TIMER(EE_TIMER3_NAME_UC, EE_TIMER3_NAME_LC)
#elif (UWL_HAL_TIMER_ID == 4)
DECLARE_FUNC_UWL_TIMER(EE_TIMER4_NAME_UC, EE_TIMER4_NAME_LC)
#else
#error "UWL_HAL ERROR: invalid timer hw interface specified"
#endif
	
int8_t uwl_timer_init(uint32_t period, uint32_t fcy);
void uwl_timer_set_isr_callback(void (*func)(void));
COMPILER_ISR(UWL_TIMER_INTERRUPT_NAME);

COMPILER_INLINE void uwl_timer_start(void)
{
    //	UWL_TIMER_INTERRUPT_FLAG_CLEAR();
	//  UWL_TIMER_INTERRUPT_ENABLE();
	//  UWL_TIMER_MODULE_ON();
	uwl_hal_timer_start();
}

COMPILER_INLINE void uwl_timer_stop(void)
{
    //	UWL_TIMER_INTERRUPT_DISABLE();
	//  UWL_TIMER_MODULE_OFF();
	//  UWL_TIMER_INTERRUPT_FLAG_CLEAR();
	uwl_hal_timer_stop();
}



#endif /* Header Protection */
