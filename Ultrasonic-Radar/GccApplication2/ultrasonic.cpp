/*
 * ultrasonic.cpp
 *
 * Created: 2023/5/6 21:21:19
 *  Author: Bonjir
 */ 

#include "ultrasonic.h"

ULTRASONIC::ULTRASONIC()
{
	this->pEcho = this->pTrig = PINS_ERR;
}

void ULTRASONIC::Attach(uint8_t pEcho, uint8_t pTrig)
{
	this->pEcho = pEcho;
	this->pTrig = pTrig;
	SetPinMode(pEcho, INPUT);
	SetPinMode(pTrig, OUTPUT);
}

uint16_t ULTRASONIC::GetDistance()
{
	if(!IsPinValid(this->pEcho) || !IsPinValid(this->pTrig))
		return 0;
		
	
	uint16_t overtime_cnt = 0;
	
	DigitalWrite(this->pTrig, 1); // start signal
	_delay_us(20);
	DigitalWrite(this->pTrig, 0);
	
	while(DigitalRead(this->pEcho) == 0) // wait while no echo
	{
		overtime_cnt ++;
		if(overtime_cnt > 9000)
			break;
	}
	
	uint32_t echo_start = 0;
	
	if(IsSysClockOn() == 0)
	{
		/*DigitalWrite(PINS_C0, 1);
		_delay_ms(1000);
		DigitalWrite(PINS_C0, 0);
		_delay_ms(1000);*/
		SetSysClock(8);
	}
	else echo_start = GetSysClock();
	
	while(DigitalRead(this->pEcho) == 1); // wait while echo is high
	
	uint32_t echo_delay = GetSysClock() - echo_start;
	SetSysClock(0); // turn off timer 0
	
	uint16_t ans = echo_delay / 100.0 * 17.2; 
	
	return ans ; 

/***********************************************************************
	// now since the MCU has a clock of 16MHz, the second method has no necessity to exist, because the former method has little error with the high frequency

	if(ans >= 250)
		return ans;
	
	
	// if the echo_delay is too small, then the timer0 will be delayed to reply because requests are too frequent, leading to a larger answer than reality
	// therefore we tend to use the plain "count method" to calculate time when having a small echo_delay
	// the GetSysClock method performs better and more stable than the "count method" when echo_delay is large, 
	// partly because the count method may be affected or interrupted to some unknown reasons, even though the timers and interruptions are shut down
	cli();
	
	overtime_cnt = 0;
	uint16_t clk_cnt = 1;
	uint8_t pEcho_group = GETPIN_GROUP(this->pEcho), pEcho_number_shifted = GETPIN_NUMBERSHIFTED(this->pEcho);
	
	DigitalWrite(this->pTrig, 1); // start signal
	_delay_us(20);
	DigitalWrite(this->pTrig, 0);
	
	switch(pEcho_group)
	{
		case 0xB:
		{
			while((PINB & pEcho_number_shifted) == 0) // wait while no echo
			{
				overtime_cnt ++;
				if(overtime_cnt > 9000)
				break;
			}
	
			while((PINB & pEcho_number_shifted) != 0)
				clk_cnt++;
			break;
		}
		case 0xC:
		{
			while((PINC & pEcho_number_shifted) == 0)
			{
				overtime_cnt ++;
				if(overtime_cnt > 9000)
				break;
			}
			
			while((PINC & pEcho_number_shifted) != 0)
				clk_cnt++;
			break;
		}
		case 0xD:
		{
			while((PIND & pEcho_number_shifted) == 0)
			{
				overtime_cnt ++;
				if(overtime_cnt > 9000)
				break;
			}
			
			while((PIND & pEcho_number_shifted) != 0)
				clk_cnt++;
			break;
		}
		default: return 0;
	}
	
	sei();
	
	ans = clk_cnt * 6.0 * 17.2 / 100.0 / 16;
	
	return ans;
	
	***********************************************************************/
}

#if 0

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
	i_cnt = 2;//ͳ��Echo�ĳ�ֵ=��⵽echo�ź�+ѭ�����ж�Լ2��CPUʱ������
	while((PINB&(1<<PINB2))!=0)//����ͳ��ECHO�źŸߵ�ƽ�ĳ���ʱ��
	i_cnt++;//ÿ��=���ݼ���+&����+�ж�+ѭ��+��Լ5��ʱ�����ڣ�Ĭ��1MHz��
	/*��֮ǰ�������жϣ�������Կ��ж��� sei();*/
	return(i_cnt*5.0/100.0*17.0 / 16);//���ؾ��룬��λmm
}

#endif