#ifndef __DDC_PORT_H
#define __DDC_PORT_H

#ifdef __cplusplus
 extern "C" {
#endif
     
//#include "stdio.h"
#include "fifo.h"
#define DDC_MAX_LENGTH 64

extern struct ebox_fifo *fifo_recv;
extern struct ebox_fifo *fifo_payload;


void ddc_init();
void ddc_get_char(uint8_t ch);
uint16_t ddc_send_char(uint8_t ch);
uint16_t ddc_send_buf(uint8_t* buf, uint16_t buflen);

#ifdef __cplusplus
}
#endif

#endif

