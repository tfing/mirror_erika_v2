/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2010  Evidence Srl
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
 * CPU-dependent part of HAL
 * Derived from pkg/cpu/pic30/inc/ee_cpu.h
 * Author: 2009-2010,  Bernardo  Dal Seno
 */


#ifndef __INCLUDE_MICO32_EE_CPU_H__
#define __INCLUDE_MICO32_EE_CPU_H__

#include "eecfg.h"
#ifdef __GNUC__
#include "cpu/common/inc/ee_compiler_gcc.h"
#else
#error Unsupported compiler
#endif


/* This instruction should cause a trap when executed.  Handy to mark invalid
 * functions */
#define INVALID_ASM_INSTR  asm volatile ( ".word 0xcccccccc" )

/* Initial stack offest (in words) */
#ifndef DEBUG_STACK
/* If DEBUG_STACK, the macro is defined inside "ee_debug.h" */
#define MICO32_INIT_TOS_OFFSET 1
#endif


/*************************************************************************
 HAL Types and structures
 *************************************************************************/


/* Primitive data types */
#include "cpu/common/inc/ee_types.h"

typedef EE_UINT32 EE_UREG;
typedef EE_INT32  EE_SREG;
typedef EE_UINT32 EE_FREG;

/* Thread IDs */
typedef EE_INT32 EE_TID;
/* Used by the common layer to decide whether to start a new thread */
#define TID_IS_STACKED_MARK 0x80000000

/* EE_TYPEIRQ is currently unused */

/* XXX: define EE_TIME? */

/* Use the "standard" implementation */
#include "cpu/common/inc/ee_hal_structs.h"


/*********************************************************************
 MICO32 interrupt disabling/enabling
 *********************************************************************/


/* Used to check the value returned by EE_mico32_disableIRQ */
#define EE_mico32_are_IRQs_enabled(ie) ((ie) & 1)


/*
 * Enable interrupts
 */
__INLINE__ void __ALWAYS_INLINE__ EE_mico32_enableIRQ(void)
{
    EE_FREG oldie, newie;
    asm volatile ("rcsr %0,ie":"=r"(oldie));
    newie = oldie | (0x1);
    asm volatile ("wcsr ie, %0"::"r"(newie));
}


/*
 * Disable interrupts
 */
__INLINE__ EE_FREG __ALWAYS_INLINE__ EE_mico32_disableIRQ(void)
{
    EE_FREG oldie, newie;
    asm volatile ("rcsr %0,ie":"=r"(oldie));
    newie = oldie & (~0x1);
    asm volatile ("wcsr ie, %0"::"r"(newie));
    return oldie;
}


/*
 * Invalidate data cache
 */
__INLINE__ void __ALWAYS_INLINE__ EE_mico32_invalidate_dcache(void)
{
    /* The NOPs are needed to be sure that no load or store instruction
       immediately precedes or follows the `wcsr' instruction, as recommended by
       the LatticeMico32 Processor Reference Manual, page 17 */
    asm ("nop\n"
        "wcsr dcc, r0\n"
        "nop");
}


/*
 * Invalidate instruction cache
 */
__INLINE__ void __ALWAYS_INLINE__ EE_mico32_invalidate_icache(void)
{
    /* Four NOPs, as recommended by the LatticeMico32 Processor Reference
       Manual, page 17 */
    asm ("wcsr icc, r0\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop");
}


/*************************************************************************
 Functions exported by the HAL to the kernel
 *************************************************************************/


/* 
 * Interrupt Handling
 */

/* Disable/Enable Interrupts */
__INLINE__ void __ALWAYS_INLINE__ EE_hal_enableIRQ(void)
{
    EE_mico32_enableIRQ();
}

__INLINE__ void __ALWAYS_INLINE__ EE_hal_disableIRQ(void)
{
    EE_mico32_disableIRQ();
}

/* Cache invalidation */
#define EE_hal_invalidate_dcache EE_mico32_invalidate_dcache
#define EE_hal_invalidate_icache EE_mico32_invalidate_icache


/*************************************************************************
 Macros for debugging
 *************************************************************************/
#include "cpu/mico32/inc/ee_debug.h"


#endif /* __INCLUDE_MICO32_EE_CPU_H__ */