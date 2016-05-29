/*
 * LP4dsm.c
 *
 * Created: 14.03.2016 21:02:43
 * Author : Ralf
 */ 
#include "LP4DSM.h"

/*
sysclock 4,194304 MHZ =4194304 Hz

		PB5	PB1
	PD4	SW1	SW2		THR+	THR-
	PD2	SW3	SW4		YAW+	YAW-
	PD7	SW5	SW6		NICK+	NICK-
	PD6	SW7	SW8		ROLL+	ROLL-
	PD0	SW9	SW10	BIND	RATE
	
	PC5	RV1		ADC5	THROTTLE
	PC4	RV2		ADC4 	YAW
	PC1	RV3		ADC1	ROLL
	PC0	RV4		ADC0	NICK
	
	PD3	Buzzer OC2B TMR2 
	PD5	LED    OC0B TMR0
	PD1	TX	   TXD
	
	PC3	MODE 1/2 SW
	
	PB3	TP1
	PB4	TP2
	
	ADC7 V-BAT/x
	
	
	PB0	 NC
	PC2	 NC
	PB2	 NC
	ADC6 NC
*/



// ************************** defines*************************************************************



//global data ****************************************************************************
#define BPM 120
#define RATIO 8
const note_t welcomeMelody[]={
	{OCT5,C5_CNT,NOTE_DURATION(RATIO,BPM)},
	{OCT5,D5_CNT,NOTE_DURATION(RATIO,BPM)},
	{OCT5,E5_CNT,NOTE_DURATION(RATIO,BPM)},
	{OCT5,F5_CNT,NOTE_DURATION(RATIO,BPM)},
	{OCT5,G5_CNT,NOTE_DURATION(RATIO,BPM)},
	{OCT5,A5_CNT,NOTE_DURATION(RATIO,BPM)},
	{OCT5,B5_CNT,NOTE_DURATION(RATIO,BPM)},
	};
#undef BPM 
#undef RATIO 
const note_t BindBeep[]={
	{OCT4,C4_CNT,100},
	{0,0,1000},
	{OCT4,E4_CNT,100},
	{0,0,1000},
	{OCT4,G4_CNT,100},
	{0,0,1000},
	{OCT5,C5_CNT,100},
	{0,0,1000},
	{OCT5,G5_CNT,100},
	{0,0,1000}
};
const note_t  LowBattery[]={
	{OCT7,E7_CNT,50},
	{0,0,2000}
};
const note_t  FlightTimerMelody[]={
	{OCT6,D6_CNT,50},
	{0,0,50},
	{OCT5,E5_CNT,50},
	{0,0,3000}
};

const Led_data_t led_data_bind[]={{200,20},{00,20}};
const Led_data_t led_data_range[]={{200,20},{00,10},{200,20},{00,150}};


//********************programm *************************************************************************+

void InitSystem(void)
{
	//enable clock prescaler change
	CLKPR=(1<<CLKPCE);
	//set clock prescaler
	CLKPR=0;
	//turn WDT off
	WDTCSR =0x00;
	//init configuration
	ConfigInit();
	//configure ADC 
	AdcInit();
	//configure O/I
	key_init();
	//configure timer 0
	InitLed();
	//configure timer 1 
	initTimer();
	//configure timer 2 
	InitBuzzer();
	//configure USART 
	tx_Init();
	//enable interrupts
	sei();

	//init user interface	
	UI_init();
	Model_Init();
	
	//wait for ADC stable
	uint8_t n=0;
	do
	{
		if(adc_new_data)
		{
			adc_new_data=0;
			n++;
		}
	} while (n<5);
}

uint8_t bind=false;		//true if bind is selected

int main(void)
{
	bool range_check=false;	//true if range check is activated
	uint32_t keys;
	uint32_t flightTimer=0;
	
	InitSystem();

	
		
//check startup config options 

	keys=Get_keys();
	//check if config is requested
	if(UI_checkforStartupGesture(keys))
		{
		SetLedStateFlashing(led_data_confiMode[0],led_data_confiMode[1]);
		}

	//check bind and button at startup
	else if(GET_KEY(keys,KEY_BIND_BTN))
		{
		bind=2;
		tx_SetBindMode(true);
		Buzz(OCT5,C5_CNT);
		SetLedStateFlashing(led_data_bind[0],led_data_bind[1]);
		}
	//check range button at startup
	else if(GET_KEY(keys,KEY_RATE_BTN))
		{
		range_check=true;
		tx_SetRangeCheck(true);
		SetLedStateCyclicSequence(sizeof(led_data_range)/sizeof(Led_data_t),led_data_range);
		buz_PlayMelody(sizeof(welcomeMelody)/sizeof(note_t),welcomeMelody);
		}
	else
		{
		SetLedStateSolid(128);
		buz_PlayMelody(sizeof(welcomeMelody)/sizeof(note_t),welcomeMelody);
		}


//main loop*********************************
		
    while (1) 
    {
	//proceed new adc readings
	if(adc_new_data)
		{
		adc_new_data=0;
		ModelCalculateOutChannelsData();
		uint16_t thr=ModelGetCannel(OUT_THROTTLE_CH);
		tx_SetChanelData(thr,OUT_THROTTLE_CH);
		for(uint8_t n=1;n<OUT_CHAN_CNT;n++)
			{
			tx_SetChanelData(ModelGetCannel(n),n);
			}

			
		tx_Send();
		flightTimer+=thr;
		}
	else
		{ //process keys
		keys=Get_keys();
		//check if mode switch is on and fast model config option is activated
		UI_CheckForConfig(keys);

		//if range check is on turn it off with raising edge mode switch key	
		if(range_check && (GET_KEY_RISING_EDGE(keys,KEY_RATE_BTN)))	
			{
			range_check=false;
			tx_SetRangeCheck(false);
			SetLedStateSolid(128);
			}
		else
			{
			//pass keys to model , model will process trim and mode				
			Model_CheckKeys(keys);
			
			//deal with bind mode
			if(bind)
				{
				if((bind==2) && !(GET_KEY(keys,KEY_BIND_BTN)))
					{
					bind=1;
					SetLedStateFlashing(led_data_bind[0],led_data_bind[1]);
					buz_PlayMelody(sizeof(BindBeep)/sizeof(note_t),BindBeep);
					}
				if(bind==1 && 0==buz_SequenceCount())
					{
					bind=0;
					tx_SetBindMode(false);
					SetLedStateSolid(128);
					buz_PlayMelody(sizeof(welcomeMelody)/sizeof(note_t),welcomeMelody);
					}
				}//end of if bind
			} //end of else range check
			
		//check battery	
		uint16_t vbat=AdcGetChanel(ADC_VREF_IDX);
		if(vbat<VOLTS(MIN_BAT_VOLTS) && 0==buz_SequenceCount())
			{
				buz_PlayMelody(sizeof(LowBattery)/sizeof(note_t),LowBattery);
			}
		
		//flight timer
		uint32_t flight_time_limit=( (uint32_t)(FLIGHT_CNT_PER_S*Config_FlightTime)  );
		if( flight_time_limit && flightTimer > flight_time_limit  &&  0==buz_SequenceCount() )
			{
				buz_PlayMelody(sizeof(FlightTimerMelody)/sizeof(note_t),FlightTimerMelody);
			}
		}//end of else adc_new data
	}
}

