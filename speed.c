 #include"header.h"
//p0.27 -> speed

u32 speed_read(void)
{
        u32 temp1,temp2, temp3,i=0;
        temp3=0;
        for(i=0;i<10;i++)
        {
                temp1=adc_read(0); //potentiometer on 0th channel
                temp2=((temp1*120)/1023);
                temp3=temp3+temp2;
//              uart0_tx_int(temp3);
//              uart0_tx_str("\t");
                temp1=0;
                temp2=0;
        }
        temp3=temp3/10;
        return temp3;
}