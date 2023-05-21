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
	i_cnt = 2;//统计Echo的初值=检测到echo信号+循环的判断约2个CPU时钟周期
	while((PINB&(1<<PINB2))!=0)//继续统计ECHO信号高电平的持续时间
	i_cnt++;//每次=数据加载+&运算+判断+循环+加约5个时钟周期（默认1MHz）
	/*若之前禁用了中断，这里可以开中断了 sei();*/
	return(i_cnt*5.0/100.0*17.0 / 16);//返回距离，单位mm
}

#endif