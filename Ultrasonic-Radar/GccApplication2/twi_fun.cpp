/*
 * twi_fun_c.c
 *
 * Created: 2023/4/22 21:39:32
 *  Author: Bonjir
 */ 
#include "twi_fun.h"

void TWI_Init(void)// TWI 接口的初始化
{//设置SCL的频率：1MHz CPU-50KHz SCL,2M-100K,8M-400K
	TWSR = 0x00; //最低2位为预分频设置(00-1,01-4,10-16,11-64)
	TWBR = 0x02; //位率设置，fscl=cpu频率/(16+2*TWBR*预分频值)
	TWCR = (1<<TWEN); //开启TWI
}

char TWI_Start(void)//发送Start信号，开始本次TWI通信
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);//发送Start信号
	while(!(TWCR &(1<<TWINT)));//等待Start信号发出
	
	return (TWSR & 0xf8) == TW_START;
}

void TWI_Stop(void)//发送Stop信号，结束本次TWI通信
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//发送Stop信号
}

char TWI_Write(unsigned char uc_data) //向TWI接口发送8位数据
{
	TWDR = uc_data;//8位数据存放在TWDR
	TWCR = (1<<TWINT)|(1<<TWEN);//发送TWDR中的数据
	while(!(TWCR &(1<<TWINT)));//等待数据发出
	
	return (TWSR & 0xf8)==TW_MT_SLA_ACK;
}

char TWI_Read_With_ACK(unsigned char *pdata)
{
	TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);//准备接收数据，并ACK
	while(!(TWCR &(1<<TWINT)));//等待接收数据
	*pdata = TWDR;
	return ((TWSR & 0xf8)==TW_SR_DATA_ACK);////数据已收到，已发ACK
}

char TWI_Read_With_ACK_Wait_And_Do(unsigned char *pdata, void (*WaitFunc)())
{
	TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);//准备接收数据，并ACK
	while(!(TWCR &(1<<TWINT)))
	if(WaitFunc != NULL)
	WaitFunc();//等待接收数据
	*pdata = TWDR;
	return ((TWSR & 0xf8)==TW_SR_DATA_ACK);////数据已收到，已发ACK
}

unsigned char TWI_Read_With_NACK(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN);//准备接收数据，并NACK
	while(!(TWCR &(1<<TWINT)));//等待接收数据
	return TWDR;//返回接收到的数据
}

unsigned char TWI_Get_State_Info(void)
{
	unsigned char uc_status;
	uc_status = TWSR & 0xf8;
	return uc_status;
}

char TWI_Slave_Start()
{
	TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);//清标志，开TWI，自动应答
	while(!(TWCR & (1<<TWINT)));//等待接收sla+w
	return ((TWSR & 0xf8)==TW_SR_SLA_ACK); //sla+W已收到，已发ACK
}

char TWI_Slave_Start_Wait_And_Do(void (*WaitFunc)())
{
	TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);//清标志，开TWI，自动应答
	while(!(TWCR & (1<<TWINT)))
	if(WaitFunc != NULL)
	WaitFunc();//等待接收sla+w
	return ((TWSR & 0xf8)==TW_SR_SLA_ACK); //sla+W已收到，已发ACK
}

void TWI_Slave_Init(unsigned slv_addr)
{
	TWCR=0x0;//禁止TWI接口
	TWAR = slv_addr;//设置从机地址
}

void TWI_RecvStop()
{
	TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
}

void TWI_End()
{
	TWCR=(1<<TWINT);//清除标志位，禁止TWI
}
