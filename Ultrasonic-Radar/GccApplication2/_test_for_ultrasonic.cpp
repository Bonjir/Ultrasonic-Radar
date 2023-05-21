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
		i=12; //�����λ��ʼת������ʾ�����7λ
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
		while(i>0) //��λû������ʱ����ʾ
		{ 
			LCD_Write_Char(1,i,' ');
			i--; 
		}
		
		
		// count method
		i=6; //�����λ��ʼת������ʾ�����7λ
		while(dist2>0)
		{
			uc_d = dist2 % 10+0x30;
			LCD_Write_Char(1,i,uc_d);
			i--;
			dist2 /=10;
		}
		while(i>0) //��λû������ʱ����ʾ
		{
			LCD_Write_Char(1,i,' ');
			i--;
		}
		_delay_ms(500);//
	}
}


uint16_t HCSR04_Run(void)
{
	uint16_t i_cnt=0;//ͳ��echo�ĸߵ�ƽ(�̶�ʱ���µĴ���)
	uint16_t uc_cnt=1;//��ʱ������ͳ����һ��ʱ����û����Ӧ��
	/*Ϊ�˲�����׼ȷ����ϵͳ��ʹ�����жϣ�����Ҫ��ֹ�ж� cli(); */
	DDRB |= (1<<DDRB1);//PB1Ϊ�������HC_SR04��Trig�ܽţ�
	DDRB &= ~(1<<DDRB2);//PB2Ϊ���루����HC_SR04��Echo�ܽţ�
	PORTB |=(1<<PORTB1);//PB1����ߵ�ƽ��Trig����HC_SR04׼�����ͳ�����
	_delay_us(20);//����20us(>10us)
	PORTB &=~(1<<PORTB1);//PB1����͵�ƽ��Trig����֪ͨ
	while((PINB&(1<<PINB2))==0) //�ȴ�EchoΪ�ߵ�ƽ(���յ������ز�)
	{
		uc_cnt++;
		if(uc_cnt>9000)
		break;//�����������ʱ������
	}
	//i_cnt = 2;//ͳ��Echo�ĳ�ֵ=��⵽echo�ź�+ѭ�����ж�Լ2��CPUʱ������
	while((PINB&(1<<PINB2))!=0)//����ͳ��ECHO�źŸߵ�ƽ�ĳ���ʱ��
	i_cnt++;//ÿ��=���ݼ���+&����+�ж�+ѭ��+��Լ5��ʱ�����ڣ�Ĭ��1MHz��
	/*��֮ǰ�������жϣ�������Կ��ж��� sei();*/
	return(i_cnt*6.0/100.0*17.0 / 16);//���ؾ��룬��λmm
}
