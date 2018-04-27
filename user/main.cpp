/*----------------------------------------------------------------------------
 * Name:    demo.c
 * Purpose: USB HID Demo
 * Version: V1.20
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/
#include "mcu_config.h"    

//#include "stm32f10x_lib.h"                           // stm32f10x definitions

#include "type.h"

#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"

//#include "STM32_Init.h"                              // stm32 initialisation
#include "usbcore.h"
#include "cdcuser.h"

#include "usart.h"
#include "stm32_init.h"
/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/

#include "ebox.h"
#include "usbreg.h"
Timer timer(TIM2);
uint8_t i = 0;
void test()
{
    i++;
    if(i == 5)
    {
        PB9.toggle();
        if(terminal_connected)
            CDC2_InBufChar('9');
        i = 0;
    }
}
int main (void) 
{
	unsigned int d;
    ebox_init();
	stm32_Init();                                     /* STM32 Initialization */
	USB_Init();                                        /* USB Initialization */
	CDC_Init();
	USB_Connect(__TRUE);
    PB8.mode(OUTPUT_PP);
    PB9.mode(OUTPUT_PP);
    
	timer.begin(100);
    timer.attach(test);
    timer.interrupt(ENABLE);
    timer.start();
	while (1)                                         /* Loop forever */
	{
		d = Uart_Task();
		if(d < 0xffff)
        {
			PB8.set();
        }
		else
			PB8.reset();
        
//        CDC2_InBufChar('9');
//        PB8.toggle();
	}
}
