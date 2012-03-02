/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2012  Evidence Srl
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

#ifndef EE_INCLUDE_ISR_DEFINE_H
#define EE_INCLUDE_ISR_DEFINE_H

#ifdef __STATIC_ISR_TABLE__
/* Workaround to introduce Standard ISR macro definition */
#define ISR(f) ISR##f(f)

/* Those shall be defined by RT-Druid*/
#define ISRIsrLow(f)        ISR2(f)
#define ISRIsrMedium(f)     ISR2(f)

#define ISRIsrHigh(f)       ISR1(f)

/* FOLLOWING DEFINES ARE NOT USED BY code.c ARE HERE ONLY AS REMINDER */
/* Handlers mapping */
#define EE_PPCE200ZX_0_ISR IsrLow
#define EE_PPCE200ZX_3_ISR IsrMedium
#define EE_PPCE200ZX_5_ISR IsrHigh

#else /* __STATIC_ISR_TABLE__ */

#define ISR(f) void f(void)

#endif /* __STATIC_ISR_TABLE__ */

/* Priorities mapping */
#define EE_PPCE200ZX_0_ISR_PRI 1
#define EE_PPCE200ZX_3_ISR_PRI 2
#define EE_PPCE200ZX_5_ISR_PRI 3

#endif /* EE_INCLUDE_ISR_DEFINE_H */
