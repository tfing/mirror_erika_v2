/** 
* @file ee_uart.c
*/

#include "mcu/mico32/inc/ee_uart.h"
#include <cpu/mico32/inc/ee_irq.h>

/******************************************************************************/
/*                             Utility Macros                                 */
/******************************************************************************/
// ...

/******************************************************************************/
/*                       Private Local Variables                              */
/******************************************************************************/
/* Vectors and uart structures definitions */
#ifdef EE_UART1_NAME_UC
DEFINE_VET_UART(EE_UART1_NAME_UC, EE_UART1_NAME_LC)
DEFINE_STRUCT_UART(EE_UART1_NAME_UC, EE_UART1_NAME_LC)
#endif

#ifdef EE_UART2_NAME_UC
DEFINE_VET_UART(EE_UART2_NAME_UC, EE_UART2_NAME_LC)
DEFINE_STRUCT_UART(EE_UART2_NAME_UC, EE_UART2_NAME_LC)
#endif

volatile int ee_uart_tip=0;

/******************************************************************************/
/*                       Private Local Functions                              */
/******************************************************************************/
// ...

/******************************************************************************/
/*                              ISRs                                          */
/******************************************************************************/
// Interrupt common handler:
void EE_uart_common_handler(int level)
{
    unsigned int iir;
    unsigned int spint;
    int ret;
    EE_UINT8 data = 0;
    
    EE_uart_st *usp = EE_get_uart_st_from_level(level);
    MicoUart_t *uartc = usp->base; 

    /* interrogate interrupt-cause */
    iir = uartc->iir;
    
    /* check the interrupt source */
    switch(iir){
        case MICOUART_IIR_RXRDY:	/* the interrupt is due to rx-data */
		{
            /* 
	         * read lsr to check for errors associated with current word. 
	         * reading lsr clears error bits 
	         */
            //lsr = uartc->lsr;
            //if((lsr & (MICOUART_LSR_PE_MASK | MICOUART_LSR_FE_MASK)) == 0) if(uiValue & MICOUART_LSR_RX_RDY_MASK)
            data = uartc->rxtx;
            ret = EE_buffer_putmsg(&(usp->rxbuf), &data);
            if(ret==EE_BUF_ERR_FULL) 
				usp->err = EE_UART_ERR_RX_BUF_FULL;  
            /* Call user Callback */
            if(usp->rxcbk != EE_NULL_CBK)
            	usp->rxcbk();
        }break;
        case MICOUART_IIR_TXRDY:	/* the interrupt is due to tx-data */
		{
			spint = 1;
			/* Loop to empty the buffer */
			while(!EE_buffer_isempty(&(usp->txbuf)))
			{
				EE_buffer_getmsg(&(usp->txbuf), &data);
				while( !((uartc->lsr) & MICOUART_LSR_TX_RDY_MASK) )
					;	
				uartc->rxtx = data;
				if(spint) spint = 0;	// is not a spurious interrupt...
			}
			
			/* To avoid interrupt is again asserted by the UART after reading the IIR register, 
	   		   we must to disable TX isr-source. NOTE: Could start spurious tx interrupt... */
	   		if(EE_uart_enabled_rx_int(usp->mode))
	   			uartc->ier = (volatile unsigned int)(MICOUART_IER_RX_INT_MASK);
	   		else
				uartc->ier = 0;
			ee_uart_tip = 0;							// transmission not in progress...
				
			/* Call user Callback (not for spurious interrupt...) */
	       	if( (usp->txcbk != EE_NULL_CBK) && (!spint) )
            	usp->txcbk();
            		
        }break;
        default:{
            /* 
             * This will never happen unless someone's reading rx
             * in parallel.  If using interrupts, isr is the only
             * code that should be reading rx
             */
        }break;
    }

    return;
}


/******************************************************************************/
/*                       Public Global Functions                              */
/******************************************************************************/
/* This function records ISR handler */
int EE_hal_uart_handler_setup(EE_uart_st* usp)
{
    /* Register IRQ handler */
    EE_mico32_register_ISR(usp->irqf, EE_uart_common_handler);	 

	return EE_UART_OK;
}

