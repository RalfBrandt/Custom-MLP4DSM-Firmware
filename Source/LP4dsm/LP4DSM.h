#ifndef LP4DSM_H
#define LP4DSM_H
// ************************** defines*************************************************************

#ifndef F_CPU
#define F_CPU 4194304UL
#endif

#define UPDATE_RATE11MS 11UL  //11ms
#define UPDATE_RATE22MS 22UL  //11ms
#define TICK_RATE 1000	  //1 ms
#define UPDATE_CNT11MS  ((F_CPU*UPDATE_RATE11MS)/1000) //ticks per 11ms
#define UPDATE_CNT22MS  ((F_CPU*UPDATE_RATE22MS)/1000) //ticks per 11ms
#define SEND_ON_IRQ 1 //must be on, otherwise there is to much jitter. must make sure that send on IRQ will give the main program enouth time for the calculations

//includes *********************************************************************
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include <avr/eeprom.h> 


#include "analog.h"
#include "SPECTRUM_TX.h"
#include "timer.h"
#include "BUZZER.h"
#include "LED.h"
#include "KEYS.h"
#include "CURVE.h"
#include "config.h"
#include "STICKS.h"
#include "USER_IF.h"
#include "MODEL.h"
#include "DEFAULT.h"

extern uint32_t keys;


#endif //LP4DSM_H