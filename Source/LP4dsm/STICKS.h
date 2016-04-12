#ifndef STICKS_H
#define STICKS_H

//definitions **************************************************************************

//channel map for GetChannel
#define IN_THROTTLE_CH  0		//THR
#define IN_AILE_CH		1		//AILE / ROLL
#define IN_ELEV_CH		2		//ELEV / NICK
#define IN_RUDD_CH		3		//RUDD / YAW


//mapping for mode 1 to 4
#define STICK_MODE1 0
#define STICK_MODE2 1
#define STICK_MODE3 2
#define STICK_MODE4 3


//stick mode 1 to 4 chanel aligment
#define MODE1_CH1 IN_RIGHT_VERT_CH  //right vertical
#define MODE1_CH2 IN_RIGHT_HRIZ_CH //right horizontal
#define MODE1_CH3 IN_LEFT_VERT_CH  //left vertical
#define MODE1_CH4 IN_LEFT_HRIZ_CH  //left horizontal

#define MODE2_CH1 IN_LEFT_VERT_CH  //left vertical
#define MODE2_CH2 IN_RIGHT_HRIZ_CH  //right horizontal
#define MODE2_CH3 IN_RIGHT_VERT_CH  //right vertical
#define MODE2_CH4 IN_LEFT_HRIZ_CH  //left horizontal

#define MODE3_CH1 IN_RIGHT_VERT_CH  //right vertical
#define MODE3_CH2 IN_LEFT_HRIZ_CH  //left horizontal
#define MODE3_CH3 IN_LEFT_VERT_CH  //left vertical
#define MODE3_CH4 IN_RIGHT_HRIZ_CH //right horizontal

#define MODE4_CH1 IN_LEFT_VERT_CH //left vertical
#define MODE4_CH2 IN_LEFT_HRIZ_CH  //left horizontal
#define MODE4_CH3 IN_RIGHT_VERT_CH //right vertical
#define MODE4_CH4 IN_RIGHT_HRIZ_CH  //right horizontal

#define STICK_MODE_CNT 4
#define STCIK_MAX_TRIM 30
#define STICK_TRIM_TIC_CNT ((((1L*CHAN_RANGE)/50)+1)/2)

//prototypes***********************************************************************************
void StickInit(void);
int16_t getRawStickValue(uint8_t Nr);
int16_t getStickValue(uint8_t Nr);
void StickTrimUp(uint8_t adc_chanel);
void StickTrimDown(uint8_t adc_chanel);



#endif //STICKS_H