#include"header.h"
CAN1 msg;
u32 iflag;
int main()
{
int lin=1,rin=1;
float vout,tempertature;
can2_init();
lcd_init();
uart0_init(115200);
config_vic_for_can2();
en_can2_interrupt();
uart0_tx_string("Reciever node\r\n");
while(1)
{
	if(iflag==1)
	{
		iflag=0;
		uart0_tx_hex(msg.byteA);
		uart0_tx_string("\n\r");
		if(msg.byte==0x312)   //turn OFF left indicator
			if(lin==1)
				lin=0;
		if(msg.byteA==0x311)  //turn OFF right indicator 
				if(rin==1)
						rin=0;
		
		if(msg.id==0x311 || lin==1)	     //Check for Left indicator
		{
			if((msg.byteA)==0x12)
					lin=1;
			else if((msg.byteA)==0x13)
					lin=0;
			if(((msg.byteA)==0x12) || lin==1)
			{
				cgram_leftIndc();
				lcd_cmd(0xC0);
				lcd_data(0);
				delay_ms(250);
				lcd_cmd(0xC0);
				lcd_data(' ');
				delay_ms(250);
				uart0_tx_string("Left Indi ON\r\n");
			}
			else if(((msg.byteA)==0x13))
			{
				lcd_cmd(0xC0);
				lcd_data(' ');
				uart0_tx_string("LEFT Indi OFF\r\n");
			}
		}
		if(msg.id==0x312 || rin==1)	     //Check for right indicator
		{
			if((msg.byteA)==0x14)
					rin=1;
			else if((msg.byteA)==0x15)
					rin=0;
			
			if(((msg.byteA)==0x14 || rin==1))
			{
				cgram_rightIndc();
				lcd_cmd(0xC2);
				lcd_data(1);
				delay_ms(250);
				lcd_cmd(0xC0);
				lcd_data(' ');
				delay_ms(250);
				uart0_tx_string("Left Indi ON\r\n");
			}
			else if(((msg.byteA)==0x15))
			{
				lcd_cmd(0xC2);
				lcd_data(' ');
				uart0_tx_string("LEFT Indi OFF\r\n");
			}
		}
		
		if(msg.id==0x310)    //HL Control
		{
				if(msg.byteA==0x10)  //HL ON
				{
						cgram_hl();
						lcd_cmd(0x80);
						lcd_data(2);
				}
				else if(msg.byteA==0x09)   //HL OFF
				{
						lcd_cmd(0x80);
						lcd_data(' ');
				}
		}
		if(msg.id==0x309)
		{
				vout=(msg.byteA*3.3)/1023;
				temperature=(vout-0.5)/0.01;
				lcd_cmd(0x8A);
				lcd_float(temperature);
				lcd_data('C');
		}
		if(msg.id==0x308)
		{
				if(msg.byteA>=0 && msg.byteA<=250)
				{
						cgram_bA();
						lcd_data(3);
						lcd_cmd(0x82);
				}
				else if(msg.byteA>=251 && msg.byteA<=500)
				{
						cgram_bB();
						lcd_data(4);
						lcd_cmd(0x82);
				}
				else if(msg.byteA>=501 && msg.byteA<=750)
				{
						cgram_bC();
						lcd_data(5);
						lcd_cmd(0x82);
				}
				else if(msg.byteA>=751 && msg.byteA<=1024)
				{
						cgram_bD();
						lcd_data(6);
						lcd_cmd(0x82);
				}
			}	
		}	
	}
}

