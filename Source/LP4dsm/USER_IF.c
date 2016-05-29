#include "LP4DSM.h"
#include "USER_IF.h"

#define BPM 60
#define RATIO 4
const note_t confirmationMelody[]={
	{OCT4,C6_CNT,NOTE_DURATION(RATIO,BPM)},
	{OCT5,D5_CNT,NOTE_DURATION(RATIO,BPM)},
	{OCT4,C4_CNT,NOTE_DURATION(RATIO,BPM)},
};
const note_t SaveMelody[]={
	{OCT6,F6_CNT,NOTE_DURATION(RATIO,BPM)},
	{OCT5,A5_CNT,NOTE_DURATION(RATIO,BPM)},
	{OCT6,C6_CNT,NOTE_DURATION(RATIO,BPM)},
};
const note_t EntryMelody[]={
	{OCT4,C4_CNT,400},
	{0,0,400},
	{OCT5,C5_CNT,400},
};
const note_t ThreePointEntryMelody[]={
	{OCT6,F6_CNT,NOTE_DURATION(RATIO,120)},
	{0,0,NOTE_DURATION(RATIO,BPM)},
	{OCT4,F4_CNT,NOTE_DURATION(RATIO,120)},
};
const note_t ErrorMelody[]={
	{OCT4,C4_CNT,40},
	{OCT6,C6_CNT,40},
	{0,0,200},
};
note_t conf[] ={
	{OCT4,C4_CNT,NOTE_DURATION(8,60)},
	{OCT5,E4_CNT,NOTE_DURATION(8,60)},
};

extern note_t center[];
//made global to save flash memory
uint8_t l_stick_pos;
uint8_t r_stick_pos;

#undef BPM
#undef RATIO
const Led_data_t led_data_unsaved[]={{200,10},{00,10}};
const Led_data_t led_data_confiMode[]={{00,50},{200,70}};
const Led_data_t led_data_ConfigNr[]={{0,200},{255,10},{0,10},{255,10},{0,10},{255,10},{0,10},{255,10},{0,10},{255,10},{0,10},{255,10}};
	

bool tx_config_changed=false;
bool model_config_changed=false;

void UI_init(void)
{	
	tx_config_changed=false;
	model_config_changed=false;
}

//left sick is in TX_config position check position of r stick
 inline bool UI_CheckR_Stick_TX_Config()
{
				switch (r_stick_pos)
				{
					case R_STICK_TX_CONFIG_DSMX:
					//DSMX
						ConfigSetRF_Mode(DSMX);
						model_config_changed=true;
						return true;
					break;
					case R_STICK_TX_CONFIG_DSM2:
						//DSM2
						ConfigSetRF_Mode(DSM2);
						model_config_changed=true;
						return true;
					break;
					case R_STICK_TX_CONFIG_DSMF:
						//france mode
						ConfigSetRF_Mode(DSMF);
						model_config_changed=true;
						return true;
					break;
					case R_STICK_TX_CONFIG_RESET:
						//reset to factory
						ConfigResetTxConfig();
						tx_config_changed=true;
						return true;
					break;
					case R_STICK_TX_CONFIG_MODE1:
						//Stick Mode 1
						ConfigSetStic_Mode(STICK_MODE1);
						tx_config_changed=true;
						return true;
					break;
					case R_STICK_TX_CONFIG_MODE2:
						//Stick Mode 2
						ConfigSetStic_Mode(STICK_MODE2);
						tx_config_changed=true;
						return true;
					break;
					case R_STICK_TX_CONFIG_MODE3:
						//Stick Mode 3
						ConfigSetStic_Mode(STICK_MODE3);
						tx_config_changed=true;
						return true;
					break;
					case R_STICK_TX_CONFIG_MODE4:
						//Stick Mode 4
						ConfigSetStic_Mode(STICK_MODE4);
						tx_config_changed=true;
						return true;
					break;
				}//end off switch r_stick_pos
	return false;
}

