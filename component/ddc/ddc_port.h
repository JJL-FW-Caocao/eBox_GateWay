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
void ddc_get_char(unsigned char ch);
int ddc_send_char(unsigned char ch);
int ddc_send_buf(unsigned char* buf, int buflen);

#ifdef __cplusplus
}
#endif

#endif

