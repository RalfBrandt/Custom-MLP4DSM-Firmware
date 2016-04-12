#include "LP4DSM.h"
#include "MODEL.h"


//glogal data
 uint8_t flight_mode;
 bool bind_status;
 bool thr_cut;

 const Led_data_t led_data_Stunt[]={{200,60},{00,60}};
 const Led_data_t led_data_cut[]={{200,20},{00,20}};
 int16_t out_data[OUT_CHAN_CNT];	

void Model_Init(void)
{
	flight_mode=FLIGHT_MODE_STD;
	bind_status=false;
	thr_cut=false;
}

inline void CheckTrimKeys(uint32_t keys)
{
	//check left sick trim
	if(GET_KEY_RISING_EDGE(keys,KEY_LEFT_VERT_TRIM_P))
	{
		StickTrimUp(IN_LEFT_VERT_CH);
	}
	if(GET_KEY_RISING_EDGE(keys,KEY_LEFT_VERT_TRIM_M))
	{
		StickTrimDown(IN_LEFT_VERT_CH);
	}
	if(GET_KEY_RISING_EDGE(keys,KEY_LEFT_HRIZ_TRIM_P))
	{
		StickTrimUp(IN_LEFT_HRIZ_CH);
	}
	if(GET_KEY_RISING_EDGE(keys,KEY_LEFT_HRIZ_TRIM_M))
	{
		StickTrimDown(IN_LEFT_HRIZ_CH);
	}
	//check right sick trim
	if(GET_KEY_RISING_EDGE(keys,KEY_RIGHT_VERT_TRIM_P))
	{
		StickTrimUp(IN_RIGHT_VERT_CH);
	}
	if(GET_KEY_RISING_EDGE(keys,KEY_RIGHT_VERT_TRIM_M))
	{
		StickTrimDown(IN_RIGHT_VERT_CH);
	}
	if(GET_KEY_RISING_EDGE(keys,KEY_RIGHT_HRIZ_TRIM_P))
	{
		StickTrimUp(IN_RIGHT_HRIZ_CH);
	}
	if(GET_KEY_RISING_EDGE(keys,KEY_RIGHT_HRIZ_TRIM_M))
	{
		StickTrimDown(IN_RIGHT_HRIZ_CH);
	}
}


void Model_CheckKeys(uint32_t keys)
{
	bool changed=false;
	CheckTrimKeys(keys);
	if(GET_KEY_RISING_EDGE(keys,KEY_RATE_BTN))
	{
		flight_mode++;
		flight_mode=flight_mode%FLIGHT_MODE_CNT;
		note_t confirm={OCT6,C6_CNT,NOTE_DURATION(8,60)};
		if(flight_mode)
			{
				confirm.cnt=B6_CNT;
			}
		buz_PlayNote(confirm);
		changed=true;
	}
	if(changed)
		{
		if(thr_cut)
			SetLedStateFlashing(led_data_cut[0],led_data_cut[1]);
		else if(flight_mode)
			SetLedStateFlashing( led_data_Stunt[0], led_data_Stunt[1]);
		else
			SetLedStateSolid(128);
		}
	if(ConfigModellType==MODEL_TYPE_CLASIC)
	{
		bind_status=(GET_KEY(keys,KEY_BIND_BTN));
		if(bind_status)
		{
		note_t confirm={OCT8,FS8_CNT,NOTE_DURATION(32,120)};
		buz_PlayNote(confirm);
		}
	}
	else if(GET_KEY_RISING_EDGE(keys,KEY_BIND_BTN))
	{
		thr_cut=!thr_cut;
		note_t confirm={OCT6,FS6_CNT,NOTE_DURATION(8,60)};
		buz_PlayNote(confirm);
		changed=true;
	}
	
}


void ModelCalculateOutChannelsData(void)
{
	int16_t values[RATE_CH_CNT];
	//apply rate and expo
	values[EXPO_IDX_AILE]=Rate(ConfigRate(flight_mode,RATE_IDX_AILE),expo(ConfigExpo(flight_mode,EXPO_IDX_AILE),getStickValue(IN_AILE_CH)));
	values[EXPO_IDX_ELEV]=Rate(ConfigRate(flight_mode,RATE_IDX_ELEV),expo(ConfigExpo(flight_mode,EXPO_IDX_ELEV),getStickValue(IN_ELEV_CH)));
	values[EXPO_IDX_RUDD]=Rate(ConfigRate(flight_mode,RATE_IDX_RUDD),expo(ConfigExpo(flight_mode,EXPO_IDX_RUDD),getStickValue(IN_RUDD_CH)));

	switch (ConfigModellType)
	{
		case MODEL_TYPE_HELI:
			//apply curve
			out_data[OUT_THROTTLE_CH] =thr_cut?CHAN_MIN_VALUE:curveXpoint(ConfigThrCurve(flight_mode),getStickValue(IN_THROTTLE_CH));
			//apply rate and expo
			out_data[OUT_AILE_CH]=values[EXPO_IDX_AILE];
			out_data[OUT_ELEV_CH]=values[EXPO_IDX_ELEV];
			out_data[OUT_RUDD_CH]=values[EXPO_IDX_RUDD];
			out_data[OUT_GEAR_CH]=ConfigGyroGain(flight_mode);
			out_data[OUT_AUX1_CH]=curveXpoint(ConfigPitchCurve(flight_mode),getStickValue(IN_THROTTLE_CH));;
		break;
		case MODEL_TYPE_CLASIC:
			out_data[OUT_THROTTLE_CH]= getStickValue(IN_THROTTLE_CH);
			out_data[OUT_AILE_CH]=values[EXPO_IDX_AILE];
			out_data[OUT_ELEV_CH]=values[EXPO_IDX_ELEV];
			out_data[OUT_RUDD_CH]=values[EXPO_IDX_RUDD];
			out_data[OUT_GEAR_CH]= ConfigGyroGain(flight_mode);
			out_data[OUT_AUX1_CH]= bind_status?SWITCH_ON_VALUE:SWITCH_OFF_VALUE;
		break;
		case MODEL_TYPE_AIRPLANE:
			out_data[IN_THROTTLE_CH]=getStickValue(IN_THROTTLE_CH);

			//apply mixing
			out_data[OUT_AILE_CH]=mix(values,ConfigAileMix(flight_mode));
			out_data[OUT_ELEV_CH]=mix(values,ConfigElevMix(flight_mode));
			out_data[OUT_RUDD_CH]=values[EXPO_IDX_RUDD];
			out_data[OUT_GEAR_CH]=thr_cut?SWITCH_ON_VALUE:SWITCH_OFF_VALUE;
			out_data[OUT_AUX1_CH]=mix(values,ConfigAuxMix(flight_mode))+ConfigGyroGain(flight_mode);
		break;
	}
}



uint16_t ModelGetCannel(int8_t Nr)
{
return Scale(CHAN_MIN_VALUE,CHAN_MAX_VALUE,OUT_CHAN_MIN_VAL,OUT_CHAN_MAX_VAL,out_data[Nr]);
}

