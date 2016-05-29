#ifndef ANALOG_H
#define ANALOG_H

// ************************** defines*************************************************************
#define OVERSAMPLE_CNT 8
#define ADC_CHANEL_CNT 4
#define ADC_OUT_CHANEL_CNT ADC_CHANEL_CNT+1 //V+BAT
#define ADC_BITS 10
#define ADC_MAX ((1<<(ADC_BITS))-1)
#define ADC_MIN 0
#define ADC_CENTER (1<<(ADC_BITS-1))
#define ADC_UPDATE_INTERVAL11MS  (UPDATE_CNT11MS/(OVERSAMPLE_CNT*ADC_CHANEL_CNT+1)) //ticks per adc sample //one extra for VBAT
#define ADC_UPDATE_INTERVAL22MS  (UPDATE_CNT22MS/(OVERSAMPLE_CNT*ADC_CHANEL_CNT+1)) //ticks per adc sample //one extra for VBAT
//analog input channel map
#define IN_LEFT_VERT_CH 0//IN_RV1_CH
#define IN_LEFT_HRIZ_CH 1//IN_RV2_CH
#define IN_RIGHT_VERT_CH 2//IN_RV4_CH
#define IN_RIGHT_HRIZ_CH 3//IN_RV3_CH
#define IN_VREF_CH	7

#define IN_RV1_CH	5	
#define IN_RV2_CH	4	
#define IN_RV3_CH	1	
#define IN_RV4_CH	0	

#define ADC_VREF_IDX ADC_CHANEL_CNT
//#define ADC_GET_CHANEL(x) (adc_avg[x])
#define ADC_GET_CHANEL(x) AdcGetChanel(x)
#define VOLTS(Volt) ((uint16_t)((1.0 * Volt * ADC_MAX)/(2 * 3.3)))
#define MIN_BAT_VOLTS 3.7

//prototypes ********************************************************************************
void AdcInit(void);
int16_t AdcGetChanel(uint8_t idx);
//public data************************************************************************************
extern volatile bool adc_new_data;
extern uint8_t adc_tick;



#endif //ANALOG_H