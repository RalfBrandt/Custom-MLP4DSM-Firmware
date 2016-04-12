#ifndef MODEL_H
#define MODEL_H

//defines***************************************
#define MODEL_TYPE_CLASIC 0
#define MODEL_TYPE_HELI 1
#define MODEL_TYPE_AIRPLANE 2
#define MODEL_TYPE_CNT 3

#define FLIGHT_CNT_PER_S 46545UL
#define SWITCH_ON_VALUE (CHAN_CENTER_VALUE+(100L*CHAN_RANGE)/125)
#define SWITCH_OFF_VALUE (CHAN_CENTER_VALUE-(100L*CHAN_RANGE)/125)


//global data


//prototypes*****************************************
void Model_Init(void);
uint16_t ModelGetCannel(int8_t Nr);
void Model_CheckKeys(uint32_t keys);
void ModelCalculateOutChannelsData(void);



#endif //MODEL_H