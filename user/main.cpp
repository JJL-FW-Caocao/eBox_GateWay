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
#include <stdio.h>

#include "ebox.h"
#include "usbreg.h"
Timer timer(TIM2);
extern callback_fun_type EP_evnent[3];

//void uart1_rx_event()
//{
//    PB9.toggle();
////    
//    if(terminal_connected)
//    {        
//        CDC1_InBufChar(uart1.read());
//    }
//}

void timer_event()
{
    PB9.toggle();
//    
    if(terminal_connected)
    {   
        CDC1_InBufWrite((char*) "123\r\n",5);
    }
}


void uart2_rx_event()
{
    PB9.toggle();
//    
    if(terminal_connected)
    {        
        CDC2_InBufChar(uart2.read());
    }
}


void uart3_rx_event()
{
    PB9.toggle();
    if(terminal_connected)
    {
        
        CDC3_InBufChar(uart3.read());
    }


}


char buf[100];
int main (void) 
{
	unsigned int d;
    int i;
    int size;
    ebox_init();
	stm32_Init();                                     /* STM32 Initialization */
	USB_Init();                                        /* USB Initialization */
	CDC_Init();
	USB_Connect(__TRUE);
    PB8.mode(OUTPUT_PP);
    PB9.mode(OUTPUT_PP);
    
	timer.begin(1);
    timer.attach(timer_event);
    timer.interrupt(ENABLE);
    timer.start();
    
//    uart1.begin(115200);
//    uart1.attach(uart1_rx_event,RxIrq);
//    uart1.interrupt(RxIrq,ENABLE);
  
    uart2.begin(115200);
    uart2.attach(uart2_rx_event,RxIrq);
    uart2.interrupt(RxIrq,ENABLE);
    
    uart3.begin(115200);
    uart3.attach(uart3_rx_event,RxIrq);
    uart3.interrupt(RxIrq,ENABLE);
    
    
    
    while (1)                                         /* Loop forever */
        

	{
//		d = Uart_Task();
//		if(d < 0xffff)
//        {
//			PB8.set();
//        }
//		else
//			PB8.reset();
            
//    if(terminal_connected)
//    {
//        
//        size = sprintf(buf,"%d\r\n", i);

//        CDC2_InBufWrite((char *)buf,size);
//    }
    i++;
    //delay_ms(1);
//        CDC2_InBufChar('9');
//        PB8.toggle();
	}
}
