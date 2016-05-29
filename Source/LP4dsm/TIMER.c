#include "LP4DSM.h"
#include "TIMER.h"

#define ADC_UPDATE_INTERVAL adc_update_intervall


//global data ****************************************************************************

volatile uint32_t sys_tick;
int16_t tic_error;
uint16_t adc_update_intervall;
uint8_t send_idx;

void initTimer(void)
{
		//init data
		sys_tick=0;
		tic_error=0;
		TimerSetUpdateIntervall();
		//setup the timer
		TCCR1B=(1<<CS10); //No prescaling
		TCCR1A=0; //normal operation
		OCR1B=ADC_UPDATE_INTERVAL; //Compare match B will trigger ADC convertion
		OCR1A=CNT_PER_TIC; //systick
		TIMSK1=(1<<OCIE1B)|(1<<OCIE1A); //Output Compare A & B Match Interrupt Enable

}

void TimerSetUpdateIntervall(void)
{
		adc_update_intervall=CONFIG_GET_RX_OPTIONS(RX_OPTION_11MS)?ADC_UPDATE_INTERVAL11MS:ADC_UPDATE_INTERVAL22MS;
		send_idx=CONFIG_GET_RX_OPTIONS(RX_OPTION_11MS)?21:9;			
}
//called to trigger ADC every ADC_UPDATE_INTERVAL , just update OCR1B
//for low jitter need to send at a fixed time
//give main app enought time for calculations
ISR(TIMER1_COMPB_vect)
{
	OCR1B+=ADC_UPDATE_INTERVAL;
	if(adc_tick==send_idx && tx_active)
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


