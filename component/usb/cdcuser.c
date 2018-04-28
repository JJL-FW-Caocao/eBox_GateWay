/*----------------------------------------------------------------------------
 * U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    cdcuser.c
 * Purpose: USB Communication Device Class User module
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
 *----------------------------------------------------------------------------
 * History:
 *          V1.20
 *          V1.00 Initial Version
 *----------------------------------------------------------------------------*/
#include "mcu_config.h"    

//#include "stm32f10x_lib.h"                        // STM32F10x Library Definitions
//#include "STM32_Reg.h"                            // missing bit definitions

#include "type.h"

#include "usb.h"
#include "usbhw.h"
#include "usbcfg.h"
#include "usbcore.h"
#include "cdc.h"
#include "cdcuser.h"

BOOL terminal_connected = __FALSE;
CDC_LINE_CODING CDC1_LineCoding  = {9600, 0, 0, 8};
CDC_LINE_CODING CDC2_LineCoding  = {9600, 0, 0, 8};
CDC_LINE_CODING CDC3_LineCoding  = {9600, 0, 0, 8};
/*----------------------------------------------------------------------------
  CDC SendEncapsulatedCommand Request Callback
  Called automatically on CDC SEND_ENCAPSULATED_COMMAND Request
  Parameters:   None                          (global SetupPacket and EP0Buf)
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
BOOL CDC_SendEncapsulatedCommand (void) {

  return (__TRUE);
}


/*----------------------------------------------------------------------------
  CDC GetEncapsulatedResponse Request Callback
  Called automatically on CDC Get_ENCAPSULATED_RESPONSE Request
  Parameters:   None                          (global SetupPacket and EP0Buf)
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
BOOL CDC_GetEncapsulatedResponse (void) {

  /* ... add code to handle request */
  return (__TRUE);
}


/*----------------------------------------------------------------------------
  CDC SetCommFeature Request Callback
  Called automatically on CDC Set_COMM_FATURE Request
  Parameters:   FeatureSelector
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
BOOL CDC_SetCommFeature (unsigned short wFeatureSelector) {

  /* ... add code to handle request */
  return (__TRUE);
}


/*----------------------------------------------------------------------------
  CDC GetCommFeature Request Callback
  Called automatically on CDC Get_COMM_FATURE Request
  Parameters:   FeatureSelector
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
BOOL CDC_GetCommFeature (unsigned short wFeatureSelector) {

  /* ... add code to handle request */
  return (__TRUE);
}


/*----------------------------------------------------------------------------
  CDC ClearCommFeature Request Callback
  Called automatically on CDC CLEAR_COMM_FATURE Request
  Parameters:   FeatureSelector
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
BOOL CDC_ClearCommFeature (unsigned short wFeatureSelector) {

  /* ... add code to handle request */
  return (__TRUE);
}


