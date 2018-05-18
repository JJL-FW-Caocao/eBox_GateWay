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

#include "ddc.h"

Timer timer(TIM2);
extern callback_fun_type EP_evnent[3];

char buf[100];
uint8_t bufr1[100];
uint8_t bufr2[100];
uint8_t bufr3[100];

uint8_t buft1[100];
uint8_t buft2[100];
uint8_t buft3[100];

uint8_t data[5] = {0x01,0X01,0x05,0X06,0X07};
uint16_t len;


void timer_event()
{
    static unsigned char i;
    i++;
    data[1] = i;
//    PB9.toggle();
////    
//    if(terminal_connected)
//    {   
//        CDC1_InBufWrite((char*) "123\r\n",5);
//    }
    
    len = ddc_make_frame(buft2,data,5,1,0);
    for(int i = 0; i < len; i++)
        ddc_get_char(buft2[i]);

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
extern DdcNode_t list_send;



void test(uint8_t *ptr,uint16_t len)
{
    PB8.toggle();
    uart1.printf("【value = %d】\r\n",ptr[1]);
}
int main (void) 
{
    ebox_init();
//	stm32_Init();                                     /* STM32 Initialization */
//	USB_Init();                                        /* USB Initialization */
//	CDC_Init();
//	USB_Connect(__TRUE);
    PB8.mode(OUTPUT_PP);
    PB9.mode(OUTPUT_PP);
//    
	timer.begin(1);
    timer.attach(timer_event);
    timer.interrupt(ENABLE);
    timer.start();
    
    uart1.begin(115200);
//    uart1.attach(uart1_rx_event,RxIrq);
//    uart1.interrupt(RxIrq,ENABLE);
  
//    uart2.begin(115200);
//    uart2.attach(uart2_rx_event,RxIrq);
//    uart2.interrupt(RxIrq,ENABLE);
//    
//    uart3.begin(115200);
//    uart3.attach(uart3_rx_event,RxIrq);
//    uart3.interrupt(RxIrq,ENABLE);

     ddc_init();
     ddc_attach_type0(test);
//    ddc_get_char(0x55);
//    ddc_get_char(0xaa);//头
//    ddc_get_char(0x07);
//    ddc_get_char(0x00);//len
//    ddc_get_char(0x0);
//    ddc_get_char(0x0);//id
//    ddc_get_char(0x00);//质量
//    ddc_get_char(0xff);//类型
//    ddc_get_char(0x03);//用户数据
//    
// 
//    ddc_get_char(0x6c);//crc
//    ddc_get_char(0x31);
//    

//    make_frame(data,10,1,1);

//    DdcNode_t* node;
//    node = list;
//    while(node != NULL)
//    {
//        uart1.printf("id = %d\r\n",node->id);
//        node = node->next;
//    }
    
    
//    uart1.print(sizeof(DdcNode_t));
    
//    i=0;
//    DdcFrame_t frame1;
//    frame1.type = 1;
//    frame1.ack  = 1;
//    frame1.payload_len = 5;
    
//    len = make_frame1(buf1,data,5,1,1);
//    for(int i = 0; i < len; i++)
//        ddc_get_char(buf1[i]);
//    
//    len = make_frame1(buf1,data,5,1,1);
//    for(int i = 0; i < len; i++)
//        ddc_get_char(buf1[i]);

//    
//    len = make_ack_frame1(buf1,12);
//    for(int i = 0; i < len; i++)
//        ddc_get_char(buf1[i]);
//    while(1)
//    {
//    ddc_recv_process();
//    }

    while (1)                                         /* Loop forever */
	{
        
        //make_frame(data,5,1,1);
        uart1.printf("----------创建消息----------------------\r\n");
        len = ddc_make_frame(buft1,data,100,1,1);
        ddc_add_to_list(buft1);
//        len = make_frame(buft2,data,50,1,1);
//        add_to_list(buft2);
//        len = make_frame(buft3,data,5,0,1);
//        add_to_list(buft3);
        uart1.printf("----------------------------------------\r\n");
        
        
        uart1.printf("----------发送消息----------------------\r\n");
        ddc_send_list(&list_send);
        uart1.printf("----------------------------------------\r\n");
        
       
        
        uart1.printf("----------模拟接收----------------------\r\n");
        len = ddc_make_ack_frame(bufr1,get_frame_id(buft1));
        
        no_interrupts();
        for(int i = 0; i < len; i++)
            ddc_get_char(bufr1[i]);
        interrupts();
//        len = make_ack_frame(bufr2,get_id_frome_frame(buft2));
//        for(int i = 0; i < len; i++)
//            ddc_get_char(bufr2[i]);
        for(int i = 0; i < 50; i++)
            ddc_recv_process();
        uart1.printf("----------------------------------------\r\n");
        uart1.printf("----------检查需要回复的列表------------\r\n");
        ddc_check_ack_list();
         uart1.printf("---------------------------------------\r\n");
        
//        ddc_loop();
       // ddc_recv_process();
//        make_frame(data,10,0,1);
//    make_frame(data,5,1,1);
//    make_frame(data,5,1,1);
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
//    uart1.println(i++);
//    delay_ms(1);
//        CDC2_InBufChar('9');
//        PB8.toggle();
	}
}
