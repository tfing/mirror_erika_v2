///* ###*B*###
// * ERIKA Enterprise - a tiny RTOS for small microcontrollers
// *
// * Copyright (C) 2002-2008  Evidence Srl
// *
// * This file is part of ERIKA Enterprise.
// *
// * ERIKA Enterprise is free software; you can redistribute it
// * and/or modify it under the terms of the GNU General Public License
// * version 2 as published by the Free Software Foundation, 
// * (with a special exception described below).
// *
// * Linking this code statically or dynamically with other modules is
// * making a combined work based on this code.  Thus, the terms and
// * conditions of the GNU General Public License cover the whole
// * combination.
// *
// * As a special exception, the copyright holders of this library give you
// * permission to link this code with independent modules to produce an
// * executable, regardless of the license terms of these independent
// * modules, and to copy and distribute the resulting executable under
// * terms of your choice, provided that you also meet, for each linked
// * independent module, the terms and conditions of the license of that
// * module.  An independent module is a module which is not derived from
// * or based on this library.  If you modify this code, you may extend
// * this exception to your version of the code, but you are not
// * obligated to do so.  If you do not wish to do so, delete this
// * exception statement from your version.
// *
// * ERIKA Enterprise is distributed in the hope that it will be
// * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// * GNU General Public License version 2 for more details.
// *
// * You should have received a copy of the GNU General Public License
// * version 2 along with ERIKA Enterprise; if not, write to the
// * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// * Boston, MA 02110-1301 USA.
// * ###*E*### */

///*
// * Author: Dario Di Stefano, 2011
// */

//#include "cpu/hs12xs/inc/ee_compiler.h"

#ifdef __CODEWARRIOR__
 #define STACK_POINTER SP
#else
 #define STACK_POINTER s
#endif

#ifdef __CODEWARRIOR__
	xdef EE_s12xs_terminate_savestk
EE_s12xs_terminate_savestk:
#else
	xdef f_EE_s12xs_terminate_savestk
f_EE_s12xs_terminate_savestk:
#endif
	
#ifdef __CODEWARRIOR__
OFST:	set	0
	pshx
	pshd
	tfr STACK_POINTER,d				// transfer stack pointer in d register
	addd #$04						// add 2 (d+=2)
	std [OFST+7,STACK_POINTER]		// Save the stack pointer (from d to the terminate_data vector saved in the stack pointer)    
	call	[OFST+1,STACK_POINTER]	// Call the thread
	//puld				// Remove two bytes (the address of terminate_data) from the stack and put them in d (like a call, if the user doesnt'call the TerminateTask() function...)
	//pulx
	leas	4,STACK_POINTER				// remove 4 bytes from the stack (d,x previously pushed...)
	pulx // prendo l'indirizzo di ritorno
	pula // prendo l'indirizzo di ritorno
	puly // prendo l'indirizzo dal quale leggere lo stack pointer
	pshd 
	psha
	pshx
	tfr y,d
#else
	pshd				// Push (d, the first argument) in the stack the address of the memory location of the return stack pointer... 
OFST:	set	0
    //tfr STACK_POINTER,x			// transfer stack pointer in the register x
    //addx #$02			// add 2 (x+=2)	
    //tfr x,d			// transfer x in d
    tfr STACK_POINTER,d				// transfer stack pointer in d register
    addd #$02			// add 2 (d+=2)
    std [OFST+0,STACK_POINTER]		// Save the stack pointer (from d to the terminate_data vector saved in the stack pointer)
	call	[OFST+5,STACK_POINTER]	// Call the thread
	puld				// Remove two bytes (the address of terminate_data) from the stack and put them in d (like a call, if the user doesnt'call the TerminateTask() function...)
#endif

#ifdef __CODEWARRIOR__
	xdef EE_s12xs_terminate_task
EE_s12xs_terminate_task:
#else
	xdef f_EE_s12xs_terminate_task
f_EE_s12xs_terminate_task:
#endif
	
	tfr	d,y				// transfer d in y register
	
	//ldd	0,y			// transfer the content of y in d
	//tfr	d,STACK_POINTER			// Change the stack pointer (transfer d in sp)
	lds 0,y				// Change the stack pointer (transfer the content of y in sp)
	
    rtc					// Return in the thread stub (with ppage change)
    
	end