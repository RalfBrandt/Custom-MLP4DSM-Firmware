#ifndef SPECTRUM_TX_H
#define SPECTRUM_TX_H

// ************************** defines*************************************************************
//UART
#define USART_BAUDRATE 131000
#define BAUD_PRESCALE ((( F_CPU / ( USART_BAUDRATE * 16UL ))) - 1)

//out channel to rf Module
#define OUT_CHAN_CNT 6		//number of channel supported by RF module
#define TX_LEN (OUT_CHAN_CNT*2+2)
#define OUT_CHAN_RESOLUTION 10 //bits
#define OUT_CHAN_MAX_VAL ((1<<OUT_CHAN_RESOLUTION)-1)
#define OUT_CHAN_CENTER_VAL (1<<(OUT_CHAN_RESOLUTION-1))
#define OUT_CHAN_MIN_VAL 0
//bits of header byte
#define SPTX_BIND_MODE 7
#define SPTX_RANGE_CHECK 5
#define SPTX_NORM_MODE 4	//0=france mode
#define SPTX_DSMX_MODE 3	//0=DSM2
//modes of operation
#define DSMX ((1<<SPTX_NORM_MODE)|(1<<SPTX_DSMX_MODE))
#define DSM2 ((1<<SPTX_NORM_MODE))
#define DSMF (0)


//output channel map for spektrum tx module
#define OUT_THROTTLE_CH 0		//THR
#define OUT_AILE_CH		1		//AILE / ROLL
#define OUT_ELEV_CH		2		//ELEV / NICK
#define OUT_RUDD_CH		3		//RUDD / YAW
#define OUT_GEAR_CH		4		//Gear  tail gyro gain
#define OUT_AUX1_CH		5		//Aux 1 /PITCH


//prototypes*************************************************************************************
void tx_Init(void);
void tx_SetChanelData(int16_t value,uint8_t idx);
void tx_SetRangeCheck(bool rangeCheck);
void tx_SetModelNr(uint8_t Nr);
void tx_SetBindMode(bool bind);
void tx_SetMode(uint8_t mode);
void tx_Send(void);
void SendTX_Buf(void);

//global data*********************************************
extern volatile	bool tx_active;

#endif //SPECTRUM_TX