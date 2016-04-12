#include "LP4DSM.h"
#include "TIMER.h"



//global data ****************************************************************************

volatile uint32_t sys_tick;
int16_t tic_error;

void initTimer(void)
{
		//init data
		sys_tick=0;
		tic_error=0;
		//setup the timer
		TCCR1B=(1<<CS10); //No prescaling
		TCCR1A=0; //normal operation
		OCR1B=ADC_UPDATE_INTERVAL; //Compare match B will trigger ADC convertion
		OCR1A=CNT_PER_TIC; //systick
		TIMSK1=(1<<OCIE1B)|(1<<OCIE1A); //Output Compare A & B Match Interrupt Enable

}

//called to trigger ADC every ADC_UPDATE_INTERVAL , just update OCR1B
ISR(TIMER1_COMPB_vect)
{
	OCR1B+=ADC_UPDATE_INTERVAL;
	if(adc_checkSend() && tx_active)
	{
		SendTX_Buf();
	} 

	key_cyclic(); //scan keys while ADC is working
}

//called every tic interval (1ms)
ISR(TIMER1_COMPA_vect)
{
	sys_tick++;
	tic_error+=TIC_ERROR;
	if(tic_error>=TICK_RATE)  //with the given crystal we are a bit off 1000us
	{
		OCR1A+=(CNT_PER_TIC-1); //compensate error
		tic_error-=TICK_RATE; //set next trigger
	}
	else
	{
		OCR1A+=CNT_PER_TIC; //set next trigger
	}
	BuzzerCyclic();
	LedCyclic();
}


