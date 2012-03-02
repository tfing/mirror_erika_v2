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
 * Derived from the mico32 code.
 * Author: 2010 Fabio Checconi
 */

#ifndef __INCLUDE_E200ZX_IRQ_INTERNAL_H__
#define __INCLUDE_E200ZX_IRQ_INTERNAL_H__

#include "ee_internal.h"
#include "ee_irq.h"

/* IRQ handler */
void EE_e200z7_irq(EE_SREG level);

/*
 * ORTI ISR2 support
 */
typedef EE_e200z7_ISR_handler EE_ORTI_ISR2_magic_type;

#ifdef __OO_ORTI_ISR2__
extern volatile EE_ORTI_ISR2_magic_type EE_ORTI_ISR2_magic;

__INLINE__ EE_ORTI_ISR2_magic_type EE_ORTI_get_ISR2_magic(void)
{
	return EE_ORTI_ISR2_magic;
}

__INLINE__ void EE_ORTI_set_ISR2_magic(EE_ORTI_ISR2_magic_type isr2)
{
	EE_ORTI_ISR2_magic = isr2;
}

#else /* if __OO_ORTI_ISR2__ */

__INLINE__ EE_ORTI_ISR2_magic_type EE_ORTI_get_ISR2_magic(void)
{
	return NULL;
}

__INLINE__ void EE_ORTI_set_ISR2_magic(EE_ORTI_ISR2_magic_type isr2)
{
}

#endif /* else __OO_ORTI_ISR2__ */

#endif /* __INCLUDE_E200ZX_IRQ_INTERNAL_H__ */
