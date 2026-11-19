#include"header.h"
s32 can_flag=0;
frame receive;
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

void uart0_hex(u32 num){

        char buf[10];

        sprintf(buf,"%x",num);
        buf[9]='\0';
        uart0_tx_str(buf);

}

void config_VIC(void)
{
        VICIntSelect=0x0;
        VICVectCntl0=(1<<5)|(26);
        VICVectAddr0=(unsigned long int)isr_handler;
        VICIntEnable=1<<26;
        C1IER=1;
}

void isr_handler(void) __irq
{
        can_flag=1;
        receive.id= C1RID;
        receive.dlc=((C1RFS>>16)&0x0f);
        receive.rtr=(C1RFS>>30)&1;
        receive.ff=(C1RFS>>31)&1;
        if(receive.rtr==0)
        {
                receive.byteA=C1RDA;
                receive.byteB=C1RDB;
        }
        VICVectAddr=0;
        C1CMR=0x04;
}