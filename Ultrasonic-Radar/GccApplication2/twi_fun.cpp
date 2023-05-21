/*
 * twi_fun_c.c
 *
 * Created: 2023/4/22 21:39:32
 *  Author: Bonjir
 */ 
#include "twi_fun.h"

void TWI_Init(void)// TWI �ӿڵĳ�ʼ��
{//����SCL��Ƶ�ʣ�1MHz CPU-50KHz SCL,2M-100K,8M-400K
	TWSR = 0x00; //���2λΪԤ��Ƶ����(00-1,01-4,10-16,11-64)
	TWBR = 0x02; //λ�����ã�fscl=cpuƵ��/(16+2*TWBR*Ԥ��Ƶֵ)
	TWCR = (1<<TWEN); //����TWI
}

char TWI_Start(void)//����Start�źţ���ʼ����TWIͨ��
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);//����Start�ź�
	while(!(TWCR &(1<<TWINT)));//�ȴ�Start�źŷ���
	
	return (TWSR & 0xf8) == TW_START;
}

void TWI_Stop(void)//����Stop�źţ���������TWIͨ��
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//����Stop�ź�
}

char TWI_Write(unsigned char uc_data) //��TWI�ӿڷ���8λ����
{
	TWDR = uc_data;//8λ���ݴ����TWDR
	TWCR = (1<<TWINT)|(1<<TWEN);//����TWDR�е�����
	while(!(TWCR &(1<<TWINT)));//�ȴ����ݷ���
	
	return (TWSR & 0xf8)==TW_MT_SLA_ACK;
}

char TWI_Read_With_ACK(unsigned char *pdata)
{
	TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);//׼���������ݣ���ACK
	while(!(TWCR &(1<<TWINT)));//�ȴ���������
	*pdata = TWDR;
	return ((TWSR & 0xf8)==TW_SR_DATA_ACK);////�������յ����ѷ�ACK
}

char TWI_Read_With_ACK_Wait_And_Do(unsigned char *pdata, void (*WaitFunc)())
{
	TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);//׼���������ݣ���ACK
	while(!(TWCR &(1<<TWINT)))
	if(WaitFunc != NULL)
	WaitFunc();//�ȴ���������
	*pdata = TWDR;
	return ((TWSR & 0xf8)==TW_SR_DATA_ACK);////�������յ����ѷ�ACK
}

unsigned char TWI_Read_With_NACK(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN);//׼���������ݣ���NACK
	while(!(TWCR &(1<<TWINT)));//�ȴ���������
	return TWDR;//���ؽ��յ�������
}

unsigned char TWI_Get_State_Info(void)
{
	unsigned char uc_status;
	uc_status = TWSR & 0xf8;
	return uc_status;
}

char TWI_Slave_Start()
{
	TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);//���־����TWI���Զ�Ӧ��
	while(!(TWCR & (1<<TWINT)));//�ȴ�����sla+w
	return ((TWSR & 0xf8)==TW_SR_SLA_ACK); //sla+W���յ����ѷ�ACK
}

char TWI_Slave_Start_Wait_And_Do(void (*WaitFunc)())
{
	TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);//���־����TWI���Զ�Ӧ��
	while(!(TWCR & (1<<TWINT)))
	if(WaitFunc != NULL)
	WaitFunc();//�ȴ�����sla+w
	return ((TWSR & 0xf8)==TW_SR_SLA_ACK); //sla+W���յ����ѷ�ACK
}

void TWI_Slave_Init(unsigned slv_addr)
{
	TWCR=0x0;//��ֹTWI�ӿ�
	TWAR = slv_addr;//���ôӻ���ַ
}

void TWI_RecvStop()
{
	TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
}

void TWI_End()
{
	TWCR=(1<<TWINT);//�����־λ����ֹTWI
}
