#include "LP4DSM.h"
#include "SPECTRUM_TX.h"
/*
DSM2/DSMX mode:               18 00 AA AA BB BB CC CC DD DD EE EE FF FF
DSM2/DSMX mode, range test:   38 00 AA AA BB BB CC CC DD DD EE EE FF FF
DSM2/DSMX mode, bind:         98 00 00 00 05 FF 09 FF 0D FF 10 AA 14 AA

DSM2 mode:                    10 00 AA AA BB BB CC CC DD DD EE EE FF FF
DSM2 mode, range test:        30 00 AA AA BB BB CC CC DD DD EE EE FF FF
DSM2 mode, bind:              90 00 00 00 05 FF 09 FF 0D FF 10 AA 14 AA

France mode:                  00 00 AA AA BB BB CC CC DD DD EE EE FF FF
France mode, range test:      20 00 AA AA BB BB CC CC DD DD EE EE FF FF
France mode, bind:            80 00 00 00 05 FF 09 FF 0D FF 10 AA 14 AA

bit 7 - 1 -> bind mode enable
bit 6 - unknown, always 0
bit 5 - 1 -> range test enable
bit 4 - 1 -> normal mode, 0 -> france mode
bit 3 - 1 -> DSMX enable
bit 2 - unknown, always 0
bit 1 - unknown, always 0
bit 0 - unknown, always 0

Note:
The manual says that in france mode DSMX cannot be enabled.

*/

//global data
static uint8_t tx_buf[TX_LEN]; //transmitt buffer
static uint8_t tx_idx=0;			//index used for interrupt driven tx
	//RC data
static int16_t OUT_CHAN_data[OUT_CHAN_CNT];
volatile bool tx_active=false;

//set value for a given cannel
void tx_SetChanelData(int16_t value,uint8_t idx)	
{
	int16_t val=value;
	if(idx<OUT_CHAN_CNT)
	{
		if(val>OUT_CHAN_MAX_VAL)
			val=OUT_CHAN_MAX_VAL;
		if(val<OUT_CHAN_MIN_VAL)
			val=OUT_CHAN_MIN_VAL;
		OUT_CHAN_data[idx]=val;
	}
}


//set header byte
void tx_SetMode(uint8_t mode)
{
	tx_buf[0]=mode;
}

void tx_SetBindMode(bool bind)
{
	if(bind)
	{
		tx_buf[0]|=(1<<SPTX_BIND_MODE);
	}
	else
	{
		tx_buf[0]&=~(1<<SPTX_BIND_MODE);
	}
}

void tx_SetRangeCheck(bool rangeCheck)
{
	if(rangeCheck)
	{
		tx_buf[0]|=(1<<SPTX_RANGE_CHECK);
	}
	else
	{
		tx_buf[0]&=~(1<<SPTX_RANGE_CHECK);
	}
}

//convert data from chanal_data into bytes for tx buffer
void tx_ConvertChannelData(void)
{
	for(uint8_t n=0;n<OUT_CHAN_CNT;n++)
	{
		uint16_t val=OUT_CHAN_data[n];
		uint8_t mask=(((1<<OUT_CHAN_RESOLUTION)-1)>>8);
		tx_buf[n*2+2] =  ((val >> 8) & mask);
		tx_buf[n*2+3] = (val  & 0xff);
		tx_buf[n*2+2] |=(n<<(OUT_CHAN_RESOLUTION-8));
	}
}


void tx_Init(void)
{
	//int data
	tx_idx=0;
	tx_active=false;
	//setup USART for TX
	UBRR0=BAUD_PRESCALE; //set baud rate
	UCSR0B = (1<<TXEN0); //enable TX,
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00); // Async mode, 8N1

	//init global rc data***************************************************************************
	for(uint8_t n=0;n<OUT_CHAN_CNT;n++)
	OUT_CHAN_data[n]=OUT_CHAN_CENTER_VAL;
	
	tx_SetMode(ConfigRF_Mode);
	tx_ConvertChannelData();
}

//called when data register is empty, so we can send the next byte
ISR(USART_UDRE_vect)
{
	while(UCSR0A&(1<<UDRE0)&&tx_idx)
		{
		UDR0=tx_buf[tx_idx++];
		} 
	if(tx_idx==TX_LEN)
	{
		UCSR0B &= ~(1<<UDRIE0); //Data Register Empty Interrupt disable,
	}
}

//start sending the TX buffer
void SendTX_Buf(void)
{
	tx_idx=0; //reset index
	UDR0=tx_buf[tx_idx++]; //remaining bytes will be send by interrupt
	if(UCSR0A&(1<<UDRE0)&&tx_idx)
	{
		UDR0=tx_buf[tx_idx++];
	}
	UCSR0B |= (1<<UDRIE0); //Data Register Empty Interrupt enable,
}


void tx_Send(void)
{
	tx_ConvertChannelData();
	tx_active=1; //sent will be trigerd by timer interrupt 
#ifndef SEND_ON_IRQ
	SendTX_Buf();
#endif
}
