/*
  Name: test7.h
  Copyright: Evidence Srl
  Author: Dario Di Stefano
  Date: 29/03/10 18.23
  Description: LWIP udp test.
*/

#ifndef __TEST7_H__
#define __TEST7_H__

/* RT-Kernel */
#include <ee.h>

/* Mecros used in the application code */
#define LWIP_ON
#define PRINT_ON

#define turn_on_led() 				EE_misc_gpio_write_bit_data(1,EE_DL3_BIT)
#define turn_off_led() 				EE_misc_gpio_write_bit_data(0,EE_DL3_BIT)
/* Size of the expected payload */
#define UDP_PAYLOAD_PKT_SIZE 		4
/* Size of the buffer (vector of packets) */
#define UDP_BUFFER_SIZE 			10

/* Functions used in the application code */
void system_timer_callback(void);
int UDP_receive(BYTE* rxv);
int UDP_send(BYTE* txv);
void print_pbuf(const char *name, struct pbuf *p);
void print_array(const char *name, BYTE* vet, int len);
void myprintf(const char* format, ...);

#endif //__TEST7_H__