/*----------------------------------------------------------------------------
  CDC SetLineCoding Request Callback
  Called automatically on CDC SET_LINE_CODING Request
  Parameters:   none                    (global SetupPacket and EP0Buf)
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
BOOL CDC_SetLineCoding (void) {
	char i = (SetupPacket.wIndex.WB.L - USB_CDC_CIF_NUM) >> 1;

	switch(i)
	{
	case 0:
		CDC1_LineCoding.dwDTERate   =  (EP0Buf[0] <<  0)
									 | (EP0Buf[1] <<  8)
									 | (EP0Buf[2] << 16)
									 | (EP0Buf[3] << 24);
		CDC1_LineCoding.bCharFormat =  EP0Buf[4];
		CDC1_LineCoding.bParityType =  EP0Buf[5];
		CDC1_LineCoding.bDataBits   =  EP0Buf[6];
	
		USART1->CR1 &= ~(1 << 13);	//dis uart
		__nop();__nop();__nop();__nop();__nop();
		USART1->CR2 &= ~(3 << 12);
		switch(CDC1_LineCoding.bCharFormat)
		{
		case 1:	//1.5
			USART1->CR2 |= (3 << 12);
			break;
		case 2:	//2
			USART1->CR2 |= (2 << 12);
			break;
		case 0:	//1
		default:
			USART1->CR2 |= (0 << 12);
			break;
		}
		USART1->CR1 &= ~((1 << 9) | (1 << 10) | (1 << 12));
		switch(CDC1_LineCoding.bParityType)
		{
		case 1:	//odd
			USART1->CR1 |= (1 << 9);
			USART1->CR1 |= (1 << 10);
			USART1->CR1 |= (1 << 12);
			break;
		case 2:	//even
			USART1->CR1 |= (0 << 9);
			USART1->CR1 |= (1 << 10);
			USART1->CR1 |= (1 << 12);
			break;
		case 3:	//mark
		case 4:	//space
			USART1->CR1 |= (0 << 9);
			USART1->CR1 |= (0 << 10);
			USART1->CR1 |= (1 << 12);
			break;
		case 0:	//none
		default:
			USART1->CR1 |= (0 << 9);
			USART1->CR1 |= (0 << 10);
			USART1->CR1 |= (0 << 12);
			break;
		}
		USART1->BRR  = 72000000 / CDC1_LineCoding.dwDTERate; // set baudrate
		USART1->CR1 |= (1 << 13);	//en uart
		break;
	case 1:
		CDC2_LineCoding.dwDTERate   =  (EP0Buf[0] <<  0)
									 | (EP0Buf[1] <<  8)
									 | (EP0Buf[2] << 16)
									 | (EP0Buf[3] << 24);
		CDC2_LineCoding.bCharFormat =  EP0Buf[4];
		CDC2_LineCoding.bParityType =  EP0Buf[5];
		CDC2_LineCoding.bDataBits   =  EP0Buf[6];
	
		USART2->CR1 &= ~(1 << 13);	//dis uart
		__nop();__nop();__nop();__nop();__nop();
		USART2->CR2 &= ~(3 << 12);
		switch(CDC2_LineCoding.bCharFormat)
		{
		case 1:	//1.5
			USART2->CR2 |= (3 << 12);
			break;
		case 2:	//2
			USART2->CR2 |= (2 << 12);
			break;
		case 0:	//1
		default:
			USART2->CR2 |= (0 << 12);
			break;
		}
		USART2->CR1 &= ~((1 << 9) | (1 << 10) | (1 << 12));
		switch(CDC2_LineCoding.bParityType)
		{
		case 1:	//odd
			USART2->CR1 |= (1 << 9);
			USART2->CR1 |= (1 << 10);
			USART2->CR1 |= (1 << 12);
			break;
		case 2:	//even
			USART2->CR1 |= (0 << 9);
			USART2->CR1 |= (1 << 10);
			USART2->CR1 |= (1 << 12);
			break;
		case 3:	//mark
		case 4:	//space
			USART2->CR1 |= (0 << 9);
			USART2->CR1 |= (0 << 10);
			USART2->CR1 |= (1 << 12);
			break;
		case 0:	//none
		default:
			USART2->CR1 |= (0 << 9);
			USART2->CR1 |= (0 << 10);
			USART2->CR1 |= (0 << 12);
			break;
		}
		USART2->BRR  = 36000000 / CDC2_LineCoding.dwDTERate; // set baudrate
		USART2->CR1 |= (1 << 13);	//en uart
		break;
	default:
		CDC3_LineCoding.dwDTERate   =  (EP0Buf[0] <<  0)
									 | (EP0Buf[1] <<  8)
									 | (EP0Buf[2] << 16)
									 | (EP0Buf[3] << 24);
		CDC3_LineCoding.bCharFormat =  EP0Buf[4];
		CDC3_LineCoding.bParityType =  EP0Buf[5];
		CDC3_LineCoding.bDataBits   =  EP0Buf[6];
	
		USART3->CR1 &= ~(1 << 13);	//dis uart
		__nop();__nop();__nop();__nop();__nop();
		USART3->CR2 &= ~(3 << 12);
		switch(CDC3_LineCoding.bCharFormat)
		{
		case 1:	//1.5
			USART3->CR2 |= (3 << 12);
			break;
		case 2:	//2
			USART3->CR2 |= (2 << 12);
			break;
		case 0:	//1
		default:
			USART3->CR2 |= (0 << 12);
			break;
		}
		USART3->CR1 &= ~((1 << 9) | (1 << 10) | (1 << 12));
		switch(CDC3_LineCoding.bParityType)
		{
		case 1:	//odd
			USART3->CR1 |= (1 << 9);
			USART3->CR1 |= (1 << 10);
			USART3->CR1 |= (1 << 12);
			break;
		case 2:	//even
			USART3->CR1 |= (0 << 9);
			USART3->CR1 |= (1 << 10);
			USART3->CR1 |= (1 << 12);
			break;
		case 3:	//mark
		case 4:	//space
			USART3->CR1 |= (0 << 9);
			USART3->CR1 |= (0 << 10);
			USART3->CR1 |= (1 << 12);
			break;
		case 0:	//none
		default:
			USART3->CR1 |= (0 << 9);
			USART3->CR1 |= (0 << 10);
			USART3->CR1 |= (0 << 12);
			break;
		}
		USART3->BRR  = 36000000 / CDC3_LineCoding.dwDTERate; // set baudrate
		USART3->CR1 |= (1 << 13);	//en uart
		break;
	}

	return (__TRUE);
}


/*----------------------------------------------------------------------------
  CDC GetLineCoding Request Callback
  Called automatically on CDC GET_LINE_CODING Request
  Parameters:   None                         (global SetupPacket and EP0Buf)
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
BOOL CDC_GetLineCoding (void) {
	char i = (SetupPacket.wIndex.WB.L - USB_CDC_CIF_NUM) >> 1;

	switch(i)
	{
	case 0:
		EP0Buf[0] = (CDC1_LineCoding.dwDTERate >>  0) & 0xFF;
		EP0Buf[1] = (CDC1_LineCoding.dwDTERate >>  8) & 0xFF;
		EP0Buf[2] = (CDC1_LineCoding.dwDTERate >> 16) & 0xFF;
		EP0Buf[3] = (CDC1_LineCoding.dwDTERate >> 24) & 0xFF;
		EP0Buf[4] =  CDC1_LineCoding.bCharFormat;
		EP0Buf[5] =  CDC1_LineCoding.bParityType;
		EP0Buf[6] =  CDC1_LineCoding.bDataBits;
		break;
	case 1:
		EP0Buf[0] = (CDC2_LineCoding.dwDTERate >>  0) & 0xFF;
		EP0Buf[1] = (CDC2_LineCoding.dwDTERate >>  8) & 0xFF;
		EP0Buf[2] = (CDC2_LineCoding.dwDTERate >> 16) & 0xFF;
		EP0Buf[3] = (CDC2_LineCoding.dwDTERate >> 24) & 0xFF;
		EP0Buf[4] =  CDC2_LineCoding.bCharFormat;
		EP0Buf[5] =  CDC2_LineCoding.bParityType;
		EP0Buf[6] =  CDC2_LineCoding.bDataBits;
		break;
	default:
		EP0Buf[0] = (CDC3_LineCoding.dwDTERate >>  0) & 0xFF;
		EP0Buf[1] = (CDC3_LineCoding.dwDTERate >>  8) & 0xFF;
		EP0Buf[2] = (CDC3_LineCoding.dwDTERate >> 16) & 0xFF;
		EP0Buf[3] = (CDC3_LineCoding.dwDTERate >> 24) & 0xFF;
		EP0Buf[4] =  CDC3_LineCoding.bCharFormat;
		EP0Buf[5] =  CDC3_LineCoding.bParityType;
		EP0Buf[6] =  CDC3_LineCoding.bDataBits;
		break;
	}

  return (__TRUE);
}


/*----------------------------------------------------------------------------
  CDC SetControlLineState Request Callback
  Called automatically on CDC SET_CONTROL_LINE_STATE Request
  Parameters:   ControlSignalBitmap
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
BOOL CDC_SetControlLineState (unsigned short wControlSignalBitmap) {

    terminal_connected = __TRUE;
  /* ... add code to handle request */
  return (__TRUE);
}


