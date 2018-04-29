#include "ddc_port.h"

struct ebox_fifo *fifo_recv;

struct ebox_fifo *fifo_payload;

void ddc_init()
{
    fifo_recv = ebox_fifo_alloc(DDC_MAX_LENGTH);
    fifo_payload = ebox_fifo_alloc(DDC_MAX_LENGTH);

}
void ddc_get_char(unsigned char ch)
{
    ebox_fifo_put(fifo_recv,&ch,1);
}
int ddc_send_char(unsigned char ch)
{
    /**************
        user add your send api.
    **************/
    return 1;
}
int ddc_send_buf(unsigned char* buf, int buflen)
{
    /**************
        user add your send api.
    **************/
    return buflen;
}