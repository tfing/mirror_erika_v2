#include "ee.h"

#include "P2PDefs.h"
#include "console_serial.h"


const EE_UINT8 char_array[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D',
						'E','F'};

/*
 * This function prints on the console the hexadecimal value of to_print
 */
void PrintChar(EE_UINT8 to_print)
{
    console_write(MY_FIRST_CONSOLE, (EE_UINT8*) &char_array[(to_print & 0xF0)>>4], 1);
    console_write(MY_FIRST_CONSOLE, (EE_UINT8*) &char_array[to_print & 0x0F], 1);
}