/*----------------------------------------------------------------------------
  CDC SendBreak Request Callback
  Called automatically on CDC Set_COMM_FATURE Request
  Parameters:   0xFFFF  start of Break
				0x0000  stop  of Break
				0x####  Duration of Break
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
BOOL CDC_SendBreak (unsigned short wDurationOfBreak) {

  /* ... add code to handle request */
  return (__TRUE);
}


#define CDC_BUF_SIZE               (1024)
#define CDC_BUF_MASK               (CDC_BUF_SIZE-1ul)

typedef struct __CDC_BUF_T {
	unsigned int wrIdx;
	unsigned int rdIdx;
	char data[CDC_BUF_SIZE];
} CDC_BUF_T;

#define CDC_BUF_RESET(cdcBuf)      (cdcBuf.rdIdx = cdcBuf.wrIdx = 0)
#define CDC_BUF_WR(cdcBuf, dataIn) (cdcBuf.data[CDC_BUF_MASK & cdcBuf.wrIdx++] = (dataIn))
#define CDC_BUF_RD(cdcBuf)         (cdcBuf.data[CDC_BUF_MASK & cdcBuf.rdIdx++])
#define CDC_BUF_EMPTY(cdcBuf)      (cdcBuf.rdIdx == cdcBuf.wrIdx)
#define CDC_BUF_FULL(cdcBuf)       ((CDC_BUF_MASK & cdcBuf.rdIdx) == (CDC_BUF_MASK & (cdcBuf.wrIdx+1)))
#define CDC_BUF_COUNT(cdcBuf)      (CDC_BUF_MASK & (cdcBuf.wrIdx - cdcBuf.rdIdx))

