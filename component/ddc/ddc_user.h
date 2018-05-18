#ifndef __DDC_USER_H
#define __DDC_USER_H
#include "ebox.h"
#include "ddc.h"

class XIo
{

    public:
    XIo(Gpio *io){
        this->io = io;
    };

    void mode(uint8_t mode)
        {
            switch(mode)
            {
                case 0:
                    io->mode(OUTPUT_PP);
                case 1:
                    Exti temp(io,CHANGE);
                    
                
            }
        }
    private:
        Gpio *io;
};

#endif
