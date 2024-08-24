#include"header.h"
extern int gltflag;
void int_handler_timer1(void)__irq
{
T1IR=1;
gltflag=1;
//T1TCR=2;
T1TC=0;
T1PC=0;
VICVectAddr=0;
}
void timer1_config(void)
{
int a[]={15,60,30,15,15};
unsigned int pclk;
pclk=a[VPBDIV]*1000000;
T1PC=0;
T1PR=pclk-1;
T1MR0=1;
T1MCR=1;   //Generate interrupt when T1TC==T1MR0
T1TCR=1;  //Reset T1TC and T1PC
}

void config_timer1_for_VIC()
{
VICIntSelect=0;
VICVectCntl0=(1<<5)|5;
VICVectAddr0=(unsigned int)int_handler_timer1;
VICIntEnable|=(1<<5);
}

