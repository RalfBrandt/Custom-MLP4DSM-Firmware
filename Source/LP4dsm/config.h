#ifndef CONFIG_H
#define CONFIG_H

#include <string.h>
 #include <avr/pgmspace.h>

//512 byte eeprom

//defines ***************************************************
#define TX_CONFIG_ADDR 0
#define TX_CONFIG_SIZE sizeof(tx_config)
#define MODEL_CONFIG_ADDR 50
#define MODEL_CONFIG_CNT 4
#define MODEL_CONFIG_SIZE sizeof(model_config_t)

#define MODEL_CONFIG_FLIGHT_MODE_CNT 2

#define MODEL_CONFIG_MIXER_CNT 3
#define MODEL_CONFIG_CURVE_CNT 2
 
#define FLIGHT_MODE_STD 0
#define FLIGHT_MODE_STUNT 1
#define FLIGHT_MODE_CNT 2

#define RATE_CH_CNT 3
#define FIRST_RATE_CH OUT_AILE_CH
#define EXPO_IDX_AILE (OUT_AILE_CH-FIRST_RATE_CH)
#define EXPO_IDX_ELEV (OUT_ELEV_CH-FIRST_RATE_CH)
#define EXPO_IDX_RUDD (OUT_RUDD_CH-FIRST_RATE_CH)
#define RATE_IDX_AILE (OUT_AILE_CH-FIRST_RATE_CH)
#define RATE_IDX_ELEV (OUT_ELEV_CH-FIRST_RATE_CH)
#define RATE_IDX_RUDD (OUT_RUDD_CH-FIRST_RATE_CH)


#define TX_CONFIG_ID ( (((uint32_t)'T')<<24)|(((uint32_t)'X')<<16)|0x201)
#define MODEL_CONFIG_ID ( (((uint32_t)'M')<<24)|(((uint32_t)'O')<<16)|0x205)

#define CONFIG_CALIB_CNT 3
#define CONFIG_CALIB_MIN 0
#define CONFIG_CALIB_CENT 1
#define CONFIG_CALIB_MAX 2

#define MODEL_CONFIG_HELI_DEFAULT {MODEL_CONFIG_ID,MODEL_TYPE_HELI,DEFAULT_RF_MODE,DEFAULT_MODEL_OPTIONS, DEFAULT_TRIM,DEFAULT_FLIGHT_TIME_HELI,\
		DEFAULT_GYRO_GAIN_HELI,DEFAULT_EXPO_HELI,DEFAULT_RATE_HELI,{DEFAULT_HELI_CURVES}}
	
#define MODEL_CONFIG_AIRPLANE_DEFAULT {MODEL_CONFIG_ID,MODEL_TYPE_AIRPLANE,DEFAULT_RF_MODE,DEFAULT_MODEL_OPTIONS,DEFAULT_TRIM,DEFAULT_FLIGHT_TIME_AIRPLANE,\
		DEFAULT_GYRO_GAIN_AIRPLANE,DEFAULT_EXPO_AIRPLANE,DEFAULT_RATE_AIRPLANE,{DEFAULT_AIRPLANE_MIX}}
		
#define MODEL_CONFIG_CLASIC_DEFAULT {MODEL_CONFIG_ID,MODEL_TYPE_CLASIC,DEFAULT_RF_MODE,DEFAULT_MODEL_OPTIONS,DEFAULT_TRIM,DEFAULT_FLIGHT_TIME_CLASIC,\
		DEFAULT_GYRO_GAIN_CLASSIC,DEFAULT_EXPO_CLASSIC,DEFAULT_RATE_CLASSIC,}

typedef struct 
	{
	uint32_t ID;	
	uint8_t stic_mode; //stick input mode 1 to 4
	uint8_t modell_Nr; //model selected
	uint8_t options; //varius obtion bits
	int16_t Stick_Calibaration[ADC_CHANEL_CNT][CONFIG_CALIB_CNT];
	}tx_config_t;
	
typedef struct  
{
	uint32_t ID;
	uint8_t mode_type; //Heli, airplane, chasic 
	uint8_t rf_mode; //DSM2 DSMX DSMF (france mode)
	uint8_t options;
	int8_t trim[ADC_CHANEL_CNT]; 
	uint8_t FlightTimeLimit;
	int16_t gyro_gain[MODEL_CONFIG_FLIGHT_MODE_CNT];
	int16_t expo[MODEL_CONFIG_FLIGHT_MODE_CNT][RATE_CH_CNT];
	int16_t rate[MODEL_CONFIG_FLIGHT_MODE_CNT][RATE_CH_CNT];
	union {
		Mix_3_t mixs[MODEL_CONFIG_MIXER_CNT][MODEL_CONFIG_FLIGHT_MODE_CNT];
		Curve3_t curves[MODEL_CONFIG_CURVE_CNT][MODEL_CONFIG_FLIGHT_MODE_CNT];
	};
}model_config_t;
 
//glogal data******************************************************************
extern tx_config_t tx_config;
extern model_config_t model_config;
//pseudo function for model config access
#define ConfigModellType (model_config.mode_type)
#define ConfigGetModellOption(Nr) ((Nr>7)?false:((model_config.options&(1<<Nr))?true:false))
#define ConfigExpo(Mode,ch) (model_config.expo[Mode][ch])
#define ConfigRate(Mode,ch) (model_config.rate[Mode][ch])
#define ConfigStickCalibartion(Chanel,x) (tx_config.Stick_Calibaration[Chanel][x])

#define ConfigPitchCurve(Mode) (model_config.curves[0][Mode])
#define ConfigAileMix(Mode) (model_config.mixs[0][Mode])
#define ConfigCurve1(Mode) (model_config.curves[0][Mode])

#define ConfigThrCurve(Mode) (model_config.curves[1][Mode])
#define ConfigElevMix(Mode) (model_config.mixs[1][Mode])
#define ConfigCurve2(Mode) (model_config.curves[1][Mode])

#define ConfigAuxMix(Mode) (model_config.mixs[2][Mode])
#define ConfigCurve3(Mode)  (model_config.curves[2][Mode])

#define ConfigGyroGain(Mode) (model_config.gyro_gain[Mode])
#define ConfigTrim(Chanel) (model_config.trim[Chanel])

//prototypes******************************
void ConfigInit(void);
void ConfigSaveTxConfig(void);
void ConfigResetTxConfig(void);

//void ConfigReadModelConfig();
void ConfigSaveModelConfig();
void ConfigResetModelConfig(uint8_t mode_type);
bool configGet_Tx_option(uint8_t Nr);
uint8_t ConfigGetModellType();
void ConfigSaveTrim(void);

//pseudo functions for configuration access
#define ConfigStic_Mode (tx_config.stic_mode) 
#define ConfigRF_Mode (model_config.rf_mode)
#define Config_ModelNr (tx_config.modell_Nr)
#define Config_FlightTime (model_config.FlightTimeLimit)
#define CONFIG_GET_TX_OPTIONS(Nr) ((Nr>7)?false:((tx_config.options&(1<<Nr))?true:false))
//#define CONFIG_GET_TX_OPTIONS(Nr) configGet_Tx_option(Nr)
// functions for configuration access
void ConfigSetStic_Mode(uint8_t mode);
void ConfigSetRF_Mode(uint8_t mode);
void configSet_ModelNr(uint8_t modell_Nr);
void configSet_Tx_options(uint8_t Nr,bool value);



#endif //CONFIG_H
