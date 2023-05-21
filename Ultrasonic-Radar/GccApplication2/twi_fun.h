
#ifndef TWI_FUN_H_
#define TWI_FUN_H_

#include <util/twi.h>//����Դ�TWI�ӿڵļĴ�����ͷ�ļ�

#ifndef NULL
#define NULL 0
#endif

void TWI_Init(void); // TWI �ӿڵĳ�ʼ��
char TWI_Start(void); //����Start�źţ���ʼ����TWIͨ��
void TWI_Stop(void); //����Stop�źţ���������TWIͨ��
void TWI_End(); //�����־λ����ֹTWI
char TWI_Write(unsigned char uc_data); //��TWI�ӿڷ���8λ����
char TWI_Read_With_ACK(unsigned char *pdata);
char TWI_Read_With_ACK_Wait_And_Do(unsigned char *pdata, void (*WaitFunc)());
unsigned char TWI_Read_With_NACK(void);
unsigned char TWI_Get_State_Info(void);

void TWI_Slave_Init(unsigned slv_addr);
char TWI_Slave_Start();
char TWI_Slave_Start_Wait_And_Do(void (*WaitFunc)());
void TWI_RecvStop();


#endif