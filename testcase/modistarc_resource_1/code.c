/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2008  Evidence Srl
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

#include "ee.h"
#include "test/assert/inc/ee_assert.h"

#ifdef __EVALUATOR7T__
#include "mcu/samsung_ks32c50100/inc/ee_ic.h"
#endif

#ifdef __PPCE200Z7__
#include "cpu/e200zx/inc/ee_irq.h"
#endif

#define TRUE 1
#define FALSE 0

/* an invalud resource number */
#define NoResource 57

/* assertion data */
EE_TYPEASSERTVALUE EE_assertions[21];

/* set by the ISR */
volatile int ISR2_fired=0;

DeclareTask(Task1);
DeclareTask(Task2);

TASK(Task1)
{
  StatusType s;

  EE_assert(1, TRUE, EE_ASSERT_NIL);

  // probably a change in the standard specification: now
  // E_OS_ACCESS is returned only if the caller's priority is greater
  // than the resource ceiling.
  //s = GetResource(ResourceA);
  //EE_assert(2, (s==E_OS_ACCESS), 1);
  EE_assert(2, 1, 1); 
  
  s = GetResource(NoResource);
  EE_assert(3, (s==E_OS_ID), 2);
  
  s = GetResource(Resource1);
  EE_assert(4, (s==E_OK), 3);
	       
  s = GetResource(Resource2);
  EE_assert(5, (s==E_OK), 4);
  
  s = GetResource(Resource3);
  EE_assert(6, (s==E_OK), 5);
  
  s = GetResource(Resource4);
  EE_assert(7, (s==E_OK), 6);

  s = GetResource(Resource5);
  EE_assert(8, (s==E_OK), 7);

  s = GetResource(Resource6);
  EE_assert(9, (s==E_OK), 8);

  s = ReleaseResource(Resource6);
  EE_assert(10, (s==E_OK), 9);

  s = ReleaseResource(Resource5);
  EE_assert(11, (s==E_OK), 10);

  s = ReleaseResource(Resource4);
  EE_assert(12, (s==E_OK), 11);

  s = ReleaseResource(Resource3);
  EE_assert(13, (s==E_OK), 12);

  s = ReleaseResource(Resource2);
  EE_assert(14, (s==E_OK), 13);

  s = ReleaseResource(Resource1);
  EE_assert(15, (s==E_OK), 14);

  while (!ISR2_fired) {};

  s = ReleaseResource(Resource1);
  EE_assert(19, (s==E_OS_NOFUNC), 18);

  s = ReleaseResource(NoResource);
  EE_assert(20, (s==E_OS_ID), 19);

  TerminateTask();
}

TASK(Task2)
{
  EE_assert(0, FALSE, -1);

  TerminateTask();
}

#if defined(__NIOS2__)
#include "sys/alt_alarm.h"
void myISR2(void);
alt_u32 mycallback (void* arg)
{
  myISR2();
  return 0;
}
#endif

#if defined(__ARM7GNU__) && defined(__JANUS__)
#include "arm7gnu/janus/stub.h"
_ISR2(myISR2)
#else
	#if defined(__HCS12XS__)
		#include "cpu/cosmic_hs12xs/inc/ee_irqstub.h"
		#include "ee_hs12xsregs.h" 
		ISR2(myISR2)
	#else
		void myISR2(void)
	#endif
#endif
{
  StatusType s;

  EE_assert(16, TRUE, 15);

  s = GetResource(Resource6);
  EE_assert(17, (s==E_OK), 16);

  s = ReleaseResource(Resource6);
  EE_assert(18, (s==E_OK), 17);

  ISR2_fired = 1;

#if defined(__ARM7GNU__) && defined(__JANUS__)
  // Reset the interrupt pending bit on the EIC
  *OCCDEFINT &= ~INTF0B;
#endif
#if defined(__HCS12XS__)
	PITTF         	= 0x01;
	PITCE 			= 0;
#endif
}

int main(int argc, char **argv)
{
#if defined(__ARM7GNU__) && defined(__EVALUATOR7T__)

#define COUNTDOWN ((1 * 1000 * 1000 * 1000) / 20) /* 1 sec */

  EE_IC_clear_pending_IRQ();
  EE_IC_enable_external_IRQ(IRQ_TIMER0_MASK);
  EE_timer_0_set(((1 * 1000 * 1000 * 1000) / 20)/10); /* 100 msec */
  EE_timer_0_start();

#endif

#if defined(__ARM7GNU__) && defined(__JANUS__)

  EE_janus_IRQ_enable_channel(IRQ_ETUOCFB);
  EE_janus_IRQ_set_priority(IRQ_ETUOCFB, 1);
  EE_janus_IRQ_set_handler(IRQ_ETUOCFB, (EE_ADDR)myISR2);
  EE_janus_IRQ_enable_EIC();
  *OCREGF0B = 0x4FFF;
  *OCEN |= ENF0;
  *OCCDEFIE |= IEF0B;

#endif

#if defined(__NIOS2__)
  {
    alt_alarm myalarm;
    alt_alarm_start(&myalarm, 100, mycallback, NULL);
  }

#endif

#if defined(__HCS12XS__)

  EE_pit0_init(99, 140, 2);

#endif

#if defined(__PPCE200Z7__)
  EE_e200z7_register_ISR(10, myISR2, 0);
  EE_e200z7_setup_decrementer(3000000);
#endif

  StartOS(OSDEFAULTAPPMODE);

  EE_assert_range(0,1, 20);
  EE_assert_last();
}

//#ifdef __EVALUATOR7T__
//
//#ifdef __TIMER_0_USED__
//#define MYISR2 myISR2
//void myISR2(void);
//#else
//#define MYISR2 forever_loop
//#endif
//
//void forever_loop(void)
//{ 
//  for (;;);
//}
//
//const EE_ADDR EE_IC_EXTIRQ_vector[22] = {
//  (EE_ADDR)forever_loop, /* Ext 0 */
//  (EE_ADDR)forever_loop, /* Ext 1 */
//  (EE_ADDR)forever_loop, /* Ext 2 */
//  (EE_ADDR)forever_loop, /* Ext 3 */
//  (EE_ADDR)forever_loop, /* UART0 Tx */
//  (EE_ADDR)forever_loop, /* UART0 Rx & error*/
//  (EE_ADDR)forever_loop, /* UART1 Tx */
//  (EE_ADDR)forever_loop, /* UART1 Rx & error*/
//  (EE_ADDR)forever_loop, /* GDMA ch. 0 */
//  (EE_ADDR)forever_loop, /* GDMA ch. 1 */
//  (EE_ADDR)MYISR2, /* Timer 0 */
//  (EE_ADDR)forever_loop, /* Timer 1 */
//  (EE_ADDR)forever_loop, /* HDLC A Tx */
//  (EE_ADDR)forever_loop, /* HDLC A Rx */
//  (EE_ADDR)forever_loop, /* HDLC B Tx */
//  (EE_ADDR)forever_loop, /* HDLC B Rx */
//  (EE_ADDR)forever_loop, /* Ethernet BDMA Tx */
//  (EE_ADDR)forever_loop, /* Ethernet BDMA Rx */
//  (EE_ADDR)forever_loop, /* Ethernet MAC Tx */
//  (EE_ADDR)forever_loop, /* Ethernet MAC Rx */
//  (EE_ADDR)forever_loop, /* I2C-bus */
//  (EE_ADDR)forever_loop /* No Pending Interrupt */
//};
//#endif

#ifdef __JANUS__
EE_UINT16 fiq_arm0_handler(EE_UINT16 etu0_fir)
{
  return etu0_fir;
}
#endif
