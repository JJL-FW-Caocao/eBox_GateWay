/*----------------------------------------------------------------------------
 * U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    cdcuser.h
 * Purpose: USB Communication Device Class User module Definitions
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

#ifndef __CDCUSER_H__
#define __CDCUSER_H__

#ifdef __cplusplus
 extern "C" {
#endif
     
extern BOOL terminal_connected;     
/* CDC Requests Callback Functions */
extern BOOL CDC_SendEncapsulatedCommand  (void);
extern BOOL CDC_GetEncapsulatedResponse  (void);
extern BOOL CDC_SetCommFeature           (unsigned short wFeatureSelector);
extern BOOL CDC_GetCommFeature           (unsigned short wFeatureSelector);
extern BOOL CDC_ClearCommFeature         (unsigned short wFeatureSelector);
extern BOOL CDC_GetLineCoding            (void);
extern BOOL CDC_SetLineCoding            (void);
extern BOOL CDC_SetControlLineState      (unsigned short wControlSignalBitmap);
extern BOOL CDC_SendBreak                (unsigned short wDurationOfBreak);

/* CDC Bulk Callback Functions */
extern void CDC_Init (void);
extern void CDC_StartOfFrame             (void);
extern void CDC1_BulkIn                  (void);
extern void CDC1_BulkOut                 (void);
extern void CDC2_BulkIn                  (void);
extern void CDC2_BulkOut                 (void);
extern void CDC3_BulkIn                  (void);
extern void CDC3_BulkOut                 (void);

/* FIFO Functions */
void CDC1_InBufChar(char dat);
int CDC1_InBufWrite(char* dat,int len);
int  CDC1_OutBufLength(void);
int  CDC1_OutBufRead (char *buffer, int length);
char CDC1_OutBufChar(void);

void CDC2_InBufChar(char dat);
int  CDC2_InBufWrite(char* dat,int len);
int  CDC2_OutBufLength(void);
int  CDC2_OutBufRead (char *buffer, int length);
char CDC2_OutBufChar(void);

void CDC3_InBufChar(char dat);
int  CDC3_OutBufLength(void);
int  CDC3_OutBufRead (char *buffer, int length);
char CDC3_OutBufChar(void);


#ifdef __cplusplus
}
#endif

#endif  /* __CDCUSER_H__ */

