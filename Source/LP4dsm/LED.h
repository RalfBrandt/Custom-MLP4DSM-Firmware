#ifndef LED_H
#define LED_H

// ************************** defines*************************************************************
#define LED_DATA_CNT 16

typedef struct 
	{
	uint8_t value; //%
	uint8_t duration; //ds (10 ms)
	}Led_data_t;

//prototypes ********************************************************************************
void LedCyclic(void);
void SetLedVal(uint8_t value);
void InitLed(void);
uint8_t LedSequenceCount(void);
void SetLedStateFlashing(Led_data_t on,Led_data_t off);
void SetLedStateSolid(uint8_t value);
void SetLedStateCyclicSequence(uint8_t cnt,const Led_data_t * data);
void SetLedStateSingelSequence(uint8_t cnt,const Led_data_t * data);


#endif //LED