static CDC_BUF_T CDC1_InBuf;
static volatile unsigned char CDC1_DepInEmpty = 1;
void CDC1_BulkIn(void)
{
	static unsigned char nBuf[USB_CDC_BUFSIZE];
	int leng,i;
	unsigned char *pt = nBuf;

	leng = CDC_BUF_COUNT(CDC1_InBuf);
	if (leng > 0)
	{
		if(leng > USB_CDC_BUFSIZE)
			leng = USB_CDC_BUFSIZE;
		for(i = 0; i < leng; i++)
			*pt++ = CDC_BUF_RD(CDC1_InBuf);
		USB_WriteEP (0x82, nBuf, leng);
	}
	else
		CDC1_DepInEmpty = 1;
}

void CDC1_InBufChar(char dat)
{
	if(!CDC_BUF_FULL(CDC1_InBuf))
		CDC_BUF_WR(CDC1_InBuf,dat);
	if(CDC1_DepInEmpty)
	{
		CDC1_DepInEmpty = 0;
		CDC1_BulkIn();
	}
}
int CDC1_InBufWrite(char* dat,int len)
{
    int i;
    char *p = dat;
    if(len > 64)
        return 0;
    for(i = 0; i < len; i++)
    {
        if(!CDC_BUF_FULL(CDC1_InBuf))
            CDC_BUF_WR(CDC1_InBuf,*p++);
    }
	if(CDC1_DepInEmpty)
	{
		CDC1_DepInEmpty = 0;
		CDC1_BulkIn();
	}
    return len;
}
static CDC_BUF_T CDC1_OutBuf;
static volatile unsigned char CDC1_DepOutPending = 0;
void CDC1_BulkOut(void)
{
	static unsigned char nBuf[USB_CDC_BUFSIZE];
	int leng;
	unsigned char *pt = nBuf;

	if ((CDC_BUF_SIZE - CDC_BUF_COUNT(CDC1_OutBuf)) <= USB_CDC_BUFSIZE)
		CDC1_DepOutPending = 1;
	else
	{
		leng = USB_ReadEP(0x02, nBuf);
		while (leng--)
			CDC_BUF_WR(CDC1_OutBuf, *pt++);
	}
}