/* This function configures UART baudrate and other features (parity, stop bits...) */
int EE_hal_uart_config(EE_uart_st* usp, int baudrate, int settings)
{
	unsigned int iir;
	MicoUart_t *uartc = usp->base; 
	
	/* reset ier (isr register) */
    uartc->ier = 0;						// if ier==0 -> POLLING MODE (ATT! is a blocking mode!!!)
    									// if ier!=0 -> ISR MODE (ATT! is not a blocking mode!!!)
    iir = uartc->iir;					// read iir register to clean ISR flags.	FARE PROVA!!!
	/* Register IRQ handler */
    EE_hal_uart_handler_setup(usp);
	/* set the control register */
    uartc->lcr = settings;    
    /* Calculate clock-divisor */
    uartc->div = (MICO32_CPU_CLOCK_MHZ)/baudrate;
    
    /* ISR management */
    return EE_UART_OK;	//EE_uart_set_ISR_callback_base(base, irq_flag, ie_flag, isr_rx_callback, isr_tx_callback);
}

/* This function sets UART operating mode */
int EE_hal_uart_set_mode(EE_uart_st* usp, int mode)
{
	int old_mode;
	unsigned int intst;
	
	MicoUart_t *uartc = usp->base; 
	
	old_mode = usp->mode;
	if(old_mode == mode)
		return EE_UART_OK;
	
	/* Disble IRQ */
	intst = EE_mico32_disableIRQ();
	
	/* Register IRQ handler */
	EE_hal_uart_handler_setup(usp);
	usp->mode = mode; 	
	/* Buffer initialization */
	if (EE_uart_need_init_rx_buf(old_mode, mode))
		EE_buffer_init(&usp->rxbuf, EE_UART_MSGSIZE, EE_UART_BUFSIZE, usp->rxbuf.data);
	if (EE_uart_need_init_tx_buf(old_mode, mode))
		EE_buffer_init(&usp->txbuf, EE_UART_MSGSIZE, EE_UART_BUFSIZE, usp->txbuf.data);
	/* IRQ settings */
	if(EE_uart_need_enable_int(mode))
		mico32_enable_irq(usp->irqf);
	else
		mico32_disable_irq(usp->irqf);
	if(EE_uart_need_enable_rx_int(old_mode, mode))
		uartc->ier = MICOUART_IER_RX_INT_MASK;	// check if we lose data when ier is written...	
	else
		uartc->ier = 0;

	/* Enable IRQ */
	if (EE_mico32_are_IRQs_enabled(intst))
        EE_mico32_enableIRQ();
        
	return EE_UART_OK;
}

/* This functions sets rx callback */
int EE_hal_uart_set_rx_callback(EE_uart_st* usp, EE_ISR_callback isr_rx_callback)
{
	usp->rxcbk = isr_rx_callback;
	
	return EE_UART_OK;
}

/* This functions sets tx callback */
int EE_hal_uart_set_tx_callback(EE_uart_st* usp, EE_ISR_callback isr_tx_callback)
{
	usp->txcbk = isr_tx_callback;
	
	return EE_UART_OK;
}

int EE_hal_uart_enable_tx_ISR(EE_uart_st* usp)
{
	unsigned int intst, mode = usp->mode;;
	MicoUart_t *uartc = usp->base; 
	
	intst = EE_mico32_disableIRQ();				
	ee_uart_tip = 1;								// transmission in progress...
	if(EE_uart_enabled_rx_int(mode))
		uartc->ier = (volatile unsigned int)(MICOUART_IER_RX_INT_MASK | MICOUART_IER_TX_INT_MASK);
	else
		uartc->ier = (volatile unsigned int)(MICOUART_IER_TX_INT_MASK);	// Enable interrupts to empty the tx buffer...
	if (EE_mico32_are_IRQs_enabled(intst))		
	   	EE_mico32_enableIRQ();
	   	
	return EE_UART_OK;	
}




