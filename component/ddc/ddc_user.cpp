
#include "ddc_user.h"
#include "ebox.h"

//�ص��������

Exti Io0(&PA8, FALLING);

void exit()
{
    uart1.printf("\r\nxx = %d");
    PB8.toggle();
}
void mode()
{
    Io0.begin();
    Io0.attach(exit);
    Io0.interrupt(ENABLE);
}
//make_frame�ĵ���
