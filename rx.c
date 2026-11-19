#include"header.h"
extern s32 can_flag;
extern frame receive;

//p0.0->Left
//p0.1->head
//p0.2->Right
int main()
{
        u32 val1,val3,percent;
        float val2;
        u8 i=0, head_flag=0;
        can1_init();
        config_VIC();
        uart0_init(9600);

        IODIR0=led;
        IOSET0=led;
        IOCLR0=0;

        uart0_tx_str("node B\r\n");

        while(1)
        {
                delay_ms(900);

                if(can_flag==1)
                {
                        uart0_tx_str("Vehical dashboard\r\n");
                        uart0_tx_str("-------------------------\r\n");
                        can_flag=0;
                        delay_ms(250);
                        if((receive.id==0x11))
                        {
                                uart0_tx_str("\r\nLight Status: ");
                                if((receive.byteA&0xff)==0x68)
                                {
                                        if(head_flag==0)
                                        {
                                                        uart0_tx_str("\r\nHead Light ON");
                                                        IOCLR0=(1<<7);
                                                        head_flag=1;
                                        }
                                        else
                                        {
                                                        uart0_tx_str("\r\nHead Light OFF");
                                                        IOSET0=(1<<7);
                                                        head_flag=0;
                                        }
                                }
                                if((receive.byteA&0xff)==0x72)
                                {
                                        uart0_tx_str("\r\nRight Indicator ON");
                                        IOSET0=(1<<5);
                                        for(i=0;i<20;i++)
                                        {
                                                        IOCLR0=(1<<2);
                                                        delay_ms(70);
                                                        IOSET0=(1<<2);
                                                        delay_ms(70);
                                        }
                                }
                                if((receive.byteA&0xff)==0x6C)
                                {
                                        uart0_tx_str("\r\nLeft Indicator ON");
                                        IOSET0=(1<<2);
                                        for(i=0;i<20;i++)
                                        {
                                                        IOCLR0=(1<<5);
                                                        delay_ms(70);
                                                        IOSET0=(1<<5);
                                                        delay_ms(70);
                                        }
                                }
                                delay_ms(50);
                                continue;
                        }

                else
        {

                                val1=(receive.byteA&0xffff);//speed data
                                val2=receive.byteB;
                                val3=((receive.byteA>>16)&0xff);// fuel data

                                        percent=(((170-(val3))*100)/130);
                                        //fuel_per_conversion(&fuel_data);
                                        if(receive.rtr==0)
                                        {
//                                              uart0_tx_str("\r\nid: ");
//                                              uart0_hex(receive.id);
//                                              uart0_tx_str("\r\n");

                                                uart0_tx_str("\r\nspeed: ");
                                                uart0_tx_int(val1);
                                                uart0_tx_str("  ");
                                                uart0_tx_str("Km/hr\r\n");

                                                uart0_tx_str("\r\ntemp: ");
                                                uart0_tx_float(val2);
                                                uart0_tx_str("  ");
                                                uart0_tx_str(" C\r\n");

                                                uart0_tx_str("\r\nfuel status: ");
                                                uart0_tx_str("\r\npercent: ");
                                                uart0_tx_int(percent);
                                                if(percent>0 && percent <30)
                                                {
                                                                uart0_tx_int(percent);
                                                                uart0_tx_str("  LOW\r\n");
                                                }
                                                else if(percent>=30 && percent <60)
                                                {
                                                                uart0_tx_int(percent);
                                                                uart0_tx_str("  Med\r\n");
                                                }
                                                else if(percent>=60 && percent <100)
                                                {
                                                                uart0_tx_int(percent);
                                                                uart0_tx_str("  Full\r\n");
                                                }
                                                else
                                                {
                                                        continue;
                                                }
                                        }
                                        else
                                        {
                                                uart0_tx_str("Remote frame requested\r\n");
                                        }
                                }
                                delay_ms(900);
                        }
                }

}