/* This functions can be used to write a character */
/*
	- polling mode: 
		- blocking mode: wait until tx is ready, return 1
		- no-blocking mode: no wait, return EE_UART_ERR_TX_NOT_READY
	- isr mode:
		- blocking mode: wait until tx buffer is ready, return 1
		- no-blocking mode: no wait, return EE_UART_ERR_TX_BUF_FULL
*/
int EE_hal_uart_write_byte(EE_uart_st* usp, EE_UINT8 data)
{
	unsigned int uiValue, mode;
	EE_buffer* buffer;
	int ret;
  
	MicoUart_t *uartc = usp->base; 
	
	buffer = &usp->txbuf;
	mode = usp->mode;
	
	if( EE_uart_tx_polling(mode) )						// TX POLLING MODE
	{						
		do
		{
			/* if uart's ready to accept character, send immediately */
    		uiValue = uartc->lsr;
    		if(uiValue & MICOUART_LSR_TX_RDY_MASK)
			{
    			uartc->rxtx = data;						// one byte transmitted
    			ret = 1;		
    			usp->err = EE_UART_OK;
    			break;
			}
			else 
			{
				usp->err = EE_UART_ERR_TX_NOT_READY;
				if(!EE_uart_tx_block(mode))				// if no blocking mode
				{		
					ret = EE_UART_ERR_TX_NOT_READY;		// one byte transmitted
					break;
				}
			}
		}while(1);	
	}
	else
	{													// TX ISR MODE
		do
		{
			ret = EE_buffer_putmsg(buffer, &data);	
			if(ret == EE_BUF_OK)
			{											// one byte loaded in the buffer
				ret = 1;	
				usp->err = ret;
				break;
			}
			else
			{
				usp->err = EE_UART_ERR_TX_BUF_FULL;
				if(!EE_uart_tx_block(mode))				// if no blocking mode
				{			
					ret =  EE_UART_ERR_TX_BUF_FULL;
					break;
				}
				else
					while(ee_uart_tip)
						;					// wait until transmission is finished
			}
		}while(1);
		EE_hal_uart_enable_tx_ISR(usp);					// enable tx interrupt
	}
	
	// All done!!!
	return ret;
}

/* This functions can be used to read a character */
/*
	- polling mode: 
		- blocking mode: wait until rx is ready, return 1
		- no-blocking mode: no wait, return EE_UART_ERR_RX_NOT_READY
	- isr mode:
		- blocking mode: wait until rx buffer is ready, return 1
		- no-blocking mode: no wait, return EE_UART_ERR_RX_BUF_EMPTY
*/
int EE_hal_uart_read_byte(EE_uart_st* usp, EE_UINT8 *data)
{
	unsigned int uiValue; 
	int mode;
	int ret;
	EE_buffer* buffer;
	
	MicoUart_t *uartc = usp->base; 

	buffer = &usp->rxbuf;
	mode = usp->mode;
	
	if( EE_uart_rx_polling(mode) )								// RX POLLING MODE
	{					
		do
		{
			uiValue = uartc->lsr;
			if(uiValue & MICOUART_LSR_RX_RDY_MASK)
			{
				ret = 1;										// one byte received.
    			usp->err = EE_UART_OK;
    			*data = uartc->rxtx;
        		// All done!!!
    			break;
    		}
			else
			{
    			if(!EE_uart_rx_block(mode))						// if no blocking mode
				{					
	    			usp->err = EE_UART_ERR_RX_NOT_READY;
					ret = EE_UART_ERR_RX_NOT_READY;				// one byte transmitted
					break;
				}
			} 
		}while(1);
	}
	else
	{
		do
		{
			ret = EE_buffer_getmsg(buffer, data);			
			if(ret == EE_BUF_OK)								// byte extracted from the rx buffer
			{								
				ret = 1;
				usp->err = ret;
				break;
			}
			else
			{	
				usp->err = EE_UART_ERR_RX_BUF_EMPTY;
				if(!EE_uart_rx_block(mode))
				{
					ret = EE_UART_ERR_RX_BUF_EMPTY;
					break;
				}
			}
		}while(1);
	}

		
    // All done!!!
	return ret;
}

