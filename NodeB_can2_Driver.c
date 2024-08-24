#include"header.h"
extern unsigned int iflag;
extern CAN2 msg;
void can2_init(void)
{
PINSEL1|=0x14000;
VPBDIV=1;
AFMR=2;
C2MOD=1;
C2BTR=0x001C001D;
C2MOD=0;
}

void CAN2_Rx_Handler(void)__irq
{
msg.id=C2RID;
msg.dlc=(C2RFS>>16)&0xF;
msg.rtr=(C2RFS>>30)&1;
if(msg.rtr==0)
{
msg.byteA=C2RDA;
msg.byteB=C2RDB;
}
C2CMR=(1<<2); //Release reciever buffer
iflag=1;
VICVectAddr=0;
uart0_tx_string("In ISR, Reciever node\r\n");
}

void config_vic_for_can2(void)
{
VICIntSelect=0;
VICVectCntl2=27|(1<<5);
VICVectAddr2=(unsigned int)CAN2_Rx_Handler;
VICIntEnable=(1<<27);
}

void en_can2_interrupt(void)
{
C2IER=1; //Enable CAN2 Rx Interrupt
}

