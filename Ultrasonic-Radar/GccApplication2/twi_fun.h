
#ifndef TWI_FUN_H_
#define TWI_FUN_H_

#include <util/twi.h>//软件自带TWI接口的寄存器等头文件

#ifndef NULL
#define NULL 0
#endif

void TWI_Init(void); // TWI 接口的初始化
char TWI_Start(void); //发送Start信号，开始本次TWI通信
void TWI_Stop(void); //发送Stop信号，结束本次TWI通信
void TWI_End(); //清除标志位，禁止TWI
char TWI_Write(unsigned char uc_data); //向TWI接口发送8位数据
char TWI_Read_With_ACK(unsigned char *pdata);
char TWI_Read_With_ACK_Wait_And_Do(unsigned char *pdata, void (*WaitFunc)());
unsigned char TWI_Read_With_NACK(void);
unsigned char TWI_Get_State_Info(void);

void TWI_Slave_Init(unsigned slv_addr);
char TWI_Slave_Start();
char TWI_Slave_Start_Wait_And_Do(void (*WaitFunc)());
void TWI_RecvStop();


#endif