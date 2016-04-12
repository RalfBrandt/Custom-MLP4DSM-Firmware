#include "LP4DSM.h"
#include "LED.h"

//global data
Led_data_t Led_data[LED_DATA_CNT];
uint8_t led_idx;
uint8_t led_ptr;
uint16_t led_time;
uint8_t led_reload;


//add entry to buffer
void AddLedSequence(Led_data_t data)
{
	Led_data_t* led_data=&Led_data[led_ptr];
	led_ptr=(led_ptr+1)%LED_DATA_CNT;
	led_data->value=data.value;
	led_data->duration=data.duration;
}


void SetLedStateFlashing(Led_data_t on,Led_data_t off)
{
	led_reload=led_ptr;
	AddLedSequence(on);
	AddLedSequence(off);
}

//turn off the led from any sequence
inline void TurnLedOff(void)
{
	SetLedVal(0);
	led_time=0;
	led_idx=0;
	led_ptr=0;
	led_reload=0;
}


void SetLedStateSolid(uint8_t value)
{
	TurnLedOff();
	SetLedVal(value);
}

void SetLedStateSingelSequence(uint8_t cnt,const Led_data_t * data)
{
	led_reload=(led_ptr+cnt)%LED_DATA_CNT;
	for(uint8_t n=0;n<cnt;n++)
	{
		AddLedSequence(data[n]);
	}
}

void SetLedStateCyclicSequence(uint8_t cnt,const Led_data_t * data)
{
	led_reload=led_ptr;
	for(uint8_t n=0;n<cnt;n++)
	{
		AddLedSequence(data[n]);
	}
}


//initialize Timer 0 for LED PWM
 void InitLed(void)
{
	TCCR0A=(1<<COM0B0)|(1<<COM0B1)|(1<<WGM00)|(1<<WGM01);//inverted  Fast PWM Mode
	TCCR0B= (1<<CS00)|(1<<CS01); //prescaler = 64
	DDRD|=(1<<PORTD5);
	TurnLedOff();
}


//return number of elements in buffer
uint8_t LedSequenceCount(void)
{
	return (led_ptr-led_idx+LED_DATA_CNT)%LED_DATA_CNT;
}

void SetLedVal(uint8_t value)
{
	OCR0B=value;
}

//called from systic
void LedCyclic(void)
{
	if(led_time) //led sequence is on
	{
		led_time--;
		if(!led_time && led_ptr==led_idx && led_reload!=led_idx)
			{
			led_idx=led_reload;
			}
	}
	else
	{
			if(led_ptr!=led_idx) //load next step in sequence
			{
				SetLedVal(Led_data[led_idx].value);
				led_time=10*Led_data[led_idx].duration;
				led_idx=(led_idx+1)%LED_DATA_CNT;
			}

	}
	
}