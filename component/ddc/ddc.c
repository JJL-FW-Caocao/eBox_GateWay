#include "ddc.h"
#include "crc.h"

DdcNode_t list_send;
DdcNode_t list_wait_ack;
enum 
{
    DDC_HEAD,
    DDC_LEN,
    DDC_PAYLOAD,
    DDC_CRC,
    DDC_OK,

}ddc_recv_state;
static unsigned int ddc_id;

static unsigned char temp_buf[DDC_MAX_LENGTH];
void ddc_recv_process()
{
    unsigned char ch;
    unsigned char len;
    static unsigned int counter = 0;
    static unsigned int frame_payload_len = 0;
    static unsigned char head[2];
    static Data16_t frame_len;
    static Data16_t crc_value;
    

    len = ebox_fifo_get(fifo_recv,&ch,1);
    if(len == 1)
    {
//        ebox_printf("0x%02x\r\n",ch);
//        ebox_fifo_put(&fifo_recv_temp,&ch,1);//缓存
        switch((unsigned char)ddc_recv_state)
        {
            case DDC_HEAD:
                head[0] = head[1];
                head[1] = ch;
                if((head[0] == 0x55) && (head[1] == 0xaa))
                {
                    ddc_recv_state = DDC_LEN;
                    head[1] = 0x00;
                }
                break;
            case DDC_LEN:
                temp_buf[counter] = ch;
                frame_len.byte[counter++] = ch;
                if(counter == 2)
                {
                    if(frame_len.value <= DDC_MAX_LENGTH)
                    {
                        frame_payload_len = frame_len.value;
                        counter = 0;
                        ddc_recv_state = DDC_PAYLOAD; 
                    }
                    else
                        
                        ddc_recv_state = DDC_HEAD;
                }
                break;
            case DDC_PAYLOAD:
                temp_buf[2 + counter++] = ch;
                if(counter == (frame_payload_len - 2))
                {
                    ddc_recv_state = DDC_CRC;
                    counter = 0;
                }
                break;
            case DDC_CRC:
                crc_value.byte[counter++] = ch;
                if(counter == 2)
                {
                    ebox_printf("\r\ncrc = 0x%04x\r\n",crc_value.value);
                    ebox_printf("\r\npay = 0x%04x\r\n",frame_payload_len);
                    if(true)//crc ok
                    {
                        ebox_fifo_put(fifo_payload,temp_buf,frame_payload_len);
                        analyze_payload();
                    }
                    else
                    {
                        //call crc err deal function;
                    }
                }
                break;

            default:
                 
                break;
        }
        
    }
}

void analyze_payload()
{
    unsigned char ch;
    unsigned char len;
    static unsigned int counter = 0;
    Data16_t payload_len;
    Data16_t payload_id;
    unsigned char ack;
    unsigned char type;
    unsigned char buf[DDC_MAX_LENGTH];
    
    
    ebox_fifo_get(fifo_payload,&payload_len.byte[0],2);
    ebox_fifo_get(fifo_payload,&payload_id.byte[0],2);
    ebox_fifo_get(fifo_payload,&ack,1);
    ebox_fifo_get(fifo_payload,&type,1);
    ebox_fifo_get(fifo_payload,buf,payload_len.value - 6);
    switch(type)
    {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 255:
            //接收到回复帧，删除等待回复链表中的对应节点
                list_delete_by_val(&list_wait_ack,payload_id.value);        
            break;
    }
    if(ack == 1)
    {
        //创建回复帧，并插入发送队列
        make_frame(0,0,0,255);
    }
    
    
    
    ebox_printf("0x%02x",buf[0]);

}
unsigned int ddc_get_id()
{
    return ddc_id++;
}

void make_frame(unsigned char *data,unsigned int data_len,unsigned char ack,unsigned char type )
{
    unsigned int frame_len = data_len + 6 + 4;
    unsigned char *buf = (unsigned char*)ebox_malloc(frame_len);
    DdcNode_t *node_frame;
//    unsigned char buf[DDC_MAX_LENGTH];
    unsigned int i = 0,k = 0; 
    uint8_t m;
    Data16_t payload_len;
    Data16_t frame_id;
    Data16_t crc;

    payload_len.value = data_len + 6;
    frame_id.value = ddc_get_id();
    
    buf[i++] = 0x55;
    buf[i++] = 0xAA;
    
    buf[i++] = payload_len.byte[0];
    buf[i++] = payload_len.byte[1];
    
    buf[i++] = frame_id.byte[0];
    buf[i++] = frame_id.byte[1];
    
    buf[i++] = ack;
    
    buf[i++] = type;
    
    for( k = 0; k < data_len; k++ )
        buf[i++] = *data++;
    
    crc.value = crc16(&buf[2],payload_len.value);
    
    buf[i++] = crc.byte[0];
    buf[i++] = crc.byte[1];
//    ebox_printf("get free = %d\r\n",ebox_get_free());
//    for(m = 0;m<15;m++)
//    ebox_printf("0x%02x\r\n",buf[m]);
    
    list_insert(&list_send,buf);
    ebox_printf("add frame to send lisk ok ;\r\nid = %d\tget free = %d\r\n",frame_id.value,ebox_get_free());
    
    if(ack == 1)
    {
        list_insert(&list_wait_ack,buf);
        ebox_printf("add frame to WaitAck lisk ok ;\r\nid = %d\tget free = %d\r\n",frame_id.value,ebox_get_free());

    }
}
void frame_retry(DdcNode_t *p)
{
    unsigned int payload_len,id;
    unsigned char ack,type;
    payload_len = get_len_frome_frame(p->p);
    ack = get_ack_frome_frame(p->p);
    type = get_type_frome_frame(p->p);
    make_frame(&(p->p[8]),(payload_len-6),ack,type);
}
void frame_send()
{
    DdcNode_t *p = &list_send;
    unsigned int len,id;
    if(p->next != NULL)
    while(p->next != NULL)
    {
        p = p->next;
        len = get_len_frome_frame(p->p);
        id = get_id_frome_frame(p->p);
        ddc_send_buf(p->p,len);
        
        no_interrupts();//原子操作
        list_delete_by_val(&list_send,id);
        interrupts();
    }
}
void check_ack_list()
{
    DdcNode_t *p = &list_wait_ack;

    while(p->next != NULL)
    {
//        p = p->next;
//        p->time++;
//        if(p->time > 3000)
//            frame_retry(&list_wait_ack);
    }
}
unsigned int get_len_frome_frame(unsigned char *frame)
{
    Data16_t temp;
    temp.byte[0] = frame[2];
    temp.byte[1] = frame[3];
    return temp.value;
}

unsigned int get_id_frome_frame(unsigned char *frame)
{
    Data16_t temp;
    temp.byte[0] = frame[4];
    temp.byte[1] = frame[5];
    return temp.value;
}
unsigned int get_ack_frome_frame(unsigned char *frame)
{
    return frame[6];
}
unsigned int get_type_frome_frame(unsigned char *frame)
{
    return frame[7];
}
void ddc_loop()
{
    ddc_recv_process();
    frame_send();
    check_ack_list();
}

