#include <STC15F2K60S2.H>
#include <intrins.h>
#include "iic.h"

sbit SCL=P2^0;
sbit SDA=P2^1;
bit S4,S5,S6,S7;
unsigned char code Sun_Tube[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,
							0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,
							0xbf,0x7f,0xff};
unsigned char Num;
							
void Delay(unsigned int m)		//@11.0592MHz
{
	unsigned char i, j;

	while(m--)
	{
		_nop_();
		_nop_();
		_nop_();
		i = 11;
		j = 190;
		do
		{
			while (--j);
		} while (--i);
	}
}


void Shut_Off()
{
	P2=P2&0x1f|0x80;
	P0=0xff;
	P2=P2&0x1f|0xa0;
	P0=0x00;
}

void Display(unsigned char rank,unsigned char num)
{
	P2=P2&0x1f|0xc0;
	rank--;
	P0=0x01<<rank;
	P2=P2&0x1f|0xe0;
	P0=Sun_Tube[num];
	Delay(1);
	P2=P2&0x1f|0xc0;
	P0=0x00;
}

unsigned char AT24C02_Read(unsigned char Address)
{
	unsigned char Data;
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(Address);
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(0xa1);
	IIC_WaitAck();
	Data=IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop();
	
	return Data;
}



void AT24C02_Write(unsigned char Address,Data)
{
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(Address);
	IIC_WaitAck();
	
	IIC_SendByte(Data);
	IIC_WaitAck();
	IIC_Stop();
}

void Key_BTN()
{
	if(P30==0 && S7==0)
		if(P30==0)
			S7=1;
	if(P31==0 && S6==0)
		if(P31==0)
			S6=1;
	if(P32==0 && S5==0)
		if(P32==0)
			S5=1;
	if(P33==0 && S4==0)
		if(P33==0)
			S4=1;
	
	
	if(P30==1 && S7==1)
	{
		S7=0;
		Num++;
	}
	if(P31==1 && S6==1)
	{
		S6=0;
		Num--;
	}
	if(P32==1 && S5==1)
	{
		S5=0;
		AT24C02_Write(0,Num);
	}
	if(P33==1 && S4==1)
	{
		S4=0;
		Num=AT24C02_Read(0);
	}
}
//void At24C02_Write(unsigned char Address,Data)
//{
//	IIC_Start();
//	IIC_SendByte(0xa0);
//	IIC_WaitAck();
//	IIC_SendByte(Address);
//	IIC_WaitAck();
//	IIC_SendByte(Data);
//	IIC_WaitAck();
//	IIC_Stop();
//}

//void AT24C02_Read(unsigned char Address)
//{
//	unsigned char Data;
//	IIC_Start();
//	IIC_SendByte(0xa0);
//	IIC_WaitAck();
//	IIC_SendByte(Address);
//	IIC_WaitAck();
//	
//	IIC_Start();
//	IIC_SendByte(0xa1);
//	IIC_WaitAck();
//	Data=IIC_ReceiveByte();
//	IIC_SendByte(1);
//	IIC_Stop();
//	
//	
//}





void main()
{
	Shut_Off();
	while(1)
	{
		Key_BTN();
		Display(1,Num/10);
		Display(2,Num%10);
	}
}