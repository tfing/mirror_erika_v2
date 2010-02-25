# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers
# 
# Copyright (C) 2002-2008  Evidence Srl
# 
# This file is part of ERIKA Enterprise.
# 
# ERIKA Enterprise is free software; you can redistribute it
# and/or modify it under the terms of the GNU General Public License
# version 2 as published by the Free Software Foundation, 
# (with a special exception described below).
# 
# Linking this code statically or dynamically with other modules is
# making a combined work based on this code.  Thus, the terms and
# conditions of the GNU General Public License cover the whole
# combination.
# 
# As a special exception, the copyright holders of this library give you
# permission to link this code with independent modules to produce an
# executable, regardless of the license terms of these independent
# modules, and to copy and distribute the resulting executable under
# terms of your choice, provided that you also meet, for each linked
# independent module, the terms and conditions of the license of that
# module.  An independent module is a module which is not derived from
# or based on this library.  If you modify this code, you may extend
# this exception to your version of the code, but you are not
# obligated to do so.  If you do not wish to do so, delete this
# exception statement from your version.
# 
# ERIKA Enterprise is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License version 2 for more details.
# 
# You should have received a copy of the GNU General Public License
# version 2 along with ERIKA Enterprise; if not, write to the
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301 USA.
# ###*E*###

ifeq ($(findstring __MICROCHIP_PIC32__,$(EEOPT)) , __MICROCHIP_PIC32__)

ifdef EE_SPI_PORT1_DMA_CHANNEL # NOTE: The variable is checked by RT-Druid
EEOPT+="EE_SPI_PORT1_DCHCON(m)=DCH$(EE_SPI_PORT1_DMA_CHANNEL)CON\#\#m"
EEOPT+="EE_SPI_PORT1_DCHECON(m)=DCH$(EE_SPI_PORT1_DMA_CHANNEL)ECON\#\#m"
EEOPT+="EE_SPI_PORT1_DCHINT(m)=DCH$(EE_SPI_PORT1_DMA_CHANNEL)INT\#\#m"
EEOPT+="EE_SPI_PORT1_DCHSSA(m)=DCH$(EE_SPI_PORT1_DMA_CHANNEL)SSA\#\#m"
EEOPT+="EE_SPI_PORT1_DCHDSA(m)=DCH$(EE_SPI_PORT1_DMA_CHANNEL)DSA\#\#m"
EEOPT+="EE_SPI_PORT1_DCHSSIZ(m)=DCH$(EE_SPI_PORT1_DMA_CHANNEL)SSIZ\#\#m"
EEOPT+="EE_SPI_PORT1_DCHDSIZ(m)=DCH$(EE_SPI_PORT1_DMA_CHANNEL)DSIZ\#\#m"
EEOPT+="EE_SPI_PORT1_DCHCSIZ(m)=DCH$(EE_SPI_PORT1_DMA_CHANNEL)CSIZ\#\#m"
EEOPT+="EE_SPI_PORT1_DMA_IE_MASK=_IEC1_DMA$(EE_SPI_PORT1_DMA_CHANNEL)IE_MASK"
EEOPT+="EE_SPI_PORT1_DMA_IF_MASK=_IFS1_DMA$(EE_SPI_PORT1_DMA_CHANNEL)IF_MASK"
EEOPT+="EE_SPI_PORT1_DMA_IP_MASK=_IPC9_DMA$(EE_SPI_PORT1_DMA_CHANNEL)IP_MASK"
EEOPT+="EE_SPI_PORT1_DMA_IS_MASK=_IPC9_DMA$(EE_SPI_PORT1_DMA_CHANNEL)IS_MASK"
EEOPT+="EE_SPI_PORT1_DMA_IP_POS=_IPC9_DMA$(EE_SPI_PORT1_DMA_CHANNEL)IP_POSITION"
EEOPT+="EE_SPI_PORT1_DMA_IS_POS=_IPC9_DMA$(EE_SPI_PORT1_DMA_CHANNEL)IS_POSITION"
EEOPT+="EE_SPI_PORT1_DMA_IRQ=_DMA$(EE_SPI_PORT1_DMA_CHANNEL)_VECTOR"
endif

