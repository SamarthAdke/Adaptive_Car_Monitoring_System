#include"header.h"

extern u8 sw_flag, head, rg, lf;
void uart0_init(u32 baud)
{
        unsigned int pclk=0,res=0;
        if(VPBDIV==0)
                                pclk=15000000;
        else if(VPBDIV==1)
                                pclk=60000000;
        else if(VPBDIV==2)
                                pclk=30000000;


        res=pclk/(16*baud);
        PINSEL0|=0x05;
        U0LCR=0x83;
        U0DLL=(res&0xff);
        U0DLM=((res>>8)&0xff);
        U0LCR=0x03;
}

void uart0_tx(u8 data)
{
        U0THR=data;
        while(THRE==0);
}

void uart0_tx_str(char *s)
{
        while(*s)
        {
                uart0_tx(*s);
                s++;
        }
}
/*unsigned char uart0_rx()
{
        while(RDA==0);
        return U0RBR;
} */

void uart0_tx_int(s32 num)
{
        char a[10];
        int i=0;
        if(num<0)
        {
                uart0_tx('-');
                num=-num;
        }
        if(num==0)
        {
                uart0_tx('0');
                return;
        }
        while(num)
        {
                a[i]=num%10+48;
                num=num/10;
                i++;
        }
        i--;
        while(i>=0)
        {
                uart0_tx(a[i]);
                i--;
        }

}

void uart0_tx_float(float val)
{
        int temp;
        if(val==0)
        {
                uart0_tx_str("0.0");
                return;
        }
        if(val<0)
        {
                uart0_tx('-');
                val=-val;
        }
        temp=val;
        uart0_tx_int(temp);
        temp=(val-temp)*1000000;
        uart0_tx('.');
        uart0_tx_int(temp);
}

void can1_init(void)
{
                VPBDIV=1;
                PINSEL1|=0x00040000;
                C1MOD=1;
                AFMR=2;
                C1BTR=0x001C001D;
                C1MOD=0;
}

void can1_tx(frame t)
{
        C1TID1=t.id;
        C1TFI1=((t.dlc&0x0f)<<16);
        C1TFI1|=((t.ff&0x01)<<31);              //
        if(t.rtr==0)
        {
                C1TDA1=t.byteA;
                C1TDB1=t.byteB;
        }
        else
        {
                C1TFI1|=(1<<30);
        }
        C1CMR=0x21;
        while(TCS==0);
}



 void adc_init()
{
                PINSEL1|=0x05400000;
                ADCR=0x00200400;
}

u32 adc_read(u32 ch_num)
{
                u32 result=0;
                ADCR|=1<<ch_num;
                ADCR|=1<<24;
                while(done==0);
                ADCR^=1<<24;
                ADCR^=1<<ch_num;
                result=(ADDR>>6)&(0x3ff);
                return result;
}

void head_handler(void) __irq
{
        sw_flag=1;
        head=1;
        EXTINT=1<<0;
        VICVectAddr=0;
}

void right_handler(void) __irq
{
        sw_flag=1;
        rg=1;
        EXTINT=1<<1;
        VICVectAddr=0;
}

void left_handler(void) __irq
{
        sw_flag=1;
        lf=1;
        EXTINT=1<<2;
        VICVectAddr=0;
}

void eint_init(void)
{
        PINSEL0|=0x0000C0C0;//EINT2 -> PO.7  EINT1 -> P0.3(right)
        PINSEL1|=0x00000001;//EINT0 -> P0.16(head)
        EXTMODE=0x7;
        EXTPOLAR=0x0;
}

void config_VIC(void)
{
        VICIntSelect=0;
        VICVectCntl0=(1<<5)|14;
        VICVectCntl1=(1<<5)|15;
        VICVectCntl2=(1<<5)|16;

        VICVectAddr0=(unsigned long int)head_handler;
        VICVectAddr1=(unsigned long int)right_handler;
        VICVectAddr2=(unsigned long int)left_handler;

        VICIntEnable=(1<<14|1<<15|1<<16);
}
