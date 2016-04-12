#include "LP4DSM.h"
#include "CURVE.h"

//expo curve approximation range 0 to CHAN_RANGE
uint16_t expou(uint16_t value, uint8_t k)
{
	// k*x*x*x + (1-k)*x
	// 0 <= k <= 100
	#define KMAX 100
	uint32_t v=value;
	uint16_t val = (v * v / CHAN_RANGE * v / CHAN_RANGE * k
	+ (KMAX - k) * v + KMAX / 2) / KMAX;
	return val;
}


//bidirectional expo range CHAN_MIN_VALUE to CHAN_MAX_VALUE
int16_t expo(int16_t k, int16_t value)
{
	int16_t val;
	int16_t K;
	K=k + CHAN_RANGE; //only positive expo
	K=(K*100)/(CHAN_RANGE*2);
	if (K == 0)
	return value;

	if (value<CHAN_CENTER_VALUE)
	{
	val = CHAN_CENTER_VALUE - expou(CHAN_CENTER_VALUE-value, K);
	}
	else
	{
	val = CHAN_CENTER_VALUE+expou(value-CHAN_CENTER_VALUE, K);
	}
	
/*	if(val>CHAN_MAX_VALUE)
		val=CHAN_MAX_VALUE;
	if(val<CHAN_MIN_VALUE)
		val=CHAN_MIN_VALUE;*/
	return val;
}

//linear scale
int16_t Scale(int16_t in_min,int16_t in_max,int16_t out_min, int16_t out_max,int16_t value)
{
	int16_t range_in=in_max-in_min;
	int16_t range_out=out_max-out_min;
	int32_t val=((int32_t)range_out*(value-in_min));
	
	//devide and round to the nearest
	val+=(range_in/2); 
	val/=range_in; 
	
	//limot output
	val+=out_min;
	if(val>out_max)
		val=out_max;
	if(val<out_min)
		val=out_min;
	return (int16_t)val;	
}

//fixed output range from 0 to CHAN_RANGE
int16_t ScaleHalf(int16_t in_min,int16_t in_max,int16_t value)
{
	return Scale(in_min,in_max,0,CHAN_RANGE,value);
}


int16_t Rate(int16_t k,int16_t value)
{
		int32_t val=value-CHAN_CENTER_VALUE;
		val*=(k-CHAN_CENTER_VALUE);
		val+=CHAN_RANGE/2;
		val/=CHAN_RANGE;
		val+=CHAN_CENTER_VALUE;
/*		if(val>CHAN_MAX_VALUE)
			val=CHAN_MAX_VALUE;
		if(val<CHAN_MIN_VALUE)
			val=CHAN_MIN_VALUE;*/
		return val;
}
//linear interpolate
int16_t curveXpoint(Curve3_t curve, int16_t value)
{
		int16_t val;
		if(value<=CHAN_CENTER_VALUE)
		{
			val=Scale(CHAN_MIN_VALUE,CHAN_CENTER_VALUE,curve[CURVE_MIN_POS],curve[CURVE_CENTER_POS],value);
		}
		else
		{
			val=Scale(CHAN_CENTER_VALUE,CHAN_MAX_VALUE,curve[CURVE_CENTER_POS],curve[CURVE_MAX_POS],value);
		}
	return val;
}


uint16_t mix(int16_t *values,int16_t *scales)
{
	int32_t val=0;
	for(uint8_t n=0;n < MIX_CH_CNT;n++)
	{
		int32_t x=values[n]-CHAN_CENTER_VALUE;
		int16_t y=scales[n]-CHAN_CENTER_VALUE;
		val+=(x*y)/(int16_t)CHAN_RANGE;
	}
	val+=CHAN_CENTER_VALUE;
	if(val>CHAN_MAX_VALUE)
		val=CHAN_MAX_VALUE;
	if(val<CHAN_MIN_VALUE)
		val=CHAN_MIN_VALUE;

	return val ;
}
