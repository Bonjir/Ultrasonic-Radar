#define F_CPU 16000000UL

#include <avr/io.h>
#include "twi_lcd.h"
#include "ultrasonic.h"


uint16_t HCSR04_Run(void);

int main()
{
	ULTRASONIC ultrasonic;
	ultrasonic.Attach(PINS_B2, PINS_B1);
	
	unsigned int distance=0, dist2 = 0;
	unsigned char i=15,uc_d=0;
	TWI_Init(); 
	LCD_Init();
	while (1)
	{ 
		distance = ultrasonic.GetDistance();
		dist2 = HCSR04_Run();
		
		LCD_Write_String(0,0,"Distance:");
		LCD_Write_String(1,14,"mm");
		
		// timer method
		i=12; //从最低位开始转换和显示，最多7位
		if(distance == 0)
		{
			LCD_Write_Char(1,i,'0');
		}
		while(distance>0)
		{ 
			uc_d = distance % 10+0x30;
			LCD_Write_Char(1,i,uc_d);
			i--; 
			distance /=10;
		}
		while(i>0) //高位没有数字时不显示
		{ 
			LCD_Write_Char(1,i,' ');
			i--; 
		}
		
		
		// count method
		i=6; //从最低位开始转换和显示，最多7位
		while(dist2>0)
		{
			uc_d = dist2 % 10+0x30;
			LCD_Write_Char(1,i,uc_d);
			i--;
			dist2 /=10;
		}
		while(i>0) //高位没有数字时不显示
		{
			LCD_Write_Char(1,i,' ');
			i--;
		}
		_delay_ms(500);//
	}
}


uint16_t HCSR04_Run(void)
{
	uint16_t i_cnt=0;//统计echo的高电平(固定时钟下的次数)
	uint16_t uc_cnt=1;//超时计数（统计在一定时间内没有响应）
	/*为了测量的准确，如系统中使用了中断，这里要禁止中断 cli(); */
	DDRB |= (1<<DDRB1);//PB1为输出（到HC_SR04的Trig管脚）
	DDRB &= ~(1<<DDRB2);//PB2为输入（来自HC_SR04的Echo管脚）
	PORTB |=(1<<PORTB1);//PB1输出高电平到Trig告诉HC_SR04准备发送超声波
	_delay_us(20);//持续20us(>10us)
	PORTB &=~(1<<PORTB1);//PB1输出低电平到Trig结束通知
	while((PINB&(1<<PINB2))==0) //等待Echo为高电平(即收到超声回波)
	{
		uc_cnt++;
		if(uc_cnt>9000)
		break;//计数溢出：超时，跳过
	}
	//i_cnt = 2;//统计Echo的初值=检测到echo信号+循环的判断约2个CPU时钟周期
	while((PINB&(1<<PINB2))!=0)//继续统计ECHO信号高电平的持续时间
	i_cnt++;//每次=数据加载+&运算+判断+循环+加约5个时钟周期（默认1MHz）
	/*若之前禁用了中断，这里可以开中断了 sei();*/
	return(i_cnt*6.0/100.0*17.0 / 16);//返回距离，单位mm
}