//check right stick position what model Nr is selected
 bool UI_CheckR_Stick_ModelSelect()
{
	switch(r_stick_pos)
	{
		case R_STICK_MODEL_SELECT1:
			//model 1 selected
			configSet_ModelNr(0);
			tx_config_changed=true;						
			return true;
		break;
		case R_STICK_MODEL_SELECT2:
			//model 2 selected
			configSet_ModelNr(1);
			tx_config_changed=true;
			return true;
		break;
		case R_STICK_MODEL_SELECT3:
			//model 3 selected
			configSet_ModelNr(2);
			tx_config_changed=true;
			return true;
		break;
		case R_STICK_MODEL_SELECT4:
			//model 4 selected
			configSet_ModelNr(3);
			tx_config_changed=true;
			return true;
		break;
	}
	return false;
}

/*
#define CONFIG_CALIB_MIN 0
#define CONFIG_CALIB_CENT 1
#define CONFIG_CALIB_MAX 2
*/
inline  bool CalibrateSticks(void)
{
	int8_t Step=0;
	bool changed=false;
	buz_PlayMelody(sizeof(ThreePointEntryMelody)/sizeof(note_t),ThreePointEntryMelody);
	SetLedStateCyclicSequence(Step*2+2,led_data_ConfigNr);

	while (Step<3)
	{
		uint32_t keys=Get_keys();
		if(!GET_KEY(keys,KEY_MODE_SW))
		return changed;
		if(GET_KEY_RISING_EDGE(keys,CONFIG_CHECK_KEY))
		{
			for(uint8_t adc_chanel=0;adc_chanel<ADC_CHANEL_CNT;adc_chanel++)
			{
				ConfigStickCalibartion(adc_chanel,Step)=ADC_GET_CHANEL(adc_chanel);
			}
			Step++;
			changed=true;
			SetLedStateCyclicSequence(Step*2+2,led_data_ConfigNr);
			buz_PlayMelody(sizeof(confirmationMelody)/sizeof(note_t),confirmationMelody);
		}
		if(GET_KEY_RISING_EDGE(keys,CONFIG_NEXT_KEY))
		{
			Step++;
			buz_PlayMelody(sizeof(ThreePointEntryMelody)/sizeof(note_t),ThreePointEntryMelody);
			SetLedStateCyclicSequence(Step*2+2,led_data_ConfigNr);
		}
	}
	return changed;
}

//check right stick for option config position
bool UI_CheckOptionConfig()
{
	switch(r_stick_pos)
	{
		case R_STICK_OPTION_FAST_MODEL_SEL_ON:
			configSet_Tx_options(TX_OPTION_FAST_MODEL_SELECT,true);
			tx_config_changed=true;
			return true;
		break;
		case R_STICK_OPTION_FAST_MODEL_SEL_OFF:
			configSet_Tx_options(TX_OPTION_FAST_MODEL_SELECT,false);
			tx_config_changed=true;
			return true;
		break;
		case R_STICK_OPTION_CONFIG_WHILE_RUNNIG_ON:
			configSet_Tx_options(TX_OPTION_CONFIG_WHILE_RUNNIG,true);
			tx_config_changed=true;
			return true;
		break;
		case R_STICK_OPTION_CONFIG_WHILE_RUNNIG_OFF:
			configSet_Tx_options(TX_OPTION_CONFIG_WHILE_RUNNIG,false);
			tx_config_changed=true;
			return true;
		break;
		case R_STICK_CALIBRATE_INPUT_POS:
			if(CalibrateSticks())
				tx_config_changed=true;
			return true;
		break;
		case R_STICK_MODEL_CLASIC_POS:
			//Standard Model selected
			ConfigResetModelConfig(MODEL_TYPE_CLASIC);
			model_config_changed=true;
			return true;
		break;
		case R_STICK_MODEL_HELI_POS:
			//Helicopter selected
			ConfigResetModelConfig(MODEL_TYPE_HELI);
			model_config_changed=true;
			return true;
		break;
		case R_STICK_MODEL_AIRPLANE_POS:
			//airplane selected
			ConfigResetModelConfig(MODEL_TYPE_AIRPLANE);
			model_config_changed=true;
			return true;
		break;
	}
	return false;
}

