/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2011  Evidence Srl
 *
 * This file is part of ERIKA Enterprise.
 *
 * ERIKA Enterprise is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation, 
 * (with a special exception described below).
 *
 * Linking this code statically or dynamically with other modules is
 * making a combined work based on this code.  Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * As a special exception, the copyright holders of this library give you
 * permission to link this code with independent modules to produce an
 * executable, regardless of the license terms of these independent
 * modules, and to copy and distribute the resulting executable under
 * terms of your choice, provided that you also meet, for each linked
 * independent module, the terms and conditions of the license of that
 * module.  An independent module is a module which is not derived from
 * or based on this library.  If you modify this code, you may extend
 * this exception to your version of the code, but you are not
 * obligated to do so.  If you do not wish to do so, delete this
 * exception statement from your version.
 *
 * ERIKA Enterprise is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with ERIKA Enterprise; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 * ###*E*### */

/*
 * A minimal EE demo that demostrates how the nesting of the interrupts works.
 * This demo can be used to test nesting mechanism.
 * Author: 2012 Carlo Caione
 * Based on examples\cortex_mx\lpc12xx\porting_examples\monostack\EEtest5
 */

#include "ee.h"
#include "test/assert/inc/ee_assert.h"

#ifdef	__SIMUL__
#define	LEDn	0x04U

typedef enum 
{
  LED4 = 0,
  LED3 = 1,
  LED5 = 2,
  LED6 = 3
} Led_TypeDef;

volatile unsigned int leds[LEDn];

void STM_EVAL_LEDInit(unsigned int led) {
  if ( led < LEDn ) {
    leds[led] = 0;
  }
}

void STM_EVAL_LEDToggle(int led) {
  if ( led < LEDn ) {
    if (leds[led] == 0) {
      leds[led] = 1;
    }
    else {
      leds[led] = 0;
    }
  }
}

#define	EE_CORTEX_MX_TIM3_INT_NUM	EE_CORTEX_MX_INT_1D_NUM

#else
#include "stm32f4xx_conf.h"
#include "stm32f4_discovery.h"
#endif

#ifndef	TRUE
#define	TRUE	0x01U
#endif

/* Assertions */
enum EE_ASSERTIONS {
  EE_ASSERT_FIN = 0,
  EE_ASSERT_INIT,
  EE_ASSERT_TASK1_FIRED,
  EE_ASSERT_TIMER_ISR_FIRED,
  EE_ASSERT_SYSTICK_ISR_FIRED,
  EE_ASSERT_TASK1_ENDED,
  EE_ASSERT_TASK2_FIRED,
  EE_ASSERT_TASK2_ENDED,
  EE_ASSERT_TASKS_ENDED,
  EE_ASSERT_DIM
};
EE_TYPEASSERTVALUE EE_assertions[EE_ASSERT_DIM];

/* Final result */
EE_TYPEASSERTVALUE result;

/* Counters */
volatile int task1_fired = 0;
volatile int task2_fired = 0;
volatile int task1_ended = 0;
volatile int task2_ended = 0;
volatile int isr1_fired = 0;
volatile int isr2_fired = 0;
volatile int counter = 0;

/*
 * Timer ISR2
 */
ISR2(timer_handler)
{

#ifndef	__SIMUL__
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
#endif

  isr1_fired++;
  if (isr1_fired == 1) {
    EE_assert(EE_ASSERT_TIMER_ISR_FIRED, isr1_fired == 1, EE_ASSERT_TASK1_FIRED);
  }

#ifndef	__SIMUL__
  TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
  TIM_Cmd(TIM3, DISABLE);
#endif

  EE_systick_start();
  while (!(isr2_fired % 10));	/* Waits 1st ISR */
  while (isr2_fired % 10);	/* Waits 10th ISR */
  EE_systick_stop();

}

/*
 * SysTick ISR2
 */
ISR2(systick_handler)
{

  isr2_fired++;
  if (isr2_fired == 1) {
    EE_assert(
      EE_ASSERT_SYSTICK_ISR_FIRED, isr2_fired == 1, EE_ASSERT_TIMER_ISR_FIRED
    );
  }

  ActivateTask(Task2);

}

/*
 * TASK 1
 */
TASK(Task1)
{

  task1_fired++;
  if (task1_fired == 1) {
    EE_assert(EE_ASSERT_TASK1_FIRED, task1_fired == 1, EE_ASSERT_INIT);
  }

#ifndef	__SIMUL__
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM3, ENABLE);
#else
  NVIC_INT_SET_PENDING(EE_CORTEX_MX_TIM3_INT_NUM);
#endif

  while (task1_fired != isr1_fired);

  task1_ended++;
  if (task1_ended == 1) {
    EE_assert(
      EE_ASSERT_TASK1_ENDED, task1_ended == 1, EE_ASSERT_SYSTICK_ISR_FIRED
    );
  }

}

/*
 * TASK 2
 */
TASK(Task2)
{

  task2_fired++;
  if (task2_fired == 1) {
    EE_assert(EE_ASSERT_TASK2_FIRED, task2_fired == 1, EE_ASSERT_TASK1_ENDED);
  }

  task2_ended++;
  if (task2_ended == 1) {
    EE_assert(EE_ASSERT_TASK2_ENDED, task2_ended == 1, EE_ASSERT_TASK2_FIRED);
  }

}

#ifndef	__SIMUL__
/*
 * Initialize Timer
 */
void timer_init()
{

  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  TIM_TimeBaseStructure.TIM_Period = (1000 - 1);
  TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / 2) - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

}
#endif

/*
 * MAIN TASK
 */
int main(void)
{
#ifndef	__SIMUL__
  SystemInit();
#endif

  /*Initializes Erika related stuffs*/
  EE_system_init();

#ifndef	__SIMUL__
  timer_init();
#endif

  STM_EVAL_LEDInit(LED3);
  EE_systick_set_period(1000000);
  EE_systick_enable_int();

  EE_assert(EE_ASSERT_INIT, TRUE, EE_ASSERT_NIL);

  ActivateTask(Task1);

  EE_assert(
    EE_ASSERT_TASKS_ENDED, task1_ended && task2_ended, EE_ASSERT_TASK2_ENDED
  );
  EE_assert_range(EE_ASSERT_FIN, EE_ASSERT_INIT, EE_ASSERT_TASKS_ENDED);
  result = EE_assert_last();

  /* Forever loop: background activities (if any) should go here */
  for (;result == 1;)
  {

    while (counter % 10000) counter++;
    STM_EVAL_LEDToggle(LED3);
    ActivateTask(Task1);
    counter++;

  }

}
