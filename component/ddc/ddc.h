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

typedef struct
{
    uint8_t     head[2];
    DataU16_t   id;
    uint8_t     type;
    uint8_t     ack;
    DataU16_t   payload_len;
    uint8_t     payload[DDC_MAX_PAYLOAD_LENGTH];
    DataU16_t   crc;
}DdcFrame_t;

#define PAY2FRAME_LEN(x) (x + 10)




uint16_t ddc_frame_to_buf(uint8_t *dst,DdcFrame_t *frame);
uint16_t make_frame(uint8_t *dsc,uint8_t *data,uint16_t data_len,uint8_t ack,uint8_t type );
uint16_t make_ack_frame(uint8_t *dsc,uint16_t id);
//uint16_t make_frame1(uint8_t *dst,uint8_t *data,uint16_t data_len,uint8_t ack,uint8_t type );
//uint16_t make_ack_frame1(uint8_t *dst,uint16_t id);
void analyze_frame(DdcFrame_t *frame);


void ddc_loop(void);

void ddc_recv_process(void);
void frame_retry(DdcNode_t *p);
void add_to_list(uint8_t *buf);

uint16_t get_len_frome_frame(uint8_t *frame);
uint16_t get_id_frome_frame(uint8_t *frame);
uint8_t get_ack_frome_frame(uint8_t *frame);
uint8_t get_type_frome_frame(uint8_t *frame);
void print_frame(uint8_t *frame);
void print_list(DdcNode_t *list);


void frame_send(DdcNode_t *p);
void check_ack_list(void);

#ifdef __cplusplus
}
#endif

#endif
 