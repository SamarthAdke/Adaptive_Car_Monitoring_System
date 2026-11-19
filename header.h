#include<lpc21xx.h>
#include<stdio.h>
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u32;
typedef signed int s32;

void delay_us(u32);
void delay_ms(u32);
void delay_s(u32);

#define led (1<<5|1<<7|1<<2)
#define THRE ((U0LSR>>5)&1)
#define RDA ((U0LSR>>5)&1)
#define done ((ADDR>>31)&1)

void uart0_init(u32);
void uart0_tx(u8);
void uart0_tx_str(char *);
void uart0_tx_int(s32);
void uart0_hex(u32);

typedef struct CAN_frame
{
        u32 id;
        u32 byteA;
        u32 byteB;
        u8 rtr;
        u8 dlc;
        u8 ff;
}frame;



void can1_init(void);
void config_VIC(void);
void isr_handler(void) __irq;

void adc_init(void);
u32 adc_read(u32);

u32 speed_read(void);
float temp_read(void);
u32 read_ultra(void);
u32 read_fuel(void);

void uart0_tx_float(float);