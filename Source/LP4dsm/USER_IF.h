#ifndef USER_IF_H
#define USER_IF_H


#define CONFIG_SAVE_KEY KEY_LEFT_HRIZ_TRIM_M //key used to save changed config
#define CONFIG_CHECK_KEY KEY_LEFT_VERT_TRIM_P //key used to check for Stick position and read value
#define CONFIG_NEXT_KEY KEY_RIGHT_HRIZ_TRIM_M //key to go to next step
#define CONFIG_FLIGHT_TIME_P KEY_RIGHT_VERT_TRIM_P
#define CONFIG_FLIGHT_TIME_M KEY_RIGHT_VERT_TRIM_M
#define CONFIG_VAL_P KEY_RIGHT_VERT_TRIM_P
#define CONFIG_VAL_M KEY_RIGHT_VERT_TRIM_M

#define L_STICK_TX_CONFIG_POS STICK_LOWER_RIGHT //left stick position for TX config menu
#define L_STICK_MODEL_SELECT_POS STICK_UPPER_LEFT //left stick position for Model select menu
#define L_STICK_OPTION_CONFIG_POS STICK_LOWER_LEFT //left stick position for option config menu 
#define L_STICK_MODEL_CONFIG_POS STICK_UPPER_RIGHT //left stick position for Model config

//Stick position for tx config
#define R_STICK_TX_CONFIG_DSMX STICK_CENTER_LEFT //DSMX Mode 
#define R_STICK_TX_CONFIG_DSM2 STICK_CENTER_RIGHT //DSM2 Mode
#define R_STICK_TX_CONFIG_DSMF STICK_UPPER_CENTER //DSM France Mode
#define R_STICK_TX_CONFIG_RESET STICK_LOWER_CENTER //Reset to factor default
#define R_STICK_TX_CONFIG_MODE1 STICK_UPPER_LEFT //Input mode 1
#define R_STICK_TX_CONFIG_MODE2 STICK_UPPER_RIGHT //Input mode 2
#define R_STICK_TX_CONFIG_MODE3 STICK_LOWER_RIGHT //Input mode 3
#define R_STICK_TX_CONFIG_MODE4 STICK_LOWER_LEFT //Input mode 4
//stick positions for Model select
#define R_STICK_MODEL_SELECT1 STICK_UPPER_LEFT  //Model1
#define R_STICK_MODEL_SELECT2 STICK_UPPER_RIGHT	 //Model2
#define R_STICK_MODEL_SELECT3 STICK_LOWER_RIGHT	 //Model3
#define R_STICK_MODEL_SELECT4 STICK_LOWER_LEFT	 //Model4
//stick positions for Option config
#define R_STICK_OPTION_FAST_MODEL_SEL_ON STICK_UPPER_LEFT	//fast model selection on
#define R_STICK_OPTION_FAST_MODEL_SEL_OFF STICK_UPPER_RIGHT	//fast model selection off
#define R_STICK_OPTION_CONFIG_WHILE_RUNNIG_ON STICK_CENTER_LEFT	//config while running on
#define R_STICK_OPTION_CONFIG_WHILE_RUNNIG_OFF STICK_CENTER_RIGHT //config while running off
#define R_STICK_CALIBRATE_INPUT_POS STICK_UPPER_CENTER //calibrate sticks
#define R_STICK_MODEL_CLASIC_POS STICK_LOWER_RIGHT //CLASIC  MODEL
#define R_STICK_MODEL_HELI_POS STICK_LOWER_LEFT //heli
#define R_STICK_MODEL_AIRPLANE_POS STICK_LOWER_CENTER //Airplane
//stick positions for HELI / Airplane Model config
#define R_STICK_CURVE1_POS STICK_UPPER_RIGHT //configure curve 1 /pitch  
#define R_STICK_CURVE2_POS STICK_UPPER_LEFT //configure curve 2 /thr
#define R_STICK_CURVE3_POS STICK_CENTER_RIGHT //configure curve 3
#define R_STICK_EXP_POS STICK_LOWER_RIGHT //configure expo,rate gyro gain standard mode
#define R_STICK_RATE_POS STICK_LOWER_LEFT //configure expo,rate gyro gain standard mode
#define R_STICK_GYRO_POS STICK_CENTER_LEFT //configure expo,rate gyro gain stunt mode
#define R_STICK_11MS_POS STICK_UPPER_CENTER  //11 ms update rate
#define R_STICK_22MS_POS STICK_LOWER_CENTER  //22 ms update rate

#define STICK_INC_STEP ((5L*CHAN_RANGE)/100)
#define FLIGHT_TIME_STEP 15
//Gesture detection
#define STICK_UPPER_POS_LIM ((ADC_MAX*75UL)/100)
#define STICK_LOWER_POS_LIM ((ADC_MAX*25UL)/100)
#define STICK_ISUPPER(x) (x>STICK_UPPER_POS_LIM)
#define STICK_ISLOWER(x) (x<STICK_LOWER_POS_LIM)
#define STICK_ISLEFT(x) STICK_ISUPPER(x)
#define STICK_ISRIGHT(x) STICK_ISLOWER(x)
#define STICK_ISCENTER(x) ((x<STICK_UPPER_POS_LIM)&&(x>STICK_LOWER_POS_LIM))
//bit coding
#define STICK_V_ISLOWER 0x01
#define STICK_V_ISCENTER 0x02
#define STICK_V_ISUPPER 0x04
#define STICK_H_ISRIGHT 0x10
#define STICK_H_ISCENTER 0x20
#define STICK_H_ISLEFT 0x40

#define V_STICK_EVAL(y) ((STICK_ISLOWER(y)?STICK_V_ISLOWER:0)|(STICK_ISUPPER(y)?STICK_V_ISUPPER:0)|(STICK_ISCENTER(y)?STICK_V_ISCENTER:0))
#define H_STICK_EVAL(x) ((STICK_ISRIGHT(x)?STICK_H_ISRIGHT:0)|(STICK_ISLEFT(x)?STICK_H_ISLEFT:0)|(STICK_ISCENTER(x)?STICK_H_ISCENTER:0))
#define STICK_EVAL(x,y)  (V_STICK_EVAL(y)|H_STICK_EVAL(x))

#define STICK_LOWER_LEFT (STICK_V_ISLOWER|STICK_H_ISLEFT)
#define STICK_LOWER_CENTER (STICK_V_ISLOWER|STICK_H_ISCENTER)
#define STICK_LOWER_RIGHT (STICK_V_ISLOWER|STICK_H_ISRIGHT)
#define STICK_UPPER_LEFT (STICK_V_ISUPPER|STICK_H_ISLEFT)
#define STICK_UPPER_CENTER (STICK_V_ISUPPER|STICK_H_ISCENTER)
#define STICK_UPPER_RIGHT (STICK_V_ISUPPER|STICK_H_ISRIGHT)
#define STICK_CENTER_LEFT (STICK_V_ISCENTER|STICK_H_ISLEFT)
#define STICK_CENTER_CENTER (STICK_V_ISCENTER|STICK_H_ISCENTER)
#define STICK_CENTER_RIGHT (STICK_V_ISCENTER|STICK_H_ISRIGHT)


//prototypes********************************************
bool UI_checkforStartupGesture(uint16_t keys);
void UI_CheckForConfig(uint16_t keys);
void UI_init(void);
bool UI_ProceedConfigMode(void);


extern const Led_data_t led_data_confiMode[];

#endif //USER_IF_H