int CDC1_OutBufLength(void)
{
	return (CDC_BUF_COUNT(CDC1_OutBuf));
}

int CDC1_OutBufRead (char *buffer, int length)
{
	int i;
	i = CDC_BUF_COUNT(CDC1_OutBuf);

	if(length > i)
		length = i;
	for(i = 0; i < length; i++)
		*buffer++ = CDC_BUF_RD(CDC1_OutBuf);

	return(length);
}

char CDC1_OutBufChar(void)
{
	while(CDC_BUF_EMPTY(CDC1_OutBuf));
	return(CDC_BUF_RD(CDC1_OutBuf));
}

static CDC_BUF_T CDC2_InBuf;
static volatile unsigned char CDC2_DepInEmpty = 1;
void CDC2_BulkIn(void)
{
	static unsigned char nBuf[USB_CDC_BUFSIZE];
	int leng,i;
	unsigned char *pt = nBuf;

	leng = CDC_BUF_COUNT(CDC2_InBuf);
	if (leng > 0)
	{
		if(leng > USB_CDC_BUFSIZE)
			leng = USB_CDC_BUFSIZE;
		for(i = 0; i < leng; i++)
			*pt++ = CDC_BUF_RD(CDC2_InBuf);
		USB_WriteEP (0x84, nBuf, leng);
	}
	else
		CDC2_DepInEmpty = 1;
}

void CDC2_InBufChar(char dat)
{
	if(!CDC_BUF_FULL(CDC2_InBuf))
		CDC_BUF_WR(CDC2_InBuf,dat);
	if(CDC2_DepInEmpty)
	{
		CDC2_DepInEmpty = 0;
		CDC2_BulkIn();
	}
}
int CDC2_InBufWrite(char* dat,int len)
{
    int i;
    char *p = dat;
    if(len > 64)
        return 0;
    for(i = 0; i < len; i++)
    {
        if(!CDC_BUF_FULL(CDC2_InBuf))
            CDC_BUF_WR(CDC2_InBuf,*p++);
    }
	if(CDC2_DepInEmpty)
	{
		CDC2_DepInEmpty = 0;
		CDC2_BulkIn();
	}
    return len;
}
static CDC_BUF_T CDC2_OutBuf;
static volatile unsigned char CDC2_DepOutPending = 0;
void CDC2_BulkOut(void)
{
	static unsigned char nBuf[USB_CDC_BUFSIZE];
	int leng;
	unsigned char *pt = nBuf;

	if ((CDC_BUF_SIZE - CDC_BUF_COUNT(CDC2_OutBuf)) <= USB_CDC_BUFSIZE)
		CDC2_DepOutPending = 1;
	else
	{
		leng = USB_ReadEP(0x04, nBuf);
		while (leng--)
			CDC_BUF_WR(CDC2_OutBuf, *pt++);
	}
}

int CDC2_OutBufLength(void)
{
	return (CDC_BUF_COUNT(CDC2_OutBuf));
}

int CDC2_OutBufRead (char *buffer, int length)
{
	int i;
	i = CDC_BUF_COUNT(CDC2_OutBuf);

	if(length > i)
		length = i;
	for(i = 0; i < length; i++)
		*buffer++ = CDC_BUF_RD(CDC2_OutBuf);

	return(length);
}

char CDC2_OutBufChar(void)
{
	while(CDC_BUF_EMPTY(CDC2_OutBuf));
	return(CDC_BUF_RD(CDC2_OutBuf));
}

