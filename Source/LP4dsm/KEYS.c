#include "LP4DSM.h"
#include "KEYS.h"

//public data************************************************************************************
 uint8_t key_data[KEY_OERSCAN][KEY_COLUMN_CNT];
 uint16_t key_Status;
 uint16_t key_StatusLast;
 uint8_t key_ScanCnt;

#ifdef DEBUG
uint32_t debug_key_in=0;//(1<<KEY_LEFT_VERT_TRIM_P);
#endif

//called form systick perform martix scan and over scan
void key_cyclic(void)
{
	uint8_t scan_cnt=key_ScanCnt>>1;// /KEY_COLUMN_CNT;
	uint8_t row=key_ScanCnt&1; // %KEY_COLUMN_CNT;

	//get column data
	key_data[scan_cnt][row]=(KEY_ROW_PINS|KEY_SW11_PINS);

	//select next row
	if(row) //1
	{
		KEY_COLUMN_DDR|=(1<<KEY_COLUMN1_PIN); //activate column 1
		KEY_COLUMN_DDR&=~(1<<KEY_COLUMN2_PIN); //deactivate column 2
	}
	else //0
	{
		KEY_COLUMN_DDR^=((1<<KEY_COLUMN1_PIN)|(1<<KEY_COLUMN2_PIN)); //activate column 2 deactivate 1
	}
	//sum up scanned data (overscan)
	//if a key was low all the time clear staus bit , if hight all the time set staatus bit 
	{
		uint8_t valH=0xff;
		uint8_t valL=0;
		for(uint8_t n=0;n<KEY_OERSCAN;n++)
		{
			valH&=key_data[n][0];
			valL|=key_data[n][0];
		}
		*((uint8_t*)(&key_Status))|=valH;
		*((uint8_t*)(&key_Status))&=valL;
		valH=0xff;
		valL=0;
		for(uint8_t n=0;n<KEY_OERSCAN;n++)
		{
			valH&=key_data[n][1];
			valL|=key_data[n][1];
		}
		*((uint8_t*)(&key_Status)+1)|=valH;
		*((uint8_t*)(&key_Status)+1)&=valL;
	}
#ifdef DEBUG
	key_Status=debug_key_in;
#endif

	//inc scan cnt
	key_ScanCnt++;
	key_ScanCnt=key_ScanCnt%KEY_SCAN_CNT;
	
}

//initialize I/O for keys
void key_init(void)
{
	KEY_COLUMN_PORT&=~((1<<KEY_COLUMN1_PIN)|(1<<KEY_COLUMN2_PIN)); //key matrix columns are outputs need to pull low
	KEY_COLUMN_DDR|=(1<<KEY_COLUMN1_PIN); //activate column 1 
	KEY_COLUMN_DDR&=~(1<<KEY_COLUMN2_PIN); //deactivate column 2
	KEY_ROW_DDR&=~(KEY_ROW_MASK); //row are input
	KEY_ROW_PORT|=KEY_ROW_MASK;		//activate Pull_Up 
	
	KEY_SW11_DDR&=(1<<KEY_SW11_PIN); //Mode pin is input

	key_ScanCnt=0;
}

//get key status one bit per switch
uint32_t Get_keys(void)
{
	uint32_t retval=key_StatusLast;
	retval<<=16;
	retval|=key_Status;
	key_StatusLast=key_Status;
	return retval;
}

bool Get_Key(uint16_t keys,uint8_t index)
{
	return ((keys&(1<<index)));
}

bool Get_Key_Rising_Edge(uint32_t keys, uint8_t index)
{
	return( (KEY_EDGE(keys)&(keys&0xFFFF)) & ((1<<index)&0xFFFF) ); 
}