ifdef EE_SPI_PORT2_DMA_CHANNEL # NOTE: The variable is checked by RT-Druid
EEOPT+="EE_SPI_PORT2_DCHCON(m)=DCH$(EE_SPI_PORT2_DMA_CHANNEL)CON\#\#m"
EEOPT+="EE_SPI_PORT2_DCHECON(m)=DCH$(EE_SPI_PORT2_DMA_CHANNEL)ECON\#\#m"
EEOPT+="EE_SPI_PORT2_DCHINT(m)=DCH$(EE_SPI_PORT2_DMA_CHANNEL)INT\#\#m"
EEOPT+="EE_SPI_PORT2_DCHSSA(m)=DCH$(EE_SPI_PORT2_DMA_CHANNEL)SSA\#\#m"
EEOPT+="EE_SPI_PORT2_DCHDSA(m)=DCH$(EE_SPI_PORT2_DMA_CHANNEL)DSA\#\#m"
EEOPT+="EE_SPI_PORT2_DCHSSIZ(m)=DCH$(EE_SPI_PORT2_DMA_CHANNEL)SSIZ\#\#m"
EEOPT+="EE_SPI_PORT2_DCHDSIZ(m)=DCH$(EE_SPI_PORT2_DMA_CHANNEL)DSIZ\#\#m"
EEOPT+="EE_SPI_PORT2_DCHCSIZ(m)=DCH$(EE_SPI_PORT2_DMA_CHANNEL)CSIZ\#\#m"
EEOPT+="EE_SPI_PORT2_DMA_IE_MASK=_IEC1_DMA$(EE_SPI_PORT2_DMA_CHANNEL)IE_MASK"
EEOPT+="EE_SPI_PORT2_DMA_IF_MASK=_IFS1_DMA$(EE_SPI_PORT2_DMA_CHANNEL)IF_MASK"
EEOPT+="EE_SPI_PORT2_DMA_IP_MASK=_IPC9_DMA$(EE_SPI_PORT2_DMA_CHANNEL)IP_MASK"
EEOPT+="EE_SPI_PORT2_DMA_IS_MASK=_IPC9_DMA$(EE_SPI_PORT2_DMA_CHANNEL)IS_MASK"
EEOPT+="EE_SPI_PORT2_DMA_IP_POS=_IPC9_DMA$(EE_SPI_PORT2_DMA_CHANNEL)IP_POSITION"
EEOPT+="EE_SPI_PORT2_DMA_IS_POS=_IPC9_DMA$(EE_SPI_PORT2_DMA_CHANNEL)IS_POSITION"
EEOPT+="EE_SPI_PORT2_DMA_IRQ=_DMA$(EE_SPI_PORT2_DMA_CHANNEL)_VECTOR"
endif

#ifeq ($(findstring __FRSH__,$(EEOPT)) , __FRSH__)
#EE_SRCS += pkg/mcu/microchip_pic32/src/ee_pic32frsh.c
#endif

ifeq ($(findstring __USE_TIMER__,$(EEOPT)) , __USE_TIMER__)
EE_SRCS += pkg/mcu/microchip_pic32/src/ee_timer.c
endif

ifeq ($(findstring __USE_UART__,$(EEOPT)) , __USE_UART__)
EE_SRCS += pkg/mcu/microchip_pic32/src/ee_uart.c
endif

ifeq ($(findstring __USE_SPI__,$(EEOPT)) , __USE_SPI__)
EE_SRCS += pkg/mcu/microchip_pic32/src/ee_spi.c
endif

ifeq ($(findstring __USE_I2C__,$(EEOPT)) , __USE_I2C__)
EE_SRCS += pkg/mcu/microchip_pic32/src/ee_i2c.c
endif

# typically empty, the __start function is typically provided by the
# ASM32 Assembler
# EE_BOOT_SRCS +=

endif