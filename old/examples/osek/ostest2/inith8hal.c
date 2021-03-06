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

/*
 * Author: 2002 Davide Cantini
 * CVS: $Id: inith8hal.c,v 1.4 2006/01/17 13:18:22 pj Exp $
 */

#include "ee.h"

DeclareTask(thread0);
DeclareTask(thread1);
DeclareTask(thread2);
DeclareTask(thread3);


#ifdef __MULTI__

    /* stack start from bottom of RAM 0x8000-0xEF2F (28464 bytes)*/
    #define empty_stack 0xEF30
    
    /* IMPORTANT: if you need more than 256 stacks(?!), hal must be modified! */
    #define STACKID EE_UINT8
    
    /* number of used stacks */
    #define STACK_MAX (EE_MAX_TASK+1)
    
    /* stack size (in bytes) of each stack */
    #define stack1_size  256
    #define stack2_size  256
    #define stack3_size  256
    #define stack4_size  256  
    #define stack5_size  256
    
    /* NOTE:
     * maximum size of application (file .srec) < (0xEFFF - amout of stack used)
     * Consider this question when you set stack configuration.
     */
	
    /* initial sp of each stack */
    #define stack1_empty empty_stack
    #define stack2_empty stack1_empty-stack1_size
    #define stack3_empty stack2_empty-stack2_size
    #define stack4_empty stack3_empty-stack3_size    
    #define stack5_empty stack4_empty-stack4_size    

    /* id of each stack */
    #define stack1_id 0
    #define stack2_id 1
    #define stack3_id 2
    #define stack4_id 3
    #define stack5_id 4    
#endif


#if defined(__OO_BCC1__) || defined(__OO_BCC2__) || defined(__OO_ECC1__) || defined(__OO_ECC2__)

EE_UINT16 EE_terminate_data[EE_MAX_TASK];

/* ip of each real thread body (ROM) */
const EE_ADDR EE_terminate_real_th_body[EE_MAX_TASK] = {
    (EE_ADDR)Functhread0,
    (EE_ADDR)Functhread1,
    (EE_ADDR)Functhread2,
    (EE_ADDR)Functhread3
};

#endif


const EE_ADDR EE_hal_thread_body[EE_MAX_TASK] = {
    (EE_ADDR)EE_oo_thread_stub,
    (EE_ADDR)EE_oo_thread_stub,
    (EE_ADDR)EE_oo_thread_stub,
    (EE_ADDR)EE_oo_thread_stub
};

#ifdef __MULTI__
    /* each thread is assigned a stack_id (ROM) */
    const STACKID EE_h8_thread_stack_id[EE_MAX_TASK+1] = { 
	stack1_id,        /* ! stackid of main stack   ! */
        stack2_id,        /* ! stackid of thread0 stack ! */
	stack3_id,        /* ! stackid of thread1 stack ! */
	stack4_id,        /* ! stackid of thread2 stack ! */	
	stack5_id         /* ! stackid of thread3 stack ! */	
    };    

    /* sp of each stack (RAM)*/
    EE_ADDR EE_h8_sp[STACK_MAX] = {    
	(EE_ADDR) stack1_empty,   /* sp of stack1 */
	(EE_ADDR) stack2_empty,   /* sp of stack2 */
	(EE_ADDR) stack3_empty,   /* sp of stack3 */
	(EE_ADDR) stack4_empty,   /* sp of stack4 */
	(EE_ADDR) stack5_empty    /* sp of stack5 */
    };

    /* stack id for stack of running thread (RAM) */
    STACKID EE_h8_active_stack_id; 
#endif



