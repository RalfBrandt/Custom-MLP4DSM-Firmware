#ifndef CURVE_H
#define CURVE_H

#define CHAN_MAX_VALUE (CHAN_CENTER_VALUE+CHAN_RANGE)
#define CHAN_CENTER_VALUE 0L
#define CHAN_RANGE 2000L
#define CHAN_MIN_VALUE (CHAN_CENTER_VALUE-CHAN_RANGE)

#define CURVE_POINT_CNT 3
#define CURVE_MIN_POS 0
#define CURVE_CENTER_POS 1
#define CURVE_MAX_POS 2


#define MIX_CH_CNT 3
#define MIX_CH1 0
#define MIX_CH2 1
#define MIX_CH3 2


typedef int16_t Curve3_t[CURVE_POINT_CNT];
typedef int16_t Mix_3_t[MIX_CH_CNT];

//prototypes **********************************************************************
int16_t expo(int16_t k, int16_t value); 
int16_t curveXpoint(Curve3_t curve, int16_t value);
int16_t Rate(int16_t k,int16_t value);	
uint16_t mix(int16_t *values,int16_t *scales);//mix the chanels
int16_t Scale(int16_t in_min,int16_t in_max,int16_t out_min, int16_t out_max,int16_t value);//linear scale
int16_t ScaleHalf(int16_t in_min,int16_t in_max,int16_t value);//fixed output range from 0 to CHAN_RANGE




#endif //CURVE_H