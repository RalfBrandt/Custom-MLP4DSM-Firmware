#ifndef KEYS_H
#define KEYS_H

// ************************** defines*************************************************************
#define KEY_ROWS_CNT 5
#define KEY_COLUMN_CNT 2
#define KEY_CNT 11
#define KEY_OERSCAN 4
#define KEY_SCAN_CNT (KEY_OERSCAN*KEY_COLUMN_CNT)
/*

    PB5	PB1
PD4	SW1	SW2		
PD2	SW3	SW4		
PD7	SW5	SW6		
PD6	SW7	SW8		
PD0	SW9	SW10	BIND	RATE
PC3	MODE 1/2 SW
*/
#define SW1_KEY KEY_ROW1_PIN
#define SW2_KEY (KEY_ROW1_PIN+8)
#define SW3_KEY KEY_ROW2_PIN
#define SW4_KEY (KEY_ROW2_PIN+8)
#define SW5_KEY KEY_ROW3_PIN
#define SW6_KEY (KEY_ROW3_PIN+8)
#define SW7_KEY KEY_ROW4_PIN
#define SW8_KEY (KEY_ROW4_PIN+8)
#define SW9_KEY KEY_ROW5_PIN
#define SW10_KEY (KEY_ROW5_PIN+8)
#define SW11_KEY KEY_SW11_PIN


//use this for GET_KEY
#define KEY_LEFT_VERT_TRIM_P SW1_KEY
#define KEY_LEFT_VERT_TRIM_M SW2_KEY
#define KEY_LEFT_HRIZ_TRIM_P SW4_KEY
#define KEY_LEFT_HRIZ_TRIM_M SW3_KEY
#define KEY_RIGHT_VERT_TRIM_P SW5_KEY
#define KEY_RIGHT_VERT_TRIM_M SW6_KEY
#define KEY_RIGHT_HRIZ_TRIM_P SW7_KEY
#define KEY_RIGHT_HRIZ_TRIM_M SW8_KEY
#define KEY_BIND_BTN SW9_KEY
#define KEY_RATE_BTN SW10_KEY
#define KEY_MODE_SW SW11_KEY


#define KEY_COLUMN1_PIN  PORTB5
#define KEY_COLUMN2_PIN  PORTB1

#define KEY_COLUMN_PORT PORTB
#define KEY_COLUMN_DDR  DDRB

#define KEY_ROW1_PIN  PIND4
#define KEY_ROW2_PIN  PIND2
#define KEY_ROW3_PIN  PIND7
#define KEY_ROW4_PIN  PIND6
#define KEY_ROW5_PIN  PIND0
#define KEY_ROW_PIN   PIND
#define KEY_ROW_PORT  PORTD
#define KEY_ROW_DDR	  DDRD

#define KEY_ROW_MASK ((1<<KEY_ROW1_PIN)|(1<<KEY_ROW2_PIN)|(1<<KEY_ROW3_PIN)|(1<<KEY_ROW4_PIN)|(1<<KEY_ROW5_PIN))
#define KEY_ROW_PINS (~KEY_ROW_PIN&KEY_ROW_MASK)

#define KEY_SW11_PIN PINC3
#define KEY_SW11_PORT PINC
#define KEY_SW11_DDR DDRC
#define KEY_SW11_MASK (1<<KEY_SW11_PIN)
#define KEY_SW11_PINS (~KEY_SW11_PORT&KEY_SW11_MASK)

#define KEY_EDGE(keys) ( ( (keys&0xFFFF) ^ ((keys>>16)) ) &0xFFFF)
#define GET_KEY(keys,index) (((keys&0xFFFF)&((1<<index)&0xFFFF)))
//#define GET_KEY(keys,index) Get_Key(keys,index)

#define GET_KEY_RISING_EDGE(keys,index) ( (KEY_EDGE(keys)&(keys&0xFFFF)) & ((1<<index)&0xFFFF) ) 
//#define GET_KEY_RISING_EDGE(keys,index) Get_Key_Rising_Edge(keys,index)

#define GET_KEY_FALING_EDGE(keys,index) ( (KEY_EDGE(keys)&(keys>>16)) & ((1<<index)&0xFFFF) )

//prototypes ********************************************************************************
void key_cyclic(void);
void key_init(void);
uint32_t Get_keys(void);
bool Get_Key(uint16_t keys,uint8_t index);
bool Get_Key_Rising_Edge(uint32_t keys, uint8_t index);
//public data************************************************************************************

#endif //KEYS