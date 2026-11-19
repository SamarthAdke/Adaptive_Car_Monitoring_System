#include"header.h"

void delay_ms(u32 n)
{
        T0PR=60000-1;
        T0PC=0;
        T0TC=0;
        T0TCR=2;
        T0TCR=1;
        while(T0TC<n);
        T0TCR=0;
}
void delay_us(unsigned int us)
{
        T0PR=60-1;
        T0PC=0;
        T0TC=0;
        T0TCR=0x02;
        T0TCR=0x01;
        while(T0TC<us);
        T0TCR=0x01;
}

 void delay_s(u32 n)
{
        T0PR=60000000-1;
        T0PC=0;
        T0TC=0;
        T0TCR=2;
        T0TCR=1;
        while(T0TC<n);
        T0TCR=0;
}
