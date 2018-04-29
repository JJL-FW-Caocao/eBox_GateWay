#ifndef __DDC_H
#define __DDC_H


#ifdef __cplusplus
 extern "C" {
#endif
     
     
#include "stdio.h"
#include "mcu_core.h"
#include "ddc_port.h"
#include "listc.h"
     
  
typedef enum 
{
    UNLOCK,
    LOCK
}DdcLock_t;

void ddc_loop();

void ddc_recv_process();
void analyze_payload();
void make_frame(unsigned char *data,unsigned int data_len,unsigned char ack,unsigned char type );
void frame_retry(DdcNode_t *p);


unsigned int get_len_frome_frame(unsigned char *frame);
unsigned int get_id_frome_frame(unsigned char *frame);
unsigned int get_ack_frome_frame(unsigned char *frame);
unsigned int get_type_frome_frame(unsigned char *frame);


#ifdef __cplusplus
}
#endif

#endif
 