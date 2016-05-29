#include "LP4DSM.h"
#include "Config.h"


//glogal data
tx_config_t tx_config;
model_config_t model_config;

const model_config_t model_config_default[MODEL_TYPE_CNT]={MODEL_CONFIG_CLASIC_DEFAULT,MODEL_CONFIG_HELI_DEFAULT,MODEL_CONFIG_AIRPLANE_DEFAULT};

//genneral functions****************************************************
//read tx configuration from eeprom
void ConfigReadTxConfig(void)
{
	eeprom_read_block(&tx_config,TX_CONFIG_ADDR,TX_CONFIG_SIZE);
}

//read model configuration from eeprom
inline void ConfigReadModelConfig()
{
	eeprom_read_block(&model_config,(void*)(MODEL_CONFIG_ADDR+(MODEL_CONFIG_SIZE*tx_config.modell_Nr)),MODEL_CONFIG_SIZE);
}

//save tx configuration to eeprom
void ConfigSaveTxConfig(void)
{
	eeprom_update_block(&tx_config,TX_CONFIG_ADDR,TX_CONFIG_SIZE);
}

//save model configuration to eeprom
void ConfigSaveModelConfig()
{
	eeprom_update_block(&model_config,(void*)(MODEL_CONFIG_ADDR+MODEL_CONFIG_SIZE*tx_config.modell_Nr),MODEL_CONFIG_SIZE);
}

//save model configuration to eeprom
void ConfigSaveTrim(void)
{
	eeprom_update_block(&model_config.trim,(void*)(offsetof(model_config_t,trim)+MODEL_CONFIG_ADDR+MODEL_CONFIG_SIZE*tx_config.modell_Nr),sizeof(model_config));
}


//initialize configuration
void ConfigInit(void)
{
	ConfigReadTxConfig();
	if(tx_config.ID!=TX_CONFIG_ID)
	{
		ConfigResetTxConfig();
		ConfigSaveTxConfig();
	}
	configSet_ModelNr(tx_config.modell_Nr);
}

//read functions for tx config  *******************************************************
 

void configSet_Tx_options(uint8_t Nr,bool value)
{
	if(value)
		tx_config.options|=(1<<Nr);
	else
		tx_config.options&=~(1<<Nr);
}

void configSet_Rx_options(uint8_t Nr,bool value)
{
	if(value)
	model_config.options|=(1<<Nr);
	else
	model_config.options&=~(1<<Nr);
}

//write functions for tx config
void ConfigSetStic_Mode(uint8_t mode)
{
	tx_config.stic_mode=mode;
}

void ConfigSetRF_Mode(uint8_t mode)
{
	model_config.rf_mode=mode;
	tx_SetMode(mode);
}

bool configGet_Tx_option(uint8_t Nr)
{
	return ((Nr>7)?false:((tx_config.options&(1<<Nr))?true:false));
}

void configSet_ModelNr(uint8_t modell_Nr)
{
	/*if(modell_Nr>=MODEL_CONFIG_CNT)
	return;*/
	tx_config.modell_Nr=modell_Nr;
	tx_SetModelNr(tx_config.modell_Nr);
	ConfigReadModelConfig();
	if(model_config.ID!=MODEL_CONFIG_ID)
	{
		ConfigResetModelConfig(DEFAULT_MODEL_TYPE);
		ConfigSaveModelConfig();
	}
}





//reset tx configuration to default
void ConfigResetTxConfig(void)
{
tx_config.modell_Nr=0;
tx_config.stic_mode=STICK_MODE_DEFAULT;	
tx_config.ID=TX_CONFIG_ID;
tx_config.options=DEFAULT_TX_OPTIONS;
tx_config.Stick_Calibaration[IN_LEFT_VERT_CH][CONFIG_CALIB_MIN]=ADC_MIN;
tx_config.Stick_Calibaration[IN_LEFT_HRIZ_CH][CONFIG_CALIB_MIN]=ADC_MIN;
tx_config.Stick_Calibaration[IN_RIGHT_VERT_CH][CONFIG_CALIB_MIN]=ADC_MIN;
tx_config.Stick_Calibaration[IN_RIGHT_HRIZ_CH][CONFIG_CALIB_MIN]=ADC_MIN;

tx_config.Stick_Calibaration[IN_LEFT_VERT_CH][CONFIG_CALIB_CENT]=ADC_CENTER;
tx_config.Stick_Calibaration[IN_LEFT_HRIZ_CH][CONFIG_CALIB_CENT]=ADC_CENTER;
tx_config.Stick_Calibaration[IN_RIGHT_VERT_CH][CONFIG_CALIB_CENT]=ADC_CENTER;
tx_config.Stick_Calibaration[IN_RIGHT_HRIZ_CH][CONFIG_CALIB_CENT]=ADC_CENTER;

tx_config.Stick_Calibaration[IN_LEFT_VERT_CH][CONFIG_CALIB_MAX]=ADC_MAX;
tx_config.Stick_Calibaration[IN_LEFT_HRIZ_CH][CONFIG_CALIB_MAX]=ADC_MAX;
tx_config.Stick_Calibaration[IN_RIGHT_VERT_CH][CONFIG_CALIB_MAX]=ADC_MAX;
tx_config.Stick_Calibaration[IN_RIGHT_HRIZ_CH][CONFIG_CALIB_MAX]=ADC_MAX;

configSet_ModelNr(tx_config.modell_Nr);
}






//reset model configuration to default
void ConfigResetModelConfig(uint8_t mode_type)
{
	memcpy(&model_config,&model_config_default[mode_type],MODEL_CONFIG_SIZE); 
}






