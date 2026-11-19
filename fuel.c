#include"header.h"

//po.1 -> echo
//p0.4 -> trigger
        #define TRIG (1<<4)
        #define ECHO ((IOPIN0>>1)&1)

u32 read_ultra()
{       u32 time=0, val=0;

                IODIR0=TRIG;
                IOSET0=TRIG;

                T0PR=60-1;
                T0PC=0;
                T0TC=0;
                T0TCR=0x02;

                IOCLR0=TRIG;
                delay_us(2);
                IOSET0=TRIG;
                delay_us(10);
                IOCLR0=TRIG;

                while(ECHO==0);
                T0TCR=0x01;
                while(ECHO==1);

                T0TCR=0;
                time=T0TC;
                val=(u32)((0.034*time)/2);
                return val;
}

u32 read_fuel()
{
        u32 dis=0,temp=0,i=0;
        //      uart0_tx_str("flag 2\r\n");
        for(i=0;i<10;i++)
        {
                delay_ms(10);
                temp=read_ultra();
                delay_ms(10);
//              uart0_tx_int(temp);
//              uart0_tx_str("\r\n");
//              delay_ms(500);
                dis=dis+temp;
                temp=0;
        }
//      uart0_tx_str("flag 7 \r\n");
        temp=0;
        temp=dis/10;
        if(temp>0 && temp<256)
        {
                return temp;
        }
        else
        {
                temp=100;
                return temp;
        }
}