static CDC_BUF_T CDC3_InBuf;
static volatile unsigned char CDC3_DepInEmpty = 1;
void CDC3_BulkIn(void)
{
	static unsigned char nBuf[USB_CDC_BUFSIZE];
	int leng,i;
	unsigned char *pt = nBuf;

	leng = CDC_BUF_COUNT(CDC3_InBuf);
	if (leng > 0)
	{
		if(leng > USB_CDC_BUFSIZE)
			leng = USB_CDC_BUFSIZE;
		for(i = 0; i < leng; i++)
			*pt++ = CDC_BUF_RD(CDC3_InBuf);
		USB_WriteEP (0x86, nBuf, leng);
	}
	else
		CDC3_DepInEmpty = 1;
}

void CDC3_InBufChar(char dat)
{
	if(!CDC_BUF_FULL(CDC3_InBuf))
		CDC_BUF_WR(CDC3_InBuf,dat);
	if(CDC3_DepInEmpty)
	{
		CDC3_DepInEmpty = 0;
		CDC3_BulkIn();
	}
}

static CDC_BUF_T CDC3_OutBuf;
static volatile unsigned char CDC3_DepOutPending = 0;
void CDC3_BulkOut(void)
{
	static unsigned char nBuf[USB_CDC_BUFSIZE];
	int leng;
	unsigned char *pt = nBuf;
	
	if ((CDC_BUF_SIZE - CDC_BUF_COUNT(CDC3_OutBuf)) <= USB_CDC_BUFSIZE)
		CDC3_DepOutPending = 1;
	else
	{
		leng = USB_ReadEP(0x06, nBuf);
		while (leng--)
			CDC_BUF_WR(CDC3_OutBuf, *pt++);
	}
}

int CDC3_OutBufLength(void)
{
	return (CDC_BUF_COUNT(CDC3_OutBuf));
}

int CDC3_OutBufRead (char *buffer, int length)
{
	int i;
	i = CDC_BUF_COUNT(CDC3_OutBuf);

	if(length > i)
		length = i;
	for(i = 0; i < length; i++)
		*buffer++ = CDC_BUF_RD(CDC3_OutBuf);

	return(length);
}

char CDC3_OutBufChar(void)
{
	while(CDC_BUF_EMPTY(CDC3_OutBuf));
	return(CDC_BUF_RD(CDC3_OutBuf));
}

/*----------------------------------------------------------------------------
  CDC Initialisation
  Initializes the data structures and serial port
  Parameters:   None
  Return Value: None
 *---------------------------------------------------------------------------*/
void CDC_Init (void) {
	CDC1_DepInEmpty  = 1;
	CDC2_DepInEmpty  = 1;
	CDC3_DepInEmpty  = 1;
	CDC_BUF_RESET(CDC1_OutBuf);
	CDC_BUF_RESET(CDC2_OutBuf);
	CDC_BUF_RESET(CDC3_OutBuf);
	CDC_BUF_RESET(CDC1_InBuf);
	CDC_BUF_RESET(CDC2_InBuf);
	CDC_BUF_RESET(CDC3_InBuf);
}

void CDC_StartOfFrame(void)
{
	if (CDC1_DepOutPending)
	{
		if (CDC_BUF_EMPTY(CDC1_OutBuf))
		{
			CDC1_DepOutPending = 0;
			CDC1_BulkOut ();                              /* read pending data */
		}
	}

	if (CDC2_DepOutPending)
	{
		if (CDC_BUF_EMPTY(CDC2_OutBuf))
		{
			CDC2_DepOutPending = 0;
			CDC2_BulkOut ();                              /* read pending data */
		}
	}

	if (CDC3_DepOutPending)
	{
		if (CDC_BUF_EMPTY(CDC3_OutBuf))
		{
			CDC3_DepOutPending = 0;
			CDC3_BulkOut ();                              /* read pending data */
		}
	}
}
