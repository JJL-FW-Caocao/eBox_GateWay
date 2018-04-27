#include "mcu_config.h"    
//#include "stm32f10x_lib.h"                           /* stm32f10x definitions */
#include "type.h"
#include "usb.h"
#include "cdc.h"
#include "cdcuser.h"
#include "STM32_Init.h"
#include "usart.h"

static volatile unsigned int flag;
void USART1_IRQHandler (void)
{
	unsigned int rg;

	rg = USART1->SR;
	if (rg & USART_FLAG_RXNE)
	{
		flag = 0;
		CDC1_InBufChar(USART1->DR);
	}
}

void USART2_IRQHandler (void)
{
	unsigned int rg;

	rg = USART2->SR;
	if (rg & USART_FLAG_RXNE)
	{
		flag = 0;
		CDC2_InBufChar(USART2->DR);
	}
}

void USART3_IRQHandler (void)
{
	unsigned int rg;

	rg = USART3->SR;
	if (rg & USART_FLAG_RXNE)
	{
		flag = 0;
		CDC3_InBufChar(USART3->DR);
	}
}

unsigned int Uart_Task(void)
{
	flag++;
	if(USART1->SR & USART_FLAG_TXE)
	{
		if(CDC1_OutBufLength())
		{
			flag = 0;
			USART1->DR = CDC1_OutBufChar();
		}
	}

	if(USART2->SR & USART_FLAG_TXE)
	{
		if(CDC2_OutBufLength())
		{
			flag = 0;
			USART2->DR = CDC2_OutBufChar();
		}
	}
	
	if(USART3->SR & USART_FLAG_TXE)
	{
		if(CDC3_OutBufLength())
		{
			flag = 0;
			USART3->DR = CDC3_OutBufChar();
		}
	}
	return flag;
}
