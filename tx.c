#include"header.h"

u8 sw_flag=0, head=0, rg=0, lf=0;
frame data, sw;
int main()
{


u32 res=0, num=0,num1=0;
        float  res2=0;

        PINSEL0=0x0;
        PINSEL1=0X0;


        can1_init();
        adc_init();
        uart0_init(9600);
        eint_init();
        config_VIC();


        data.id=0x18;
        data.rtr=0;
        data.dlc=8;
        data.byteA=0x00000000;
        data.byteB=0x00000000;
        data.ff=0; //std frame;

        sw.rtr=0;
        sw.dlc=1;
        sw.ff=0;
        while(1)
        {

                if(sw_flag==1)
                {
                        sw_flag=0;
                        if(head==1)
                        {
                                head=0;
                                sw.id=0x11;
                                sw.byteA=0x00000068;
                                can1_tx(sw);
                                uart0_tx_str("head sw\r\n");
                                continue;
                        }
                        if(rg==1)
                        {
                                rg=0;
                                sw.id=0x11;
                                sw.byteA=0x00000072;
                                can1_tx(sw);
                                uart0_tx_str("right sw\r\n");
                                continue;
                        }
                        if(lf==1)
                        {
                                lf=0;
                                sw.id=0x11;
                                sw.byteA=0x0000006C;
                                can1_tx(sw);
                                uart0_tx_str("left sw\r\n");
                                continue;
                        }
                }
                else {
                res=speed_read();
                res=res&0x0000ffff;
                data.byteA|=res;

                num1=read_fuel();
                num1&=0xff;
                num=((num1<<16)&0x00ff0000);
                data.byteA|=num;

                res2=temp_read();
                data.byteB=res2;

                can1_tx(data);
        //      uart0_tx_str("data transmitted\r\n");
                uart0_tx_str("\r\nspeed: ");
                uart0_tx_int(res);
                uart0_tx_str("\r\nfuel: ");
                uart0_tx_int(num1);
                uart0_tx_str("\r\ntemp: ");
                uart0_tx_float(res2);
                uart0_tx_str("data transmitted\r\n");
                }

                delay_ms(50);
        }

}