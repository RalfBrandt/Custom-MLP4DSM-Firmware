#include "LP4DSM.h"
#include "analog.h"

//global data ****************************************************************************
uint16_t adc_data[OVERSAMPLE_CNT+1][ADC_CHANEL_CNT];	//the data captured by ADC plus 1 from las cycle for derivation
uint8_t adc_ch_idx; //ADC channel index
uint8_t adc_samp_idx; //the sample index (oversampling)
int16_t adc_avg[ADC_OUT_CHANEL_CNT]; //the averaged data from ADC
volatile bool adc_new_data;
const uint8_t ch_map[ADC_CHANEL_CNT]={IN_RV1_CH,IN_RV2_CH,IN_RV4_CH,IN_RV3_CH};
uint8_t adc_tick;

//initialize the adc and related data
void AdcInit(void)
{
	//init ADC buffer
	for(uint8_t ch=0;ch<ADC_CHANEL_CNT;ch++)
	{
		for(uint8_t n=0;n<=OVERSAMPLE_CNT;n++)
		{
			adc_data[n][ch]=(1<<(ADC_BITS-1));
		}
	}
	//set index to start position
	adc_ch_idx=0;
	adc_samp_idx=0;
	adc_new_data=0;
	adc_tick=0;
	//setup the ADC
	DIDR0=(1<<ADC0D)|(1<<ADC1D)|(1<<ADC4D)|(1<<ADC5D); //disable digital input on ADC channels
	ADMUX=(1<<REFS0)|ch_map[adc_ch_idx]; //AVCC as ref. right aligned
	ADCSRA=(1<<ADEN)|(1<<ADATE)|(1<<ADIE)|(1<<ADPS0)|(1<<ADPS2); //adc enable, auto trigger enable, interrupt enable; prescaler=32
	ADCSRB=(1<<ADTS0)|(1<<ADTS2); //auto trigger on Timer1 Compare Match B
}

#ifdef SEND_ON_IRQ
bool adc_checkSend(void) //this gives the main program 8.02 ms to do the calculations
{
	return (adc_ch_idx==3 && (adc_samp_idx==5));
}
#endif

#ifdef DEBUG
uint16_t adc_debug_val=ADC_CENTER;
#endif

//get value of a given channel
int16_t AdcGetChanel(uint8_t ch)
{
//	return 	adc_avg[ch];
	if(ch<ADC_OUT_CHANEL_CNT)
	{
#ifdef DEBUG
		return adc_debug_val;
#else
		return 	adc_avg[ch];
#endif
	}
	return 0;
}

//calculate average off adc reading (oversampling)
void AdcCalc_avg(void)
{
	for(uint8_t ch=0;ch<ADC_CHANEL_CNT;ch++)
	{
		int16_t adc_sum=0;
		int16_t deltaSum=0;
		for(unsigned char n=1;n<=OVERSAMPLE_CNT;n++)
		{
			adc_sum+=adc_data[n][ch];
			deltaSum+=(int16_t)adc_data[n][ch]-(int16_t)adc_data[n-1][ch];
		}
		adc_sum>>=3;// /=(OVERSAMPLE_CNT);
		deltaSum>>=2;// /=(OVERSAMPLE_CNT/2);
		adc_sum+=+deltaSum; //add some derivative to compensate for the  low pass of the average
		if(adc_sum<ADC_MIN)
			adc_sum=ADC_MIN;
		if (adc_sum>ADC_MAX)
			adc_sum=ADC_MAX;
		adc_avg[ch]=adc_sum; //store result
		//copy resent to beginning of next cycle
		adc_data[0][ch]=adc_data[OVERSAMPLE_CNT][ch];
	}
}

// Interrupt subroutine for ADC conversion complete , called when ADC conversion is done
ISR(ADC_vect)
{
	adc_tick++;
	if(adc_samp_idx==OVERSAMPLE_CNT)
	{ //special case VBAT
		adc_avg[ADC_CHANEL_CNT]=ADC;
	}
	else
	{	//store measurements ín arry
		adc_data[adc_samp_idx+1][adc_ch_idx]=ADC;
	}

	//update index
	adc_ch_idx++;
	if(adc_ch_idx==ADC_CHANEL_CNT)
	{
		adc_ch_idx=0;
		adc_samp_idx++;
	}
	if(adc_samp_idx==OVERSAMPLE_CNT)
	{  //we are done with regular channel
		if(0==adc_ch_idx)
		{ //add one measurement for VBAT
		ADMUX=IN_VREF_CH|(ADMUX&0xF0);
		AdcCalc_avg();
		adc_new_data=1;
		return;
		}
		//restart
		adc_samp_idx=0;
		adc_ch_idx=0;
		adc_tick=0;
	}
	//select next channel
	ADMUX=ch_map[adc_ch_idx]|(ADMUX&0xF0);
}