/* This functions can be used to write a buffer of characters */
/*
	- polling mode: 
		- blocking mode: every time wait until tx is ready, send all characters then return
		- no-blocking mode: no wait if in the first attempt tx is not ready, return EE_UART_ERR_TX_NOT_READY
	- isr mode:
		- blocking mode: wait if a transmission is in progress or if buffer is full, return the number of bytes loaded (should be loaded all characters...)
		- no-blocking mode: no wait if buffer is full or if a transmission is in progress, return the number of bytes loaded
*/
int EE_hal_uart_write_buffer(EE_uart_st* usp, EE_UINT8 *vet, int len)
{
	unsigned int uiValue, mode;
	EE_buffer* buffer;
	int ret,i;
  
	MicoUart_t *uartc = usp->base; 
	
	buffer = &usp->txbuf;
	mode = usp->mode;
	
	if( EE_uart_tx_polling(mode) )							// TX POLLING MODE
	{							
		for(i=0; i<len; i++)
		{
			do
			{
				/* if uart's ready to accept character, send immediately */
    			uiValue = uartc->lsr;
    			if(uiValue & MICOUART_LSR_TX_RDY_MASK)
				{
    				uartc->rxtx = vet[i];					// byte loaded in the uart tx buffer
    				ret = 1;
    				usp->err = EE_UART_OK;
    				break;
				}
				else if(i==0)
				{											// if TX is not ready because another task uses tx buffer...
					usp->err = EE_UART_ERR_TX_NOT_READY;
					if(!EE_uart_tx_block(mode))				// no bytes transmitted, operation failed...
					{			
						ret = EE_UART_ERR_TX_NOT_READY;		
						break;
					}
				}
				// if you don't want a blocking procedure return here...
			}while(1);
			if(ret == EE_UART_ERR_TX_NOT_READY)
				return ret;									// return, no bytes transmitted
		}
		ret = len;											// all bytes transmitted... 
	}
	else
	{	// if there is no transmission in progress or there is a transmission in progress but we are in blocking mode
		if( !ee_uart_tip || (ee_uart_tip && EE_uart_tx_block(mode)) )
		{																																			
			while(ee_uart_tip)
				;								// wait until transmission is finished...
			for(i=0; i<len; i++)
			{
				ret = EE_buffer_putmsg(buffer, vet+i);
				if(ret == EE_BUF_OK)						// byte loaded in the tx buffer... 
					usp->err = ret; 
				else
				{
					usp->err = EE_UART_ERR_TX_BUF_FULL;
					if(!EE_uart_tx_block(mode))
						break;
					else
					{
						i--;											// restore the previous character...
						EE_hal_uart_enable_tx_ISR(usp);					// enable tx interrupt
						while(ee_uart_tip)
							;								// wait until transmission is finished...
					}
				}
			}
			EE_hal_uart_enable_tx_ISR(usp);					// enable tx interrupt
	        ret = i;										// number of bytes loaded in the tx buffer
		}
		else												// there is a transmission in progress and we are in non blocking mode
		{
			usp->err = EE_UART_ERR_TX_NOT_READY;
			ret = EE_UART_ERR_TX_NOT_READY;
		}
	}
	
	// All done!!!
	return ret;
}

/* This functions can be used to read a buffer of characters */
/*
	- polling mode: 
		- blocking mode: wait until rx is ready, return the number of the received bytes
		- no-blocking mode: no wait, return the number of the received bytes
	- isr mode:
		- blocking mode: every time wait until rx buffer is ready (not empty), return the number of the received bytes (should be received all characters...)
		- no-blocking mode: no wait if rx buffer is empty, return the number of the received bytes
*/
int EE_hal_uart_read_buffer(EE_uart_st* usp, EE_UINT8 *vet, int len)
{
	int i;
	int ret = EE_UART_ERR_RX_BUF_EMPTY;

	for(i=0; i<len; i++)
	{
		ret = EE_hal_uart_read_byte(usp, &vet[i]); 
		if( ret < 0)
			break;
	}
	
	if(i==0)
		return ret;
	else
		return i;	// number of bytes read.
}

/* This functions enables IRQ */
int EE_hal_uart_enable_IRQ(EE_uart_st* usp, int ier)
{
	usp->base->ier = (volatile unsigned int)(ier);
	
	return EE_UART_OK;
}

/* This functions disables IRQ */
int EE_hal_uart_disable_IRQ(EE_uart_st* usp)
{
	usp->base->ier = 0;
	
	return EE_UART_OK;
}

int EE_hal_uart_return_error(EE_uart_st* usp)
{
	return usp->err;
}