bool UI_Read_N_Point(uint8_t cnt, int16_t* values)
{
	int8_t Step=0;
	bool changed=false;
	buz_PlayMelody(sizeof(ThreePointEntryMelody)/sizeof(note_t),ThreePointEntryMelody);
	SetLedStateCyclicSequence(2,led_data_ConfigNr);

	while (Step<cnt)
		{
		uint32_t keys=Get_keys();
		if(!GET_KEY(keys,KEY_MODE_SW))
			return changed;
		if(GET_KEY_RISING_EDGE(keys,CONFIG_CHECK_KEY))
			{
				values[Step]=getRawStickValue(IN_THROTTLE_CH);
				Step++;
				changed=true;
				buz_PlayMelody(sizeof(confirmationMelody)/sizeof(note_t),confirmationMelody);
				SetLedStateCyclicSequence(Step*2+2,led_data_ConfigNr);
			}
		if(GET_KEY_RISING_EDGE(keys,CONFIG_NEXT_KEY))
			{
			Step++;
			buz_PlayMelody(sizeof(ThreePointEntryMelody)/sizeof(note_t),ThreePointEntryMelody);
			SetLedStateCyclicSequence(Step*2+2,led_data_ConfigNr);
			}
		if(GET_KEY_RISING_EDGE(keys,CONFIG_VAL_P))
			{
			int16_t val=values[Step];
			val+=STICK_INC_STEP;
			if(val>CHAN_MAX_VALUE)
				{
				buz_PlayMelody(2,conf);
				val=CHAN_MAX_VALUE;
				}
			else if(val==CHAN_CENTER_VALUE) //CENTER
					{
					buz_PlayMelody(2,center);
					}
			else
				buz_PlayNote(conf[0]);
			values[Step]=val;	
			changed=true;
			}
		if(GET_KEY_RISING_EDGE(keys,CONFIG_VAL_M))
			{
			int16_t val=values[Step];
			val-=STICK_INC_STEP;
			if(val<CHAN_MIN_VALUE)
				{
				buz_PlayMelody(2,conf);
				val=CHAN_MIN_VALUE;
				}
			else if(val==CHAN_CENTER_VALUE) //CENTER
				{
				buz_PlayMelody(2,center);
				}
			else
				buz_PlayNote(conf[0]);
			values[Step]=val;
			changed=true;
			}
		}
return changed;	
}

inline bool UI_Read_Six_Point(int16_t* values)
{
	return UI_Read_N_Point(6,values);
}

inline bool UI_Read_Two_Point(int16_t* values)
{
	
	return UI_Read_N_Point(2,values);
}

 inline bool UI_CheckModelConfig()
{
	switch(r_stick_pos)
		{
		case R_STICK_CURVE1_POS:
			if(model_config.mode_type==MODEL_TYPE_HELI?UI_Read_Six_Point(ConfigCurve1(FLIGHT_MODE_STD)):UI_Read_Six_Point(ConfigAileMix(FLIGHT_MODE_STD)))
			{
				model_config_changed=true;
				return true;
			}
		break;
		case R_STICK_CURVE2_POS:
			if(model_config.mode_type==MODEL_TYPE_HELI?UI_Read_Six_Point(ConfigCurve2(FLIGHT_MODE_STD)):UI_Read_Six_Point(ConfigElevMix(FLIGHT_MODE_STD)))
			{
				model_config_changed=true;
				return true;
			}
		break;
		case R_STICK_CURVE3_POS:
			if(model_config.mode_type==MODEL_TYPE_HELI?UI_Read_Six_Point(ConfigCurve3(FLIGHT_MODE_STD)):UI_Read_Six_Point(ConfigAuxMix(FLIGHT_MODE_STD)))
			{
				model_config_changed=true;
				return true;
			}
		break;
		case R_STICK_EXP_POS:
			if(UI_Read_Six_Point(&(ConfigExpo(FLIGHT_MODE_STD,0))))
				{
				model_config_changed=true;
				return true;
				}
		break;
		case R_STICK_RATE_POS:
			if(UI_Read_Six_Point(&(ConfigRate(FLIGHT_MODE_STD,0))))
				{
				model_config_changed=true;
				return true;
				}
		break;
		case R_STICK_GYRO_POS:
		if(UI_Read_Two_Point(&(ConfigGyroGain(FLIGHT_MODE_STD))))
			{
			model_config_changed=true;
			return true;
			}
		break;
		case R_STICK_11MS_POS:
			model_config_changed=true;
			configSet_Rx_options(RX_OPTION_11MS,true);
			TimerSetUpdateIntervall();
			return true;
		break;
		case R_STICK_22MS_POS:
			model_config_changed=true;
			configSet_Rx_options(RX_OPTION_11MS,false);
			TimerSetUpdateIntervall();
		return true;
		break;
		}
	return false;
}

		

