#include"header.h"
//p0.28 -> engine temperature

float temp_read()
{
                u32 val=0,i=0;
                float vout=0,temp=0;
                for(i=0;i<10;i++)
                {
                                val=adc_read(1);// lm35 on p0.26
                                vout=(val*0.322265);
                                temp=temp+vout;
                                delay_ms(20);
                                vout=0;
                                val=0;
                }
                vout=temp/10.0;
                return vout;
}