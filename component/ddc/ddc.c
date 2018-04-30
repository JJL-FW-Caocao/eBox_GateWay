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
static uint16_t ddc_id;

static uint8_t temp_buf[DDC_MAX_LENGTH];
void ddc_recv_process()
{
    uint8_t ch;
    uint8_t len;
    static uint16_t counter = 0;
    static uint16_t frame_payload_len = 0;
    static uint8_t head[2];
    static DataU16_t frame_len;
    static DataU16_t crc_value;
    uint16_t t = 0xf119;
    

    len = ebox_fifo_get(fifo_recv,&ch,1);
    if(len == 1)
    {
//        ebox_printf("0x%02x\r\n",ch);
//        ebox_fifo_put(&fifo_recv_temp,&ch,1);//缓存
        switch((uint8_t)ddc_recv_state)
        {
            case DDC_HEAD:
                head[0] = head[1];
                head[1] = ch;
                if((head[0] == 0x55) && (head[1] == 0xaa))
                {
                    ddc_recv_state = DDC_LEN;
                    head[1] = 0x00;
                }
                ebox_printf("head->");
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
                ebox_printf("len->");

                break;
            case DDC_PAYLOAD:
                temp_buf[2 + counter++] = ch;
                if(counter == (frame_payload_len - 2))
                {
                    counter = 0;
                    ddc_recv_state = DDC_CRC;
                }
                ebox_printf("payload->");
                break;
            case DDC_CRC:
                ebox_printf("crc->");
                crc_value.byte[counter++] = ch;
                if(counter == 2)
                {
                    if(crc16(temp_buf,frame_payload_len) == crc_value.value)//crc ok
                    {
                        counter = 0;
                        ebox_fifo_put(fifo_payload,temp_buf,frame_payload_len);
                        analyze_payload();
                        ddc_recv_state = DDC_HEAD;
                    }
                    else
                    {
                        ebox_printf("\r\ncrc = 0x%04x \r\n",crc_value.value);
                        ebox_printf("\r\ncrc = 0x%04x \r\n",crc16(temp_buf,frame_payload_len));
                        
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
    uint8_t buf[12];
    DataU16_t payload_len;
    DataU16_t payload_id;
    uint8_t ack;
    uint8_t type;
    DataU16_t id;
    
    
    ebox_fifo_get(fifo_payload,&payload_len.byte[0],2);
    ebox_fifo_get(fifo_payload,&payload_id.byte[0],2);
    ebox_fifo_get(fifo_payload,&ack,1);
    ebox_fifo_get(fifo_payload,&type,1);
    ebox_fifo_get(fifo_payload,&id.byte[0],2);
    
    switch(type)
    {
        case 0:
            break;
        case 1:
            break;
        case 2:
            ebox_printf("case 2\r\n");
            break;
        case 3:
            ebox_printf("case 3\r\n");
            break;
        case 255:
            ebox_printf("\r\n接收到回复:id = %d\r\n",id.value);
            //接收到回复帧，删除等待回复链表中的对应节点
            list_delete_by_val(&list_wait_ack,id.value);        
            break;
    }
    if(ack == 1)
    {
        //创建回复帧，并插入发送队列
        make_ack_frame(buf,payload_id.value);
        add_to_list(buf);

    }
    ebox_printf("type:%d\r\n",type);

}
unsigned int ddc_get_id()
{
    return ddc_id++;
}
uint16_t make_ack_frame(uint8_t *dsc,uint16_t id)
{
    DataU16_t _id;
    uint16_t frame_len = 2 + 6 + 4;
    DdcNode_t *node_frame;
//    unsigned char buf[DDC_MAX_LENGTH];
    uint16_t i = 0,k = 0; 
    uint8_t m;
    DataU16_t payload_len;
    DataU16_t frame_id;//帧本身的id
    DataU16_t crc;
    _id.value = id;//需要回复的id

    payload_len.value = 2 + 6;
    frame_id.value = ddc_get_id();
    
    dsc[i++] = 0x55;
    dsc[i++] = 0xAA;
    
    dsc[i++] = payload_len.byte[0];
    dsc[i++] = payload_len.byte[1];
    
    dsc[i++] = frame_id.byte[0];
    dsc[i++] = frame_id.byte[1];
    
    dsc[i++] = 0;//ack no need
    
    dsc[i++] = 255;// type
    
    dsc[i++] = _id.byte[0];
    dsc[i++] = _id.byte[1];
    
    crc.value = crc16(&dsc[2],payload_len.value);
    
    dsc[i++] = crc.byte[0];
    dsc[i++] = crc.byte[1];
    return i;

}
uint16_t make_frame(uint8_t *dsc,uint8_t *data,uint16_t data_len,uint8_t ack,uint8_t type )
{
    uint16_t frame_len = data_len + 6 + 4;
    DdcNode_t *node_frame;
//    unsigned char buf[DDC_MAX_LENGTH];
    uint16_t i = 0,k = 0; 
    uint8_t m;
    DataU16_t payload_len;
    DataU16_t frame_id;
    DataU16_t crc;

    payload_len.value = data_len + 6;
    frame_id.value = ddc_get_id();
    
    dsc[i++] = 0x55;
    dsc[i++] = 0xAA;
    
    dsc[i++] = payload_len.byte[0];
    dsc[i++] = payload_len.byte[1];
    
    dsc[i++] = frame_id.byte[0];
    dsc[i++] = frame_id.byte[1];
    
    dsc[i++] = ack;
    
    dsc[i++] = type;
    
    for( k = 0; k < data_len; k++ )
        dsc[i++] = *data++;
    
    crc.value = crc16(&dsc[2],payload_len.value);
    
    dsc[i++] = crc.byte[0];
    dsc[i++] = crc.byte[1];
    return i;

}
void add_to_list(uint8_t *buf)
{
    uint8_t ack ;
    uint16_t frame_len;
    uint16_t frame_id ;
    
    frame_len = get_len_frome_frame(buf) + 4;
    ack = get_ack_frome_frame(buf);
    frame_id = get_id_frome_frame(buf);
    
    list_insert(&list_send,buf);
    ebox_printf("<<<<add frame to [send list] ok ;id = %d\t\tget free = %d\r\n",frame_id,ebox_get_free());
    
    if(ack == 1)
    {
        uint8_t *ack_buf = (uint8_t*)ebox_malloc(frame_len);
        ebox_memcpy(ack_buf,buf,frame_len);
        list_insert(&list_wait_ack,ack_buf);
        ebox_printf("<<<<add frame to [WaitAck list] ok ;id = %d\tget free = %d\r\n",frame_id,ebox_get_free());

    }

}
void frame_retry(DdcNode_t *frame)
{
    DdcNode_t *p = frame;
    uint16_t payload_len = get_len_frome_frame(p->p);
    uint16_t id = get_id_frome_frame(p->p);
    uint8_t ack = get_ack_frome_frame(p->p);
    uint8_t type = get_type_frome_frame(p->p);
    uint8_t *buf = (uint8_t *)ebox_malloc(payload_len+4);

//    print_frame(p->p);
    make_frame(buf,&(p->p[8]),(payload_len-6),ack,type);
    add_to_list(buf);
    no_interrupts();//原子操作
    list_delete_by_val(&list_wait_ack,id);
    interrupts();
}
void frame_send(DdcNode_t *p)
{
    uint16_t len,id;
    if(p->next != NULL)
    while(p->next != NULL)
    {
        p = p->next;
        len = get_len_frome_frame(p->p);
        id = get_id_frome_frame(p->p);
//        ebox_printf("len = %d,id=%d\r\n",len,id);
//        print_frame(p->p);

        ddc_send_buf(p->p,len);
        ebox_printf(">>>>send frame ok:id = %d;\t get free:%d\r\n",id,ebox_get_free());
        
        no_interrupts();//原子操作
        list_delete_by_val(&list_send,id);
        interrupts();
//        ebox_printf(">>>>delete frame:id = %d;\t get free:%d\r\n",id,ebox_get_free());
    }
}
void frame_send_node(DdcNode_t *p)
{
    uint16_t len,id;
    if(p != NULL)
    {
        len = get_len_frome_frame(p->p);
        id = get_id_frome_frame(p->p);
        ddc_send_buf(p->p,len);
        ebox_printf(">>>send frame ok.delete frame:id = %d;\t get free:%d\r\n",id,ebox_get_free());
    }
}
static uint64_t ack_timer ;
void check_ack_list()
{
    DdcNode_t *temp = &list_wait_ack;
    uint16_t i = 0;
    uint16_t len,id;
    if(temp->next == NULL) return;
    //调试用
    do
    {
        temp = temp->next;
        ebox_printf("等待回复列表:p->timer = %d;id = %d\r\n",temp->time,get_id_frome_frame(temp->p));
    }while (temp->next != NULL) ;
    temp = &list_wait_ack;
    ///////////////////////////////////
    if(millis() - ack_timer > 1000)
    {
        ack_timer = millis();
        do
        {
            temp = temp->next;
            temp->time++;
            if(temp->time >= 2) 
            {
                

//               print_frame(temp->p);
                ebox_printf("超时列表:p->timer = %d;id = %d\r\n",temp->time,get_id_frome_frame(temp->p));
                ebox_printf("创建重新发送帧，并插入发送链表\r\n",temp->time,get_id_frome_frame(temp->p));

                frame_retry(temp);
            }
//            ebox_printf("【】check ack:p->timer = %d\r\n",temp->time);
        }while (temp->next != NULL) ;

    }

//    while(p->next != NULL)
//    {
//        p = p->next;
//        p->time++;
//        if(p->time > 3)
//            frame_retry(&list_wait_ack);
//    }
}
uint16_t get_len_frome_frame(uint8_t *frame)
{
    DataU16_t temp;
    temp.byte[0] = frame[2];
    temp.byte[1] = frame[3];
    return temp.value;
}

uint16_t get_id_frome_frame(uint8_t *frame)
{
    DataU16_t temp;
    temp.byte[0] = frame[4];
    temp.byte[1] = frame[5];
    return temp.value;
}
uint8_t get_ack_frome_frame(uint8_t *frame)
{
    return frame[6];
}
uint8_t get_type_frome_frame(uint8_t *frame)
{
    return frame[7];
}
void print_frame(uint8_t *frame)
{
    int i;
    uint8_t *p = frame;
    uint16_t len = get_len_frome_frame(p) + 4;
    for(i = 0; i < len;i++)
        ebox_printf("0X%02X ",*p++);
    ebox_printf("\r\n");
}
void print_list(DdcNode_t *list)
{
    uint16_t len,id;
    DdcNode_t *p = list;
    if(p->next != NULL)
    while(p->next != NULL)
    {
        p = p->next;
        len = get_len_frome_frame(p->p) + 4;
        id = get_id_frome_frame(p->p);
        ebox_printf("len = %d,id=%d\r\n",len,id);
        print_frame(p->p);

    }

}
void ddc_loop()
{
    int i;
    frame_send(&list_send);
    check_ack_list();
    for(i = 0; i < 50; i++)
        ddc_recv_process();
}