bool UI_CheckGesture()
{
	switch (l_stick_pos)
		{
		case L_STICK_TX_CONFIG_POS:
			return UI_CheckR_Stick_TX_Config();
		break;
		case L_STICK_MODEL_SELECT_POS:
			return UI_CheckR_Stick_ModelSelect();
		break;
		case L_STICK_OPTION_CONFIG_POS:
			return UI_CheckOptionConfig();
		break;
		case L_STICK_MODEL_CONFIG_POS:
		return UI_CheckModelConfig();
		break;
		} 
	return false;
}
#ifdef DEBUG
	uint8_t debug_l_Stick=0;
	uint8_t debug_r_Stick=0;
#endif
 void ReadSticks()
{
		//read sticks
		uint16_t left_vertical_ch=ADC_GET_CHANEL(IN_LEFT_VERT_CH);
		uint16_t left_horizontal_ch=ADC_GET_CHANEL(IN_LEFT_HRIZ_CH);
		uint16_t right_vertical_ch=ADC_GET_CHANEL(IN_RIGHT_VERT_CH);
		uint16_t right_horizontal_ch=ADC_GET_CHANEL(IN_RIGHT_HRIZ_CH);
		//get stick position
		l_stick_pos=STICK_EVAL(left_horizontal_ch,left_vertical_ch);
		r_stick_pos=STICK_EVAL(right_horizontal_ch,right_vertical_ch);
#ifdef DEBUG
		l_stick_pos=debug_l_Stick;
		r_stick_pos=debug_r_Stick;
#endif
	
}
bool UI_ProceedConfigMode2(uint16_t keys)
{
	uint32_t tick=sys_tick;
	bool done=false;
	//check for trim keys down
	while (!done && (
		keys==(1<<KEY_LEFT_VERT_TRIM_P)||
		keys==(1<<KEY_LEFT_VERT_TRIM_M)||
		keys==(1<<KEY_RIGHT_VERT_TRIM_P)||
		keys==(1<<KEY_RIGHT_VERT_TRIM_M)||				
		keys==(1<<KEY_LEFT_HRIZ_TRIM_P)||
		keys==(1<<KEY_LEFT_HRIZ_TRIM_M)||
		keys==(1<<KEY_RIGHT_HRIZ_TRIM_P)||
		keys==(1<<KEY_RIGHT_HRIZ_TRIM_M)
		))
		{	
			if((sys_tick-tick)>1000)
			{
				switch (keys)
				{
					case (1<<KEY_LEFT_VERT_TRIM_M):
						configSet_Rx_options(IN_LEFT_VERT_CH,true);
						done=true;
					break;
					case (1<<KEY_LEFT_VERT_TRIM_P):
						configSet_Rx_options(IN_LEFT_VERT_CH,false);
						done=true;
					break;
					case (1<<KEY_RIGHT_VERT_TRIM_M):
						configSet_Rx_options(IN_RIGHT_VERT_CH,true);
						done=true;
					break;
					case (1<<KEY_RIGHT_VERT_TRIM_P):
						configSet_Rx_options(IN_RIGHT_VERT_CH,false);
						done=true;
					break;
					case (1<<KEY_LEFT_HRIZ_TRIM_M):
						configSet_Rx_options(IN_LEFT_HRIZ_CH,true);
						done=true;
					break;
					case (1<<KEY_LEFT_HRIZ_TRIM_P):
						configSet_Rx_options(IN_LEFT_HRIZ_CH,false);
						done=true;
					break;
					case (1<<KEY_RIGHT_HRIZ_TRIM_M):
						configSet_Rx_options(IN_RIGHT_HRIZ_CH,true);
						done=true;
					break;
					case (1<<KEY_RIGHT_HRIZ_TRIM_P):
						configSet_Rx_options(IN_RIGHT_HRIZ_CH,false);
						done=true;
					break;
				}
			}
			keys=Get_keys();
		}
	if(done)
		{
		buz_PlayMelody(sizeof(confirmationMelody)/sizeof(note_t),confirmationMelody);
		ConfigSaveTxConfig();
		}
	
	ReadSticks();
	if(CONFIG_GET_TX_OPTIONS(TX_OPTION_FAST_MODEL_SELECT))
		{
		if(l_stick_pos==L_STICK_MODEL_SELECT_POS)
			{
			if(UI_CheckR_Stick_ModelSelect())
				{
				buz_PlayMelody(sizeof(SaveMelody)/sizeof(note_t),SaveMelody);
				ConfigSaveTxConfig();
				while(buz_SequenceCount()){};
				return true;
				}
			} 
		}
return false;
}

