#include "LP4DSM.h"
#include "STICKS.h"

//global data
const uint8_t mode_ch_map[STICK_MODE_CNT][ADC_CHANEL_CNT]={{MODE1_CH1,MODE1_CH2,MODE1_CH3,MODE1_CH4},{MODE2_CH1,MODE2_CH2,MODE2_CH3,MODE2_CH4},{MODE3_CH1,MODE3_CH2,MODE3_CH3,MODE3_CH4},{MODE4_CH1,MODE4_CH2,MODE4_CH3,MODE4_CH4}};
 note_t confirm[]={
	{OCT6,FS6_CNT,NOTE_DURATION(8,60)},
	{OCT4,C4_CNT,NOTE_DURATION(8,60)},
};
note_t center[]={
	{OCT6,FS6_CNT,NOTE_DURATION(2,60)},
	{OCT4,F4_CNT,NOTE_DURATION(2,60)},
};


uint8_t GetAadChannelIndex(uint8_t Nr)
{
	return mode_ch_map[ConfigStic_Mode][Nr];
}

int16_t getRawStickValue(uint8_t Nr)
{
	int16_t out_val;
	
	uint8_t adc_chanel=GetAadChannelIndex(Nr);
	int16_t value=ADC_GET_CHANEL(adc_chanel);
	int16_t center_in=ConfigStickCalibartion(adc_chanel,CONFIG_CALIB_CENT);
	//apply calibration
	if(value<=center_in)
	{
		out_val=CHAN_MIN_VALUE+ScaleHalf(ConfigStickCalibartion(adc_chanel,CONFIG_CALIB_MIN),center_in,value);
//		out_val=Scale(ConfigStickCalibartion(adc_chanel,CONFIG_CALIB_MIN),center_in,CHAN_MIN_VALUE,CHAN_CENTER_VALUE,value);
	}
	else
	{
		out_val=CHAN_CENTER_VALUE+ScaleHalf(center_in,ConfigStickCalibartion(adc_chanel,CONFIG_CALIB_MAX),value);
//		out_val=Scale(center_in,ConfigStickCalibartion(adc_chanel,CONFIG_CALIB_MAX),CHAN_CENTER_VALUE,CHAN_MAX_VALUE,value);
	}
	return out_val;

}
int16_t getStickValue(uint8_t Nr)
{
	int16_t out_val;
	int16_t center_in;
	
	uint8_t adc_chanel=GetAadChannelIndex(Nr);
	int16_t value=getRawStickValue(Nr);
	center_in=CHAN_CENTER_VALUE-((int16_t)ConfigTrim(adc_chanel)*STICK_TRIM_TIC_CNT);
	//apply trim
	if(value<=center_in)
	{
		out_val=CHAN_MIN_VALUE+ScaleHalf(CHAN_MIN_VALUE,center_in,value);
//		out_val=Scale(CHAN_MIN_VALUE,center_in,CHAN_MIN_VALUE,CHAN_CENTER_VALUE,value);
	}
	else
	{
		out_val=CHAN_CENTER_VALUE+ScaleHalf(center_in,CHAN_MAX_VALUE,value);
//		out_val=Scale(center_in,CHAN_MAX_VALUE,CHAN_CENTER_VALUE,CHAN_MAX_VALUE,value);
	}
	if(CONFIG_GET_TX_OPTIONS(adc_chanel))
		out_val=-out_val;
	return out_val;
}

void StickTrimUp(uint8_t adc_chanel)
{
	uint8_t n=adc_chanel;
	ConfigTrim(n)++;
	confirm[0].cnt=FS6_CNT+ConfigTrim(n)*2; //vary note based on trim value
	if(ConfigTrim(n)>STCIK_MAX_TRIM) //end of range
		{
		ConfigTrim(n)=STCIK_MAX_TRIM;
		buz_PlayMelody(2,confirm);
		}
	if(ConfigTrim(n)==0) //CENTER
		{
		buz_PlayMelody(2,center);
		}
	else
		{
		buz_PlayNote(confirm[0]);
		}
	ConfigSaveTrim();
}

void StickTrimDown(uint8_t adc_chanel)
{
	uint8_t n=adc_chanel;
	ConfigTrim(n)--;
	confirm[0].cnt=FS6_CNT+ConfigTrim(n)*2; //vary note based on trim value
	if(ConfigTrim(n)<-STCIK_MAX_TRIM) //end of range
	{
		ConfigTrim(n)=-STCIK_MAX_TRIM;
		buz_PlayMelody(2,(const note_t *)&confirm);
	}
	if(ConfigTrim(n)==0) //CENTER
	{
		buz_PlayMelody(2,(const note_t *)&center);
	}
	else
	{
		buz_PlayNote(confirm[0]);
	}
	ConfigSaveTrim();
}


