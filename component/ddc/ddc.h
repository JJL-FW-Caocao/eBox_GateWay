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
void make_frame(uint8_t *data,uint16_t data_len,uint8_t ack,uint8_t type );
void frame_retry(DdcNode_t *p);
uint16_t make_frame1(uint8_t *dsc,uint8_t *data,uint16_t data_len,uint8_t ack,uint8_t type );
void add_to_list(uint8_t *buf);
uint16_t make_ack_frame(uint8_t *dsc,uint16_t id);

uint16_t get_len_frome_frame(uint8_t *frame);
uint16_t get_id_frome_frame(uint8_t *frame);
uint8_t get_ack_frome_frame(uint8_t *frame);
uint8_t get_type_frome_frame(uint8_t *frame);
void print_frame(uint8_t *frame);
void print_list(DdcNode_t *list);


void frame_send(DdcNode_t *p);
void check_ack_list();
#ifdef __cplusplus
}
#endif

#endif
 