void UI_CheckForConfig(uint16_t keys)
{
	if((GET_KEY(keys,KEY_MODE_SW)) && (CONFIG_GET_TX_OPTIONS(TX_OPTION_CONFIG_WHILE_RUNNIG)))
	{	 
		if (getRawStickValue(IN_THROTTLE_CH)<(CHAN_CENTER_VALUE-(75L*CHAN_RANGE)/100))
		{
			if(UI_ProceedConfigMode())
			{
				SetLedStateFlashing(led_data_confiMode[0],led_data_confiMode[1]);
			}
			else
			{
				SetLedStateSolid(128);
			}
		}
		else //thr is not down
		{
			if(0==buz_SequenceCount())
				buz_PlayMelody(sizeof(ErrorMelody)/sizeof(note_t),ErrorMelody);
		}
	}
}



//mode switch is on we are in config mode
//proceed user input
bool UI_ProceedConfigMode(void)
{
	uint32_t keys=Get_keys();
	//switches
	bool check=true;
	bool changed=tx_config_changed || model_config_changed;

	SetLedStateFlashing(led_data_confiMode[0],led_data_confiMode[1]);

	while(GET_KEY(keys,KEY_MODE_SW))
		{
		keys=Get_keys();
		//get stick position
		ReadSticks();
		
		if(check)
			{ //check for stick position	
			check=false;
			if(UI_CheckGesture())
				{
				changed=true;	
				buz_PlayMelody(sizeof(confirmationMelody)/sizeof(note_t),confirmationMelody);
				SetLedStateFlashing(led_data_unsaved[0],led_data_unsaved[1]);
				}
			else
				{
				buz_PlayMelody(sizeof(EntryMelody)/sizeof(note_t),EntryMelody);
				SetLedStateFlashing(led_data_confiMode[0],led_data_confiMode[1]);
				while(buz_SequenceCount()){};
				}
			}
		else if(changed && GET_KEY(keys,CONFIG_SAVE_KEY))
			{
			buz_PlayMelody(sizeof(SaveMelody)/sizeof(note_t),SaveMelody);
			changed=false;
			if(tx_config_changed)
				{
				ConfigSaveTxConfig();
				tx_config_changed=false;
				}
			if(model_config_changed)
				{
				ConfigSaveModelConfig();
				model_config_changed=false;
				}
			SetLedStateFlashing(led_data_confiMode[0],led_data_confiMode[1]);
			}	
		if(GET_KEY_RISING_EDGE(keys,CONFIG_CHECK_KEY))		
			{
			check=true;
			}
		if(GET_KEY_RISING_EDGE(keys,CONFIG_FLIGHT_TIME_P))
			{
			int16_t time=Config_FlightTime+FLIGHT_TIME_STEP;
			if(time>255)
				{
				buz_PlayMelody(2,conf);
				time=255;
				}
			else
				buz_PlayNote(conf[0]);
			Config_FlightTime=time;	
			model_config_changed=true;
			changed=true;
			}
		if(GET_KEY_RISING_EDGE(keys,CONFIG_FLIGHT_TIME_M))
			{
			int16_t time=Config_FlightTime-FLIGHT_TIME_STEP;
			if(time<0)
				{
				buz_PlayMelody(2,conf);	
				time=0;
				}
			else
			buz_PlayNote(conf[1]);
			Config_FlightTime=time;
			model_config_changed=true;
			changed=true;
			}
			
		}
	return changed;	
}

bool UI_checkforStartupGesture(uint16_t keys)
{
	
	if(GET_KEY(keys,KEY_MODE_SW))
		{
		return UI_ProceedConfigMode();
		}
	return UI_ProceedConfigMode2(keys); //config options that can be set without mode SW